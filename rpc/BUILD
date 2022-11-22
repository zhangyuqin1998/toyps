load("@rules_cc//cc:defs.bzl", "cc_proto_library") 
load("@rules_proto//proto:defs.bzl", "proto_library") 

cc_library( 
    name = "lib_rpc", 
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [ 
        "//proto:lib_meta_cc_pb", 
        "//proto:lib_server_rpc_cc_pb", 
    ], 
)

package(default_visibility = ["//visibility:public"]) 