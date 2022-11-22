#include "rpc/server_rpc.h"

namespace toyps {
/**
 * 解包, 转发rpc请求
*/
int ServerRpcHandle::DealRead(int fd) {
  // meta_size
  char rpc_meta_buf[sizeof(int)]; 
  size_t len = read(fd, rpc_meta_buf, sizeof(rpc_meta_buf));
  if(len == 0) {
    return -1;
  }
  int rpc_meta_size = *(int*)rpc_meta_buf;

  // meta_data
  std::vector<char> rpc_meta_data(rpc_meta_size, 0);
  read(fd, rpc_meta_data.data(), rpc_meta_size);

  meta::RpcMeta rpc_meta_data_proto;
  rpc_meta_data_proto.ParseFromString(std::string(&rpc_meta_data[0], rpc_meta_data.size()));

  // request_data
  std::vector<char> request_data(rpc_meta_data_proto.data_size(), 0);
  read(fd, request_data.data(), rpc_meta_data_proto.data_size());


  ProcRpcData(rpc_meta_data_proto.service_name(), rpc_meta_data_proto.method_name(), 
      std::string(&request_data[0], request_data.size()), fd);
  return 0;
}

/**
 * 处理rpc请求
 * 调用CallMethod函数进行处理
*/
void ServerRpcHandle::ProcRpcData(const std::string& service_name,
                                  const std::string& method_name,
                                  const std::string& serialzied_data,
                                  const int fd) {
  auto service = services_[service_name].service_;
  auto mdescriptor = services_[service_name].m_descriptor_[method_name];
  auto recv_msg = service->GetRequestPrototype(mdescriptor).New();
  auto resp_msg = service->GetResponsePrototype(mdescriptor).New();
  recv_msg->ParseFromString(serialzied_data);
  auto done = google::protobuf::NewCallback(
    this, &ServerRpcHandle::OnCallbackDone, resp_msg, fd);
  
  RpcController controller;
  service->CallMethod(mdescriptor, &controller, recv_msg, resp_msg, done);
}

/**
 * 回包
*/
void ServerRpcHandle::OnCallbackDone(::google::protobuf::Message* resp_msg, const int fd)
{
  int serialized_size = resp_msg->ByteSize();
  std::string resp_data;
  resp_data.insert(0, std::string((const char*)&serialized_size, sizeof(int)));
  resp_msg->AppendToString(&resp_data);
  
  //resp_msg->SerializeToString(&resp_data);
  write(fd, resp_data.data(), resp_data.size());
}

void ServerRpcHandle::RegisterService(google::protobuf::Service* service, bool ownership) {
  std::string     method_name;
  ServiceInfo     service_info;
  const ::google::protobuf::ServiceDescriptor* sdescriptor = service->GetDescriptor();
  for (int i = 0; i < sdescriptor->method_count(); ++i) {
    method_name = sdescriptor->method(i)->name();
    service_info.m_descriptor_[method_name] = sdescriptor->method(i);
  }
  
  service_info.service_ = service;
  services_[sdescriptor->name()] = service_info;
}

}//namespace toyps
