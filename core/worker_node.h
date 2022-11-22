# pragma once

#include "core/node_base.h"
#include "core/basic_connector.h"
// #include "common-utils/threadpool.h"
#include "proto/server_rpc.pb.h"
#include "proto/meta.pb.h"
#include "rpc/channel.h"
#include "rpc/controller.h"
#include <future>
#include <string>
// #include <chrono>
// #include <sys/time.h>
// #include <ctime>

namespace toyps {

class WorkerNode : public NodeBase {
 public:
  WorkerNode(std::string server_ip, int server_port);
  ~WorkerNode() = default;

  void Start();
  void Finish();

 public:
  void Pull();
  void Push();

  void HeartBeat();

  void Register();

  void Echo(std::string msg);

 private:
  uint32_t GetId() { return id_; }
 private:
  uint32_t id_;

  // std::chrono::milliseconds interval_;
  // std::chrono::milliseconds last_answer_;

  std::unique_ptr<RpcChannel> channel_;
  std::unique_ptr<server_rpc::ServerRpc_Stub> stub_;
};

} //namespace toyps
