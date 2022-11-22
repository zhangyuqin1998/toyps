cc_library(
    name = "core_lib",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
        "//common-utils:reactor_lib",
        "//common-utils:threadpool_lib",
        "//rpc:lib_rpc",
        "//proto:lib_meta_cc_pb", 
        "//proto:lib_server_rpc_cc_pb", 
    ],
)

package(default_visibility = ["//visibility:public"]) 
