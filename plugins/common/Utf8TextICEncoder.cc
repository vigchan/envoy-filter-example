#include "Utf8TextICEncoder.h"
#include "Utf8String.h"


const char Utf8TextICEncoder::HEX_DIGITS[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const int16_t Utf8TextICEncoder::MIN_SURROGATE = 0xD800;

const int16_t Utf8TextICEncoder::MAX_SURROGATE = 0xDFFF;

Utf8TextICEncoder::Utf8TextICEncoder() {
    initializeSafeOctets();
}

void Utf8TextICEncoder::initializeSafeOctets() {

    for(char ch='0'; ch <= '9'; ch++) {
        m_defaultSafeOctets[ch] = true;
    }

    for(char ch='A'; ch <= 'Z'; ch++) {
        m_defaultSafeOctets[ch] = true;
    }

    for(char ch='a'; ch <= 'z'; ch++) {
       m_defaultSafeOctets[ch] = true;
    }

    const char otherChars[] = {'-', '_', '.', '*'};
    const unsigned int otherCharsLength  = sizeof(otherChars) / sizeof(char);
    for(int i=0; i < static_cast<int>(otherCharsLength); i++) {
        m_defaultSafeOctets[otherChars[i]] = true;
    }

}

string Utf8TextICEncoder::encodeKeys(const list<string> &keys) {

    string result;

    if(keys.empty()) {
        return result;
    }

    for(auto iter=keys.begin(); iter != keys.end(); iter++) {
        if(result.length() > 0) {
            result += ',';
        }

        result += encode(*iter);
    }
    return result;

}

string Utf8TextICEncoder::encodeKeyValuePairs(const map<string, string> &inputMap) {
    if(inputMap.empty()) {
        return "";
    }

    string result;
    for(auto i:inputMap) {
        if(!result.empty()) {
            result += ',';
        }
        result += encode(i.first) + "=";
        result += encode(i.second);

    }
    return result;
}

string Utf8TextICEncoder::encode(const string& inputStr) {
    string result;
    Utf8String doubleByteStr(inputStr);

    while(doubleByteStr.bytesLeft()) {
        int16_t currBytes = doubleByteStr.getBytes();

        if(currBytes < 0x80) {
            char currChar = static_cast<char>(currBytes);
            if(m_defaultSafeOctets[static_cast<int>(currChar)]) {
                result += currChar;
            }
            else if(currChar == ' ') {
                result += '+';
            }
            else {
               encodeEscapeSequence(static_cast<char>(currBytes), result);
            }

        }
        else if(currBytes < 0x800) {
            char byteVal = ((0xF << 6) | currBytes >> 6);
            encodeEscapeSequence(byteVal, result);
            byteVal = (0x80 | (0x3F & currBytes));
            encodeEscapeSequence(byteVal, result);
        }
        else if(currBytes < MIN_SURROGATE || MAX_SURROGATE < currBytes) {
            char byteVal = ((0xF << 5) | (currBytes >> 12));
            encodeEscapeSequence(byteVal, result);
            byteVal = (0x80 | (0x3F & (currBytes >> 6)));
            encodeEscapeSequence(byteVal, result);
            byteVal = (0x80 | (0x3F & currBytes));
            encodeEscapeSequence(0x80 | (0x3F & currBytes), result);
        }
        else {
            doubleByteStr.advanceIndex();
            int16_t low = doubleByteStr.getBytes();
            if(!doubleByteStr.isSurrogatePair(currBytes, low)) {
                // TODO: Add more details here.
                throw invalid_argument("Unpaired surrogate index"); 
            }
            int32_t value = doubleByteStr.toCodePoint(currBytes, low);
            encodeEscapeSequence(((0xF << 4) | (value >> 18)), result);
            encodeEscapeSequence((0x80 | (0x3F & (value >> 12))), result);
            encodeEscapeSequence((0x80 | (0x3F & (value >> 6))), result);
            encodeEscapeSequence((0x80 | (0x3F & value)), result);
        }
        doubleByteStr.advanceIndex();
    }
    return result;
}


void Utf8TextICEncoder::encodeEscapeSequence(const char& inputChar, string& strToUpdate) {
    int16_t intChar = inputChar;

    // convert from [-128, 127] to [0, 255]
    if(intChar < 0) {
        intChar += 256;
    }
    strToUpdate += '%';
    strToUpdate += HEX_DIGITS[intChar >> 4];
    strToUpdate += HEX_DIGITS[intChar & 0xF];
}
