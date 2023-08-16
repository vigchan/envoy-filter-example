#pragma once

#include <string>

#include "source/extensions/filters/http/common/pass_through_filter.h"
#include "source/common/json/json_loader.h"

#include "plugins/test-response-header/test_response_header.pb.h"

namespace Envoy {
namespace Http {

class TestResponseHeaderFilterConfig {
public:
  TestResponseHeaderFilterConfig(const sample::TestResponseEcho& proto_config);

  const std::string& appname() const { return m_appname; }
  const std::string& hostname() const { return m_hostname; }
  const std::string& servicename() const { return m_servicename; }
  const std::string& env() const { return m_env; }
  const std::string& instance() const { return m_instance; }

private:
  const std::string m_appname;
  const std::string m_hostname;
  const std::string m_servicename;
  const std::string m_env;
  const std::string m_instance;
};

using TestResponseHeaderFilterConfigSharedPtr = std::shared_ptr<TestResponseHeaderFilterConfig>;

class TestResponseHeaderFilter : public PassThroughDecoderFilter {
public:
  TestResponseHeaderFilter(TestResponseHeaderFilterConfigSharedPtr);
  ~TestResponseHeaderFilter();

  // Http::StreamFilterBase
  void onDestroy() override;

  // Http::StreamEncoderFilter
  FilterHeadersStatus decodeHeaders(RequestHeaderMap&, bool) override;
  FilterDataStatus decodeData(Buffer::Instance&, bool) override;
  void setDecoderFilterCallbacks(StreamDecoderFilterCallbacks&) override;

private:
  const TestResponseHeaderFilterConfigSharedPtr config_;
  StreamDecoderFilterCallbacks* encoder_callbacks_;
  std::string generateCallTreeData(Json::ObjectSharedPtr jsonloader);
};

} // namespace Http
} // namespace Envoy
