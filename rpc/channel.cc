#include "rpc/channel.h"

namespace toyps {

RpcChannel::RpcChannel(std::string& server_addr, int port) :
  server_addr_(server_addr), port_(port){
  
  server_fd_ = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_adr;
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(server_addr.c_str());
  serv_adr.sin_port = htons(port);

  if (connect(server_fd_, (struct sockaddr*)&serv_adr, sizeof(serv_adr))) {
    std::cerr << "RpcChannel fail to connect!" << std::endl;
  }
}

void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                          ::google::protobuf::RpcController* controller,
                          const ::google::protobuf::Message* request,
                          ::google::protobuf::Message* response,
                          ::google::protobuf::Closure* done) {

  //---------------------------request---------------------------//
  std::string request_data_str;
  request->SerializeToString(&request_data_str);

  meta::RpcMeta rpc_meta;
  rpc_meta.set_service_name(method->service()->name());
  rpc_meta.set_method_name(method->name());
  rpc_meta.set_data_size(request_data_str.size());

  std::string rpc_meta_str;
  rpc_meta.SerializeToString(&rpc_meta_str);

  // meta_size + meta_data + request_data
  std::string serialzied_str;

  int rpc_meta_str_size = rpc_meta_str.size();
  serialzied_str.insert(0, std::string((const char*)&rpc_meta_str_size, sizeof(int)));
  serialzied_str += rpc_meta_str;
  serialzied_str += request_data_str;
  
  write(server_fd_, serialzied_str.c_str(), serialzied_str.size());

  //---------------------------response---------------------------//

  char resp_data_size[sizeof(int)];
  read(server_fd_, resp_data_size, sizeof(int));
  int resp_data_len = *(int*)resp_data_size;
  std::vector<char> resp_data(resp_data_len, 0);
  read(server_fd_, resp_data.data(), resp_data_len);
  
  response->ParseFromString(std::string(&resp_data[0], resp_data.size()));
}

} //namespace toyps