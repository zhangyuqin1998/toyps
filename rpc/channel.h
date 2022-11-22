# pragma once

#include "google/protobuf/service.h"
#include "google/protobuf/message.h"
#include "proto/meta.pb.h"
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h> // close()
#include <vector>
#include <unordered_map>
namespace toyps {
class RpcChannel : public google::protobuf::RpcChannel {
 public:
  RpcChannel(std::string& server_addr, int port);
  virtual ~RpcChannel() = default;
  virtual void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                          ::google::protobuf::RpcController* controller,
                          const ::google::protobuf::Message* request,
                          ::google::protobuf::Message* response,
                          ::google::protobuf::Closure* done);

 private:
  std::string server_addr_;
  int port_;

  int server_fd_;
};

} //namespace toyps
