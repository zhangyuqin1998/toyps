cc_library(
  name = "reactor_lib",
  srcs = ["reactor/reactor.cc"],
  hdrs = ["reactor/reactor.h"],
  deps = [":threadpool_lib"],
)

cc_library(
  name = "threadpool_lib",
  hdrs = ["threadpool/threadpool.h"],
  deps = ["//rpc:lib_rpc"],
  linkopts = ["-lpthread"],
)

package(default_visibility = ["//visibility:public"]) 
