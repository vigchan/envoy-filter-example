#pragma once

#include<string>
#include<vector>
using namespace std;

class Utf8String : public string {
    
    private:
    unsigned int m_currIndex;
    const string m_inputStr;
    vector<int16_t> m_doubleByteStr;
    static const unsigned int MIN_SUPPLEMENTARY_CODE_POINT;

    public:

    Utf8String(const string& str);
    int16_t getBytes();
    pair<char, char> getCharBytes();
    void advanceIndex();
    bool bytesLeft();
    bool isSurrogatePair(const int32_t &high, const int32_t &low);
    int32_t toCodePoint(const int16_t& high, const int16_t & low);
    static const unsigned int MIN_HIGH_SURROGATE;
    static const unsigned int MIN_LOW_SURROGATE;

    static string getCharsFromBytes(int16_t bytes);
    char getCurrChar();
};
