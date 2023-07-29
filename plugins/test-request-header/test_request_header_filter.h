#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "plugins/test-request-header/test_request_header.pb.h"

namespace Envoy {
namespace Http {

class TestRequestHeaderFilterConfig {
public:
  TestRequestHeaderFilterConfig(const sample::TestRequestEcho& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

using TestRequestHeaderFilterConfigSharedPtr = std::shared_ptr<TestRequestHeaderFilterConfig>;

class TestRequestHeaderFilter : public PassThroughDecoderFilter {
public:
  TestRequestHeaderFilter(TestRequestHeaderFilterConfigSharedPtr);
  ~TestRequestHeaderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const TestRequestHeaderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy
