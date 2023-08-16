#include <string>
#include <stdexcept>

#include "test_plugin_header_filter.h"
#include "plugins/common/Utf8TextICDecoder.h"
#include "MemberTokenParser.h"
#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

TestPluginHeaderFilterConfig::TestPluginHeaderFilterConfig(
    const sample::TestPluginResponseEcho& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

TestPluginHeaderFilter::TestPluginHeaderFilter(TestPluginHeaderFilterConfigSharedPtr config)
    : config_(config) {}

TestPluginHeaderFilter::~TestPluginHeaderFilter() {}

void TestPluginHeaderFilter::onDestroy() {}

const LowerCaseString TestPluginHeaderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string TestPluginHeaderFilter::headerValue() const {
  return config_->val();
}

std::string base64_encode(const std::string &in) {

    std::string out;

    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}


FilterHeadersStatus TestPluginHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {

  HeaderMap::GetResult result = headers.get(LowerCaseString(std::string("X-LI-R2-W-IC-1")));
  if(result.size() <= 0) {
      return FilterHeadersStatus::Continue;
  }
  std::string icData;
  auto strView = result[0]->value().getStringView();
  for(size_t i=0; i < strView.length(); i++) {
      icData += strView[i]; 
  }
  
  map<std::string, std::string> outputKeyValuePairs;
  Utf8TextICDecoder decoder2(icData);
  decoder2.decodeKeyValuePairs(outputKeyValuePairs);
  
  MemberTokenParser memberTokenParser(outputKeyValuePairs["mik"]);
  MemberToken memberTokenInput = memberTokenParser.getParsedToken();

  // add a header
  sample::MemberToken memberToken;
  memberToken.set_version(memberTokenInput.m_version);
  memberToken.set_version_key(memberTokenInput.m_keyVersion);
  memberToken.set_member_id(memberTokenInput.m_memberId);
  memberToken.set_session_id(memberTokenInput.m_sessionId);

  std::string out;
  memberToken.SerializeToString(&out);
  headers.setCopy(LowerCaseString(std::string("X-Li-ProtoBuf-Member-Header")), base64_encode(out));
  headers.setCopy(LowerCaseString(std::string("X-Li-MemberId-Header")), std::to_string(memberToken.member_id()));
  
  
  return FilterHeadersStatus::Continue;
}

FilterDataStatus TestPluginHeaderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void TestPluginHeaderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
