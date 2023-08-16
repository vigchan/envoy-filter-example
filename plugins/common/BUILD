load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
    "envoy_cc_library",
    "envoy_cc_test",
)
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_package")

api_proto_package()
package(default_visibility = ["//visibility:public"])

envoy_cc_library(
    name = "test_plugins_common",
    srcs = ["Utf8String.cc", "Utf8TextICDecoder.cc", "Utf8TextICEncoder.cc"],
    hdrs = ["Utf8String.h", "Utf8TextICDecoder.h", "Utf8TextICEncoder.h"],
    repository = "@envoy",
    deps = [],
)

