#pragma once

#include <list>
#include <string>
#include <map>

using namespace std;

class Utf8TextICEncoder {
    private:

        static const char HEX_DIGITS[16];
        static const int16_t MIN_SURROGATE;
        static const int16_t MAX_SURROGATE;

        bool m_defaultSafeOctets[256];

        void initializeSafeOctets();
        void setBit(unsigned int &bitMask, const char& inputChar);

 

        string encode(const string& inputStr);



        void encodeEscapeSequence(const char& inputChar, string& strToUpdate);

    public:
        Utf8TextICEncoder();

        string encodeKeys(const list<string> &keys);
        string encodeKeyValuePairs(const map<string, string> &inputMap);


        
};
