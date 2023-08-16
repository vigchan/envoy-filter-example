#include <string>

#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "plugins/test-plugin-header/test_plugin_header.pb.h"
#include "plugins/test-plugin-header/test_plugin_header.pb.validate.h"
#include "test_plugin_header_filter.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class TestPluginHeaderFilterConfigFactory : public NamedHttpFilterConfigFactory {
public:
  Http::FilterFactoryCb createFilterFactoryFromProto(const Protobuf::Message& proto_config,
                                                     const std::string&,
                                                     FactoryContext& context) override {

    return createFilter(Envoy::MessageUtil::downcastAndValidate<const sample::TestPluginResponseEcho&>(
                            proto_config, context.messageValidationVisitor()),
                        context);
  }

  /**
   *  Return the Protobuf Message that represents your config incase you have config proto
   */
  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return ProtobufTypes::MessagePtr{new sample::TestPluginResponseEcho()};
  }

  std::string name() const override { return "testPluginEcho"; }

private:
  Http::FilterFactoryCb createFilter(const sample::TestPluginResponseEcho& proto_config, FactoryContext&) {
    Http::TestPluginHeaderFilterConfigSharedPtr config =
        std::make_shared<Http::TestPluginHeaderFilterConfig>(
            Http::TestPluginHeaderFilterConfig(proto_config));

    return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
      auto filter = new Http::TestPluginHeaderFilter(config);
      callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{filter});
    };
  }
};

/**
 * Static registration for this sample filter. @see RegisterFactory.
 */
static Registry::RegisterFactory<TestPluginHeaderFilterConfigFactory, NamedHttpFilterConfigFactory>
    register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
