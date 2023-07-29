#include <string>
#include <stdexcept>

#include "test_response_header_filter.h"

#include "envoy/server/filter_config.h"

namespace Envoy {
namespace Http {

TestResponseHeaderFilterConfig::TestResponseHeaderFilterConfig(
    const sample::TestResponseEcho& proto_config)
    : key_(proto_config.key()), val_(proto_config.val()) {}

TestResponseHeaderFilter::TestResponseHeaderFilter(TestResponseHeaderFilterConfigSharedPtr config)
    : config_(config) {}

TestResponseHeaderFilter::~TestResponseHeaderFilter() {}

void TestResponseHeaderFilter::onDestroy() {}

const LowerCaseString TestResponseHeaderFilter::headerKey() const {
  return LowerCaseString(config_->key());
}

const std::string TestResponseHeaderFilter::headerValue() const {
  return config_->val();
}

FilterHeadersStatus TestResponseHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {
  // add a header
  headers.setCopy(headerKey(), headerValue());
  Http::RequestHeaderMap::GetResult currHeader = headers.get(LowerCaseString("via"));
  std::string currHeaderValue(currHeader[0]->value().getStringView());
  headers.addCopy(LowerCaseString(std::string("User-Agent")), std::string("Another Ghost from ennvoy") + currHeaderValue);
  //headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-2")), std::string("This is header 2"));
  // headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-3")), std::string("This is header 3"));
  // headers.addCopy(LowerCaseString(std::string("X-LinkedIn-Header-4")), std::string("This is header 4"));
  return FilterHeadersStatus::Continue;
}

FilterDataStatus TestResponseHeaderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void TestResponseHeaderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  decoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
