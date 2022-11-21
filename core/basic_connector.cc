#include "core/basic_connector.h"

namespace toyps {

int BasicServerConnector::Start() {
  int ret = 0;

  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(sockaddr_in));
  fd_ = socket(AF_INET, SOCK_STREAM, 0);

  serveraddr.sin_family=AF_INET;
  serveraddr.sin_port=htons(port_);
  inet_pton(AF_INET, ip_.c_str(), &serveraddr.sin_addr);

  ret=bind(fd_,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
  if (ret) {
    std::cerr << "BasicServerConnector::Start fail to bind!" << std::endl;
    return ret;
  }
  ret=listen(fd_, 10);
  if (ret) {
    std::cerr << "BasicServerConnector::Start fail to listen!" << std::endl;
    return ret;
  }

  return ret;
}

int BasicWorkerConnector::Start() {
  int ret = 0;
  server_fd_ = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_adr;
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(server_ip_.c_str());
  serv_adr.sin_port = htons(server_port_);

  ret = connect(server_fd_, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
  if (ret) {
    std::cerr << "BasicWorkerConnector::Start fail to connect!" << std::endl;
    return ret;
  }

  return ret;
}

int BasicWorkerConnector::Send(char* data, size_t len) {
  assert(len > 0);
  ssize_t write_bytes = write(server_fd_, data, len);
  return write_bytes;
}

int BasicWorkerConnector::Receive() {
  char buf[1024];
  memset(buf, 0, sizeof(buf));
  while(1) {
    ssize_t read_bytes = read(server_fd_, buf, sizeof(buf));
    if(read_bytes > 0){
      std::cout << "Receive msg from server : " << buf << std::endl;
      return 0;
    } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
      continue;
    } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
      //该fd上数据读取完毕
      return 0;
    } else if(read_bytes == 0){  //EOF事件，一般表示客户端断开连接
      std::cout << "Server is closed" << std::endl;
      return 0;
    }
  }
  return 0;
}
} // namespace toyps
