#include "core/worker_node.h"

namespace toyps {

WorkerNode::WorkerNode(std::string server_ip, int server_port) :
id_(-1) {
  connector_ = std::make_unique<BasicWorkerConnector>(server_ip, server_port);
  channel_ = std::make_unique<RpcChannel>(server_ip, server_port);
  stub_ = std::make_unique<server_rpc::ServerRpc_Stub>(channel_.get());
}

void WorkerNode::Start() {
  // connector_->Start();
  auto ret = std::async(std::launch::async, &WorkerNode::HeartBeat, this);
}

void WorkerNode::Finish() {}

void WorkerNode::Pull() {}
void WorkerNode::Push() {}

void WorkerNode::Register() {
  server_rpc::RegisterRequest   request;
  server_rpc::RegisterResponse  response;
  RpcController controller;
  stub_->Register(&controller, &request, &response, nullptr);

  if (controller.Failed()) {
    std::cout << "request failed: %s" << controller.ErrorText().c_str();
  } else {
    std::cout << "get Register id " << response.id() << std::endl;
    id_ = response.id();
  }
}

void WorkerNode::Echo(std::string msg) {
  server_rpc::EchoRequest   request;
  server_rpc::EchoResponse  response;
  RpcController controller;
  request.set_id(id_);
  request.set_msg(msg);
  stub_->Echo(&controller, &request, &response, nullptr);

  if (controller.Failed()) {
    std::cout << "request failed: %s" << controller.ErrorText().c_str();
  } else {
    std::cout << "get Echo msg " << response.msg() << std::endl;
  }
}

void WorkerNode::HeartBeat() {
  server_rpc::HeartBeatRequest   request;
  server_rpc::HeartBeatResponse  response;
  RpcController controller;
  while (1) {
    request.set_id(id_);
    stub_->HeartBeat(&controller, &request, &response, nullptr); 
    usleep(300 * 1000);
  }
}

} // namespace toyps