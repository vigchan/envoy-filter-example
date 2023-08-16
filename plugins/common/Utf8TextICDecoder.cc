#include "Utf8TextICDecoder.h"
#include "Utf8String.h"
#include "Utf8TextICEncoder.h"
#include <stdexcept>
#include <iostream>

void Utf8TextICDecoder::initializeLookupTable() {
    for(unsigned int c = 0; c < 128; c++) {
        m_utf8_lookup_table[c] = 0;
    }

    for(unsigned int c = 128; c <  256; c++) {
        int code;
        if ((c & 0xE0) == 0xC0) { // 2 bytes (0x0080 - 0x07FF)
            code = 2;
        } else if ((c & 0xF0) == 0xE0) { // 3 bytes (0x0800 - 0xFFFF)
            code = 3;
        } else if ((c & 0xF8) == 0xF0) {
            // 4 bytes; double-char with surrogates and all...
            code = 4;
        } else {
            // And -1 seems like a good "universal" error marker...
            code = -1;
        }
        m_utf8_lookup_table[c] = code;
    }

    const unsigned char hexDigits[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    
    for(int i =0; i < 16; i++) {
        m_hexMap.insert(pair<char, int>(hexDigits[i], i));
    }
}


Utf8TextICDecoder::Utf8TextICDecoder(const string& inputStr)
    :m_inputStr(inputStr),  m_utf8Str(inputStr) {
        m_index = 0;
        m_state = START;

    // TODO: Remove this from here.
    initializeLookupTable();
}

void Utf8TextICDecoder::decodeKeyValuePairs(map<string, string>& keyValuePairs) {
    if(m_inputStr.empty()) {
        return;
    }

    string key, value;
    bool expectKey = true;

    string token = getNextToken();
    while(!token.empty()) {
        if(expectKey) {
            if(getState() == COMMA) {
                continue;
            }

            key = token;
            expectKey = false;
        } 
        else {
            if(getState() != EQUALS && !key.empty()) {
                value = token;
                keyValuePairs.insert(pair<string, string>(key, value));
            }
            expectKey = true;
            key = "";
        }
        token = getNextToken();
    }
}

void Utf8TextICDecoder::decodeKeys(list<string>& encodedKeys) {
    if(m_inputStr.empty()) {
        return;
    }

    string key, value;

    string token = getNextToken();
    while(!token.empty()) {
        if(getState() == EQUALS) {
            continue;
        }
        encodedKeys.push_back(token);
        token = getNextToken(); 
    }
}

State Utf8TextICDecoder::getState() {
    return m_state;
}

string Utf8TextICDecoder::getNextToken() {
    string result;
    if(m_state == END) {
        return result;
    }

    bool nonWhitespaceLiteralEncountered = false;
    unsigned int trailingWhitespaceCharCount = 0;
    bool done = false;

    while(!done && m_utf8Str.bytesLeft()) {
        int16_t uniChar = m_utf8Str.getBytes();
        char ch = static_cast<char>(uniChar);
        switch(ch) {
            case '+': {
                nonWhitespaceLiteralEncountered = true;
                result += ' ';
                m_utf8Str.advanceIndex();
                break;
            }
                
            case '%': {
                nonWhitespaceLiteralEncountered = true;
                // TODO: Is this reading the next codePoint?
                m_utf8Str.advanceIndex();
                int16_t v = m_hexMap[m_utf8Str.getCurrChar()] << 4;
                m_utf8Str.advanceIndex();
                v |= m_hexMap[m_utf8Str.getCurrChar()];
                result += static_cast<char>(v);
                m_utf8Str.advanceIndex();

                unsigned int bytesNeeded = m_utf8_lookup_table[v];
                switch (bytesNeeded) {
                    case 0:  {
                        break;
                    }
                        
                    case 2: {
                        readNextCodePoint(m_utf8Str, result);
                        break;
                    }
                        
                    case 3: {
                        readNextCodePoint(m_utf8Str, result);
                        readNextCodePoint(m_utf8Str, result);
                        break;
                    }
                        
                    case 4: {
                        readNextCodePoint(m_utf8Str, result);
                        readNextCodePoint(m_utf8Str, result);
                        readNextCodePoint(m_utf8Str, result);
                        break;
                    } 
                        
                    default:
                        string exceptionStr = "Escape sequence decoded to invalid UTF-8 codepoint ";
                        exceptionStr += static_cast<char>(v);
                        throw invalid_argument(exceptionStr);
                }   
                break;
            } 
                
            case '=': {
                m_state = EQUALS;
                m_utf8Str.advanceIndex();
                done = true;
                break;
            }
            case ',': {
                m_state = COMMA;
                m_utf8Str.advanceIndex();
                done = true;
                break;
            }
            default: 
                m_utf8Str.advanceIndex();
                if(!isspace(ch)) {
                    nonWhitespaceLiteralEncountered = true;
                    trailingWhitespaceCharCount = 0;
                    result += ch;
                } 
                else if(nonWhitespaceLiteralEncountered) {
                    trailingWhitespaceCharCount ++;
                    result += ch;
                }
                break;
        }
    } 
    if(!m_utf8Str.bytesLeft()) {
        m_state = END;
    }
    return result;
}

void Utf8TextICDecoder::readNextCodePoint(Utf8String& utf8Str, string& result) {

    int16_t currUniChar = utf8Str.getBytes();
    char currChar = static_cast<char>(currUniChar);
    if(currChar != '%') {
        throw invalid_argument("Expected but could not find start of escape sequence");
    }

     utf8Str.advanceIndex();
     char firstChar = utf8Str.getCurrChar();
     utf8Str.advanceIndex();
     char secondChar = utf8Str.getCurrChar();
     utf8Str.advanceIndex();
     result += static_cast<char>(m_hexMap[firstChar] << 4 | m_hexMap[secondChar]);

    
}
