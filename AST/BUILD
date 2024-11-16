load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ASTNodes",
    srcs = ["NodeTypes.cpp"],
    hdrs = ["NodeTypes.h"],
    deps = ["//Types:types"],
)

cc_library(
    name = "pretty-printer",
    srcs = ["PrettyPrinter.cpp"],
    hdrs = ["PrettyPrinter.h"],
    deps = [
        ":ASTNodes",
        "//Types:types",
    ],
)
