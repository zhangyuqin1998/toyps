#include "core/server_node.h"

namespace toyps {

ServerNode::ServerNode(std::string ip, int port) :
        synchronization_type_(SynchronizationType::Sync),
        cur_id_(0),
        timeout_(1.0) {
  connector_ = std::make_unique<BasicServerConnector>(ip, port);
  reactor_ = std::make_unique<Reactor>();
}

void ServerNode::Start() {
  auto ret = std::async(std::launch::async, &ServerNode::ListenHeartBeat, this);
  // std::thread(&ServerNode::ListenHeartBeat, this).detach();
  connector_->Start();
  reactor_->SetServerFd(connector_->GetFd());
  reactor_->Start(); // 死循环
}

void ServerNode::Finish() {
  reactor_->Finish();
}

void ServerNode::Pull() {}
void ServerNode::Push() {}

void ServerNode::PrintWorkers() {
  std::cout << "Now has " << workers_.size() << " worker";
  for (auto p : workers_) {
    std::cout << " " << p.first ;
  }
  std::cout << std::endl;
}

void ServerNode::Register(::google::protobuf::RpcController* controller,
                          const server_rpc::RegisterRequest* request,
                          server_rpc::RegisterResponse* response,
                          ::google::protobuf::Closure* done) {
  mtx_.lock();
  std::cout << "Register id " << cur_id_ << std::endl;
  response->set_id(cur_id_);
  workers_[cur_id_] = std::chrono::steady_clock::now();
  cur_id_++;

  PrintWorkers();

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

void ServerNode::HeartBeat(::google::protobuf::RpcController* controller,
                          const server_rpc::HeartBeatRequest* request,
                          server_rpc::HeartBeatResponse* response,
                          ::google::protobuf::Closure* done) {
  auto worker_id = request->id();
  workers_[worker_id] = std::chrono::steady_clock::now();
  done->Run();
}

void ServerNode::ListenHeartBeat() {
  std::vector<WorkerId> drop;
  while (1) {
    drop.clear();
    for (auto p : workers_) {
      if (std::chrono::steady_clock::now() - p.second > timeout_) {
        drop.push_back(p.first);
      }
    }

    for (auto id : drop) {
      std::cout << "Worker " << id << " lose connect!" << std::endl;
      auto iter = workers_.find(id);
      workers_.erase(iter);
    }
    if (drop.size() > 0) {
      PrintWorkers();
    }
    usleep(700 * 1000);
  }
}

} // namespace toyps