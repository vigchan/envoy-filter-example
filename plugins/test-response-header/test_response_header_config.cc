#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "plugins/test-response-header/test_response_header.pb.h"
#include "plugins/test-response-header/test_response_header.pb.validate.h"
#include "test_response_header_filter.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class TestResponseHeaderFilterConfigFactory : public NamedHttpFilterConfigFactory {
public:
  Http::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                     const std::string&,
                                                     FactoryContext& context) override {

    return createFilter(Envoy::MessageUtil::downcastAndValidate<const sample::TestResponseEcho&>(
                            proto_config, context.messageValidationVisitor()),
                        context);
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new sample::TestResponseEcho()};
  }

  std::string name() const override { return "testResponseEcho"; }

private:
  Http::FilterFactoryCb createFilter(const sample::TestResponseEcho& proto_config, FactoryContext&) {
    Http::TestResponseHeaderFilterConfigSharedPtr config =
        std::make_shared<Http::TestResponseHeaderFilterConfig>(
            Http::TestResponseHeaderFilterConfig(proto_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::TestResponseHeaderFilter(config);
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
    };
  }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<TestResponseHeaderFilterConfigFactory, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
