# pragma once

#include "core/node_base.h"
#include "core/basic_connector.h"
#include "common-utils/reactor/reactor.h"
#include "common-utils/threadpool/threadpool.h"
#include "proto/server_rpc.pb.h"
#include "proto/meta.pb.h"
#include "google/protobuf/service.h"
#include <atomic>
#include <memory>
#include <unordered_map>
#include <string>
#include <mutex>
// #include <chrono>
// #include <sys/time.h>
// #include <ctime>

namespace toyps {

enum class SynchronizationType {
  Sync,
  Asyn,
};

class ServerNode : public NodeBase, public server_rpc::ServerRpc {
  using KVNameSpace = std::string;
  using KVpairs = std::unordered_map<int, float>;
  using WorkerId = uint32_t;

 public:
  ServerNode(std::string ip, int port);
  ~ServerNode() = default;

  void Start();
  void Finish();

  void SetSynchronizationType(SynchronizationType new_type) { synchronization_type_ = new_type; }
  SynchronizationType GetSynchronizationType() { return synchronization_type_; }

 private:
  void HeartBeat();
  void AcceptWorker();
  void DeleteWorker();
  void DispatchId();

  void Register(::google::protobuf::RpcController* controller,
                const server_rpc::RegisterRequest* request,
                server_rpc::RegisterResponse* response,
                ::google::protobuf::Closure* done);

  void Echo(::google::protobuf::RpcController* controller,
            const server_rpc::EchoRequest* request,
            server_rpc::EchoResponse* response,
            ::google::protobuf::Closure* done);

  void Pull();
  void Push();

 private:
  std::unordered_map<KVNameSpace, KVpairs> params_;
  std::unordered_map<WorkerId, int> workers_;
  // std::unordered_map<WorkerId, std::chrono::milliseconds;> heat_beat_info_;

  // std::chrono::milliseconds interval_;

  spReactor reactor_;

  SynchronizationType synchronization_type_;

  uint32_t cur_id_;

  std::mutex mtx_;
};
} // namespace toyps
