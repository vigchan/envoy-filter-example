#include <string>
#include <stdexcept>
#include <unistd.h>
#include "test_request_header_filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

TestRequestHeaderFilterConfig::TestRequestHeaderFilterConfig(
    const sample::TestRequestEcho& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

TestRequestHeaderFilter::TestRequestHeaderFilter(TestRequestHeaderFilterConfigSharedPtr config)
    : config_(config) {}

TestRequestHeaderFilter::~TestRequestHeaderFilter() {}

void TestRequestHeaderFilter::onDestroy() {}

const LowerCaseString TestRequestHeaderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string TestRequestHeaderFilter::headerValue() const {
  return config_->val();
}

const std::string gen_random(const int len) {
  static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

FilterHeadersStatus TestRequestHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  // add a header
  for(int i=0; i < 100; i++) {
    headers.setCopy(LowerCaseString(std::string("user-agent-") + std::to_string(i)), std::string("test-sample-header-response") + std::to_string(i));
  }

  headers.setCopy(LowerCaseString(std::string("user-agent-100")), gen_random(1000) + std::to_string(100));
  headers.setCopy(LowerCaseString(std::string("user-agent-101")), gen_random(1000) + std::to_string(101));
  headers.setCopy(LowerCaseString(std::string("user-agent-102")), gen_random(1000) + std::to_string(102));
  return FilterHeadersStatus::Continue;
}

FilterDataStatus TestRequestHeaderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void TestRequestHeaderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
