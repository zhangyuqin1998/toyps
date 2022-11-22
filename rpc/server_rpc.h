# pragma once

#include "rpc/controller.h"
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

#include "google/protobuf/service.h"

/**
 * 调用链:
 * ServerRpc_Stub::RpcMethod --> Channel::CallMethod --> Reactor --> ServerRpcHandle::DealRead --> ServerRpcHandle::ProcRpcData
 * --> ServerRpc::CallMethod --> ServerRpcImpl::RpcMethod --> ServerRpcHandle::OnCallbackDone
*/

//////////////////////////////////////////////////////////////////////

namespace toyps {

struct ServiceInfo {
  google::protobuf::Service* service_;
  const google::protobuf::ServiceDescriptor* s_descriptor_;
  std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_descriptor_;
};


class ServerRpcHandle {
 public:
  static ServerRpcHandle* GetInstance() {
    static ServerRpcHandle handle;
    return &handle;
  }

  ~ServerRpcHandle() = default;

  void RegisterService(google::protobuf::Service* service, bool ownership);

  int DealRead(int fd);
  
 private:
  ServerRpcHandle() = default;

  void ProcRpcData(const std::string& service_name,
                   const std::string& method_name,
                   const std::string& serialzied_data,
                   const int fd);

  void OnCallbackDone(::google::protobuf::Message* resp_msg, const int fd);

 private:
  std::unordered_map<std::string, ServiceInfo> services_;
};

} //namespace toyps
