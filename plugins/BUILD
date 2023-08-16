load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_cc_test",
)
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_package")

api_proto_package()
package(default_visibility = ["//visibility:public"])

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "//plugins/test-request-header:test_request_header_config",
        "//plugins/test-plugin-header:test_plugin_header_config",
        "//plugins/test-response-header:test_response_header_config",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)
