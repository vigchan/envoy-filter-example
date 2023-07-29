#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "plugins/test-request-header/test_request_header.pb.h"
#include "plugins/test-request-header/test_request_header.pb.validate.h"
#include "test_request_header_filter.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class TestRequestHeaderFilterConfigFactory : public NamedHttpFilterConfigFactory {
public:
  Http::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                     const std::string&,
                                                     FactoryContext& context) override {

    return createFilter(Envoy::MessageUtil::downcastAndValidate<const sample::TestRequestEcho&>(
                            proto_config, context.messageValidationVisitor()),
                        context);
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new sample::TestRequestEcho()};
  }

  std::string name() const override { return "testRequestEcho"; }

private:
  Http::FilterFactoryCb createFilter(const sample::TestRequestEcho& proto_config, FactoryContext&) {
    Http::TestRequestHeaderFilterConfigSharedPtr config =
        std::make_shared<Http::TestRequestHeaderFilterConfig>(
            Http::TestRequestHeaderFilterConfig(proto_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::TestRequestHeaderFilter(config);
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
    };
  }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<TestRequestHeaderFilterConfigFactory, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
