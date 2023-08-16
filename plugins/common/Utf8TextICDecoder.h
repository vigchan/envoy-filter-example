# pragma once
# include "Utf8String.h"
#include <string>
#include <map>
#include <list>

using namespace std;
enum State {
            START,
            COMMA,
            EQUALS,
            END
        };
        
class Utf8TextICDecoder {
    private:
        int m_utf8_lookup_table[256];
        void initializeLookupTable();
        map<char, int> m_hexMap;

        string m_inputStr;
        Utf8String m_utf8Str;
        string m_outputStr;
        unsigned int m_index;

        State m_state;

        void readNextCodePoint(Utf8String& utf8Str, string& result);
        string getNextToken();

        State getState();
    
    public:
        // These will be the entry points to be called from the headers
        void decodeKeyValuePairs(map<string, string>& keyValuePairs);
        void decodeKeys(list<string>& encodedKeys);
        Utf8TextICDecoder(const string&  inputStr);

};
