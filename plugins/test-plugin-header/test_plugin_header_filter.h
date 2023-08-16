#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"

#include "plugins/test-plugin-header/test_plugin_header.pb.h"

namespace Envoy {
namespace Http {

class TestPluginHeaderFilterConfig {
public:
  TestPluginHeaderFilterConfig(const sample::TestPluginResponseEcho& proto_config);

  const std::string& key() const { return key_; }
  const std::string& val() const { return val_; }

private:
  const std::string key_;
  const std::string val_;
};

using TestPluginHeaderFilterConfigSharedPtr = std::shared_ptr<TestPluginHeaderFilterConfig>;

class TestPluginHeaderFilter : public PassThroughDecoderFilter {
public:
  TestPluginHeaderFilter(TestPluginHeaderFilterConfigSharedPtr);
  ~TestPluginHeaderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamDecoderFilter
  FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const TestPluginHeaderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* decoder_callbacks_;

  const LowerCaseString headerKey() const;
  const std::string headerValue() const;
};

} // namespace Http
} // namespace Envoy
