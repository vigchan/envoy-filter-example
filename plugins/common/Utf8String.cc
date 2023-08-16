# include "Utf8String.h"
#include <stdexcept>
# include <string>
#include <utility>
#include <iostream>


using namespace std;

const unsigned int Utf8String::MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;
const unsigned int Utf8String::MIN_HIGH_SURROGATE = 0xD800;
const unsigned int Utf8String::MIN_LOW_SURROGATE = 0xDC00;

Utf8String::Utf8String(const string& inputStr):m_inputStr(inputStr) {
    for(int i=0; i < static_cast<int>(inputStr.length()); i++) {
        m_doubleByteStr.push_back(static_cast<int16_t>(inputStr[i]));
    }
    m_currIndex = 0;

}

int16_t Utf8String::getBytes() {
    return m_doubleByteStr[m_currIndex];
}

void Utf8String::advanceIndex() {
    m_currIndex += 1;
}

bool Utf8String::bytesLeft() {
    return m_currIndex < m_doubleByteStr.size();
}

bool Utf8String::isSurrogatePair(const int32_t &high, const int32_t &low) {
    return high >= 0xD800 && high <= 0xDBFF && low >= 0xDC00 && low <= 0xDFFF;
}

int32_t Utf8String::toCodePoint(const int16_t &high, const int16_t &low) {

    return ((high << 10) + low) + (MIN_SUPPLEMENTARY_CODE_POINT
                                       - (MIN_HIGH_SURROGATE << 10)
                                       - MIN_LOW_SURROGATE);
}

char Utf8String::getCurrChar() {
    return m_inputStr[m_currIndex];
}

string Utf8String::getCharsFromBytes(int16_t bytes) {
    string result;
    result.reserve(2);
    result += static_cast<char>(bytes);
    bytes = bytes >> 8;
    result += static_cast<char>(bytes);
    return result;
}





