
load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])


cc_library(
    name = "evaluator",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
        "//AST:ASTNodes",
        "//AST:pretty-printer",
        "//Types:types",
    ],
)