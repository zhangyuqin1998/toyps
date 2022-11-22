#include "core/server_node.h"

namespace toyps {

ServerNode::ServerNode(std::string ip, int port) :
        synchronization_type_(SynchronizationType::Sync),
        cur_id_(0) {
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


void ServerNode::Register(::google::protobuf::RpcController* controller,
                          const server_rpc::RegisterRequest* request,
                          server_rpc::RegisterResponse* response,
                          ::google::protobuf::Closure* done) {
  mtx_.lock();
  std::cout << "Register id " << cur_id_ << std::endl;
  response->set_id(cur_id_);
  workers_[cur_id_] = 0;
  cur_id_++;

  std::cout << "Now has " << workers_.size() << " worker";
  for (auto p : workers_) {
    std::cout << " " << p.first ;
  }
  std::cout << std::endl;

  mtx_.unlock();
  
  done->Run();                      
}

void ServerNode::Echo(::google::protobuf::RpcController* controller,
                      const server_rpc::EchoRequest* request,
                      server_rpc::EchoResponse* response,
                      ::google::protobuf::Closure* done) {
  std::cout << "Echo msg " << request->msg() << " from " << request->id() << std::endl;
  response->set_msg(request->msg());
  done->Run();
}
} // namespace toyps