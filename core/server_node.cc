#include "core/server_node.h"

namespace toyps {

ServerNode::ServerNode(std::string ip, int port) :
        cur_id_(0),
        synchronization_type_(SynchronizationType::Sync) {
  connector_ = std::make_unique<BasicServerConnector>(ip, port);
  reactor_ = std::make_unique<Reactor>();
}

void ServerNode::Start() {
  connector_->Start();
  reactor_->SetServerFd(connector_->GetFd());
  reactor_->Start();
}

void ServerNode::Finish() {
  reactor_->Finish();
}

void ServerNode::Pull() {}
void ServerNode::Push() {}

} // namespace toyps