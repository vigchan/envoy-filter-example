# include "MemberTokenParser.h"
# include<iostream>

MemberTokenParser::MemberTokenParser(const string& inputStr) {
    m_inputTokenStr = base64Decode(inputStr);
}

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static int getIntValue(const int& startIndex, const string& bytes) {
    int value = 0;

    value = value | static_cast<uint8_t>(bytes[startIndex]);
    value = value << 8;

    value = value | static_cast<uint8_t>(bytes[startIndex+1]);
    value = value << 8;

    value = value | static_cast<uint8_t>(bytes[startIndex+2]);
    value = value << 8;

    value = value | static_cast<uint8_t>(bytes[startIndex+2]);
    value = value << 8;

    return value;
}

static uint16_t getDoubleBytes(const int& startIndex, const string& bytes) {
    uint16_t value = 0;

    value = value | static_cast<uint8_t>(bytes[startIndex]);
    value = value << 8;

    value = value | static_cast<uint8_t>(bytes[startIndex+1]);
    return value;
}

MemberToken MemberTokenParser::getParsedToken() {
    MemberToken memberToken;
    memberToken.m_version = static_cast<uint16_t>(m_inputTokenStr[0]);
    memberToken.m_payloadSize = getDoubleBytes(1, m_inputTokenStr);
    memberToken.m_keyVersion = static_cast<uint8_t>(m_inputTokenStr[3]);
    memberToken.m_memberId = static_cast<int>(getIntValue(4, m_inputTokenStr));
    memberToken.m_sessionId = static_cast<int>(getIntValue(8, m_inputTokenStr));
    return memberToken;
}

string MemberTokenParser::base64Decode(const string& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;

}
