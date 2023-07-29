#include <string>
#include <stdexcept>

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

FilterHeadersStatus TestRequestHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  // add a header
  headers.setCopy(headerKey(), headerValue());
  headers.addCopy(LowerCaseString(std::string("User-Agent")), std::string("The Ghost from envoy"));
  //headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-2")), std::string("This is header 2"));
  // headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-3")), std::string("This is header 3"));
  // headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-4")), std::string("This is header 4"));
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
