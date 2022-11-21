# pragma once

#include "core/connector_base.h"

#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h> // close()
#include <vector>
#include <unordered_map>

namespace toyps {
  
class BasicServerConnector : public ConnectorBase {
 public:
  BasicServerConnector(std::string ip, int port) : ip_(ip), port_(port) { }
  ~BasicServerConnector() = default;

  int Start();
  int Send(char* data, size_t len) {return 0;}
	int Receive() {return 0;}

  int GetFd() { return fd_; }

 private:
  std::string ip_;
  int port_;
  int fd_;
};
using spBasicServerConnector = std::unique_ptr<BasicServerConnector>;


class BasicWorkerConnector : public ConnectorBase {
 public:
  BasicWorkerConnector(std::string server_ip, int server_port) : 
    server_ip_(server_ip), server_port_(server_port) { }
  virtual ~BasicWorkerConnector() = default;

  int Start();
  int Send(char* data, size_t len);
	int Receive();

  int GetFd() { return server_fd_; }

 private:
  std::string server_ip_;
  int server_port_;
  int server_fd_;
};
using spBasicWorkerConnector = std::unique_ptr<BasicWorkerConnector>;

} // namespace toyps
