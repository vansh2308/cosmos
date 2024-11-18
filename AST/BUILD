load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ASTNodes",
    srcs = ["nodeTypes.cpp"],
    hdrs = ["nodeTypes.h"],
    deps = ["//Types:types"],
)

cc_library(
    name = "pretty-printer",
    srcs = ["prettyPrinter.cpp"],
    hdrs = ["prettyPrinter.h"],
    deps = [
        ":ASTNodes",
        "//Types:types",
    ],
)
