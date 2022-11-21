#include "core/worker_node.h"

namespace toyps {

WorkerNode::WorkerNode(std::string server_ip, int server_port) :
id_(-1) {
  connector_ = std::make_unique<BasicWorkerConnector>(server_ip, server_port);
}

void WorkerNode::Start() {
  connector_->Start();
}

void WorkerNode::EchoSend(char* data, size_t len) {
  connector_->Send(data, len);
}

void WorkerNode::EchoReceive() {
  connector_->Receive();
}

void WorkerNode::Finish() {}

void WorkerNode::Pull() {}
void WorkerNode::Push() {}

} // namespace toyps