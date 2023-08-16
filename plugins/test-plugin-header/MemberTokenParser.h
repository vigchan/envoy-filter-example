#pragma once

#include <string>

using namespace std;

struct MemberToken {
    uint16_t m_version;
    uint16_t m_payloadSize;
    uint8_t m_keyVersion;
    uint32_t m_memberId;
    uint32_t m_sessionId;
};

class MemberTokenParser {

    private:
    string m_inputTokenStr;
    string base64Decode(const string &inputToken);


    public:
    MemberTokenParser(const string& inputStr);
    MemberToken getParsedToken();


};