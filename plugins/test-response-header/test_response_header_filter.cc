#include <string>
#include <stdexcept>

#include "test_response_header_filter.h"
#include "plugins/common/Utf8TextICDecoder.h"
#include "plugins/common/Utf8TextICEncoder.h"
#include "envoy/server/filter_config.h"
//#include "source/common/common/logger.h"
//#include "source/common/common/assert.h"

namespace Envoy {
namespace Http {

TestResponseHeaderFilterConfig::TestResponseHeaderFilterConfig(
    const sample::TestResponseEcho& proto_config)
    : m_appname(proto_config.appname()), m_hostname(proto_config.hostname()),
      m_servicename(proto_config.servicename()), m_env(proto_config.env()) {}

TestResponseHeaderFilter::TestResponseHeaderFilter(TestResponseHeaderFilterConfigSharedPtr config)
    : config_(config) {}

TestResponseHeaderFilter::~TestResponseHeaderFilter() {}

void TestResponseHeaderFilter::onDestroy() {}

std::string getBooleanAsStr(bool value) {
   return value ? std::string("true") : std::string("false");
}

std::string TestResponseHeaderFilter::generateCallTreeData(Json::ObjectSharedPtr jsonLoader) {
   std::string json_str;
   std::string version = jsonLoader -> getObject("caller")->getString("version");
  
   
   json_str += "{\"caller\":{"; 
   json_str += "\"container\":\"" + config_->appname() + "\",";
   json_str += "\"service\":\"" + config_->servicename() + "\",";
   json_str += "\"env\":\"" + config_->env() + "\",";
   json_str += "\"machine\":\"" + config_->hostname() + "\",";
   json_str += "\"instance\":\"" + config_->instance() + "\",";
   json_str += "\"version\":\"" + version + "\"";
   json_str += "},";

 
   json_str += "\"treeId\":\"" + jsonLoader->getString("treeId") + "\",";
   json_str += "\"enabled\":" + getBooleanAsStr(jsonLoader->getBoolean("enabled"))  + ",";
   json_str += "\"scale\":" + std::to_string(jsonLoader->getInteger("scale")) + ",";
   json_str += "\"rqId\":" + std::to_string(jsonLoader->getInteger("rqId")) + ",";
   json_str += "\"version\":" + std::to_string(jsonLoader->getInteger("version")) + ",";
   
   Json::ObjectSharedPtr contextPtr = jsonLoader -> getObject("context");  
   json_str += "\"context\":{"; 
   json_str += "\"source\":\"" + contextPtr->getString("source") + "\",";
   json_str += "\"forceTraceEnabled\":\"" + contextPtr->getString("forceTraceEnabled")  + "\",";
   json_str += "\"debugEnabled\":\"" + contextPtr->getString("debugEnabled") + "\",";
   json_str += "\"traceGroupingKey\":\"" + contextPtr->getString("traceGroupingKey") + "\"";
   json_str += "}}";

   return json_str;
}


FilterHeadersStatus TestResponseHeaderFilter::decodeHeaders(RequestHeaderMap& headers, bool) {

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

  if(outputKeyValuePairs.find("serviceCallTraceData") == outputKeyValuePairs.end()) {
     return FilterHeadersStatus::Continue;
  }

  std::string serviceTraceDataStr = outputKeyValuePairs["serviceCallTraceData"];
  //ENVOY_STREAM_LOG(
  //        trace, "vignesh - serviceTraceDataStr: {}", *decoder_callbacks_, serviceTraceDataStr);
  Json::ObjectSharedPtr loader = Json::Factory::loadFromString(serviceTraceDataStr);
  std::string jsonResponse = generateCallTreeData(loader);
  //ENVOY_STREAM_LOG(
  //        trace, "vignesh - jsonResponse: {}", *decoder_callbacks_, jsonResponse);
  outputKeyValuePairs["serviceCallTraceData"] = jsonResponse;

  Utf8TextICEncoder encoder;
  std::string encoded = encoder.encodeKeyValuePairs(outputKeyValuePairs);
  //ENVOY_STREAM_LOG(
  //        trace, "vignesh - encoded: {}", *decoder_callbacks_, encoded);
  headers.setCopy(LowerCaseString(std::string("X-LI-R2-W-IC-1")), encoded);
  return FilterHeadersStatus::Continue;
}

FilterDataStatus TestResponseHeaderFilter::decodeData(Buffer::Instance&, bool) {
  return FilterDataStatus::Continue;
}

void TestResponseHeaderFilter::setDecoderFilterCallbacks(StreamDecoderFilterCallbacks& callbacks) {
  encoder_callbacks_ = &callbacks;
}

} // namespace Http
} // namespace Envoy
