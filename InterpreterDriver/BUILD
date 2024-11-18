load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "interpreter-driver",
    srcs = ["InterpreterDriver.cpp"],
    hdrs = ["InterpreterDriver.h"],
    deps = [
        "//AST:ASTNodes",
        "//AST:pretty-printer",
        "//Evaluator:evaluator",
        "//Parser:parser",
        "//Scanner:scanner",
        "//Types:types",
    ],

)