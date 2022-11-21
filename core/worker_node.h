# pragma once

#include "core/node_base.h"
#include "core/basic_connector.h"
// #include "common-utils/threadpool.h"

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

  void Pull();
  void Push();

  void EchoSend(char* data, size_t len);
  void EchoReceive();
 private:
  uint32_t GetId() { return id_; }
 private:
  uint32_t id_;

  // std::chrono::milliseconds interval_;
  // std::chrono::milliseconds last_answer_;
};

} //namespace toyps
