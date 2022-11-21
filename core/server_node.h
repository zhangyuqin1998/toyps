# pragma once

#include "core/node_base.h"
#include "core/basic_connector.h"
#include "common-utils/reactor/reactor.h"
#include "common-utils/threadpool/threadpool.h"

#include <memory>
#include <unordered_map>
#include <string>
// #include <chrono>
// #include <sys/time.h>
// #include <ctime>

namespace toyps {

enum class SynchronizationType {
  Sync,
  Asyn,
};

class ServerNode : public NodeBase {
  using NameSpace = std::string;
  using KVpairs = std::unordered_map<int, float>;
  using WorkerId = uint32_t;

 public:
  ServerNode(std::string ip, int port);
  ~ServerNode() = default;

  void Start();
  void Finish();

  void Pull();
  void Push();

  void SetSynchronizationType(SynchronizationType new_type) { synchronization_type_ = new_type; }
  SynchronizationType GetSynchronizationType() { return synchronization_type_; }

 private:
  void HeartBeat();
  void AcceptWorker();
  void DeleteWorker();
  void DispatchId();

 private:
  std::unordered_map<NameSpace, KVpairs> params_;
  // std::unordered_map<WorkerId, std::chrono::milliseconds;> heat_beat_info_;

  // std::chrono::milliseconds interval_;

  spReactor reactor_;

  uint32_t cur_id_;

  SynchronizationType synchronization_type_;
};
} // namespace toyps
