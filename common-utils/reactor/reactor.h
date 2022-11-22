# pragma once

#include "common-utils/threadpool/threadpool.h"
#include "rpc/server_rpc.h"

#include <memory>
#include <sys/epoll.h> //epoll_ctl()
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h> // close()
#include <vector>
#include <unordered_map>
#include <errno.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace toyps {

class Reactor {
 public:
  explicit Reactor(int maxEvent=1024);
  ~Reactor();

  void Start();
  void Finish();

  bool AddFd(int fd, uint32_t events);
  bool ModFd(int fd, uint32_t events);
  bool DelFd(int fd);

  void SetServerFd(int serverFd) { serverFd_ = serverFd; }
 private:
  struct Client {
    int fd;
    struct sockaddr_in addr;
    Client(int fd_in, struct sockaddr_in addr_in) :
      fd(fd_in), addr(addr_in) { }
    Client() = default;
  };

 private:
  int SetNonblock(int fd);

  int Wait(int timeout=-1);

  int GetEventFd(size_t i) const;
  uint32_t GetEvent(size_t i) const;

  void DealConnect();
  void CloseClient(Client* client);
  void DealRead(Client* client);

 private:
  bool isClosed_ = false;

  int epollFd_;
  int serverFd_;
  std::vector<struct epoll_event> events_;

  std::unordered_map<int, Client> clients_;
};

using spReactor = std::unique_ptr<Reactor>;

} // namespace toyps
