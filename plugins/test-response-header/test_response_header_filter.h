#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "plugins/test-response-header/test_response_header.pb.h"

namespace Envoy {
namespace Http {

class TestResponseHeaderFilterConfig {
public:
  TestResponseHeaderFilterConfig(const sample::TestResponseEcho& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

using TestResponseHeaderFilterConfigSharedPtr = std::shared_ptr<TestResponseHeaderFilterConfig>;

class TestResponseHeaderFilter : public PassThroughDecoderFilter {
public:
  TestResponseHeaderFilter(TestResponseHeaderFilterConfigSharedPtr);
  ~TestResponseHeaderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const TestResponseHeaderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy
