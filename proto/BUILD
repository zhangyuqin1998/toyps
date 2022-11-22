load("@rules_cc//cc:defs.bzl", "cc_proto_library") 
load("@rules_proto//proto:defs.bzl", "proto_library") 



cc_proto_library( 
    name = "lib_meta_cc_pb", 
    deps = [":lib_meta_pb"], 
)

cc_proto_library( 
    name = "lib_server_rpc_cc_pb", 
    deps = [":lib_server_rpc_pb"], 
) 

proto_library( 
    name = "lib_meta_pb", 
    srcs = ["meta.proto"], 
)

proto_library( 
    name = "lib_server_rpc_pb", 
    srcs = ["server_rpc.proto"], 
) 

package(default_visibility = ["//visibility:public"]) 