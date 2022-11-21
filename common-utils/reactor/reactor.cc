#include "common-utils/reactor/reactor.h"

namespace toyps {

Reactor::Reactor(int maxEvent) : epollFd_(epoll_create(512)), events_(maxEvent){
  assert(epollFd_ >= 0 && events_.size() > 0);
}

Reactor::~Reactor() {
  close(epollFd_);
}

bool Reactor::AddFd(int fd, uint32_t events) {
  if(fd < 0) return false;
  epoll_event ev = {0};
  ev.data.fd = fd;
  ev.events = events;
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
}

bool Reactor::ModFd(int fd, uint32_t events) {
  if(fd < 0) return false;
  epoll_event ev = {0};
  ev.data.fd = fd;
  ev.events = events;
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
}

bool Reactor::DelFd(int fd) {
  if(fd < 0) return false;
  epoll_event ev = {0};
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &ev);
}

int Reactor::Wait(int timeoutMs) {
  return epoll_wait(epollFd_, &events_[0], static_cast<int>(events_.size()), timeoutMs);
}

int Reactor::GetEventFd(size_t i) const {
  assert(i < events_.size() && i >= 0);
  return events_[i].data.fd;
}

uint32_t Reactor::GetEvent(size_t i) const {
  assert(i < events_.size() && i >= 0);
  return events_[i].events;
}

/**
 * 主线程负责添加删除文件描述符, 起到acceptor的功能
 * 业务逻辑新开线程进行
*/
void Reactor::Start() {
  std::cout << "Reactor start, epoll_fd " << epollFd_ << ", server_fd " << serverFd_ << std::endl;
  AddFd(serverFd_, EPOLLRDHUP | EPOLLIN);
  SetNonblock(serverFd_);
  
  while (!isClosed_) {
    int eventCnt = Wait();
    for(int eventIdx = 0; eventIdx < eventCnt; eventIdx++){
      int fd = GetEventFd(eventIdx);
      uint32_t events = GetEvent(eventIdx);

      if(fd == serverFd_){
        DealConnect();
      } else if(events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
        CloseClient(&clients_[fd]);
      }
      else if(events & EPOLLIN){
        assert(clients_.count(fd) > 0);
        ThreadPool::GetInstance()->PushTask(std::bind(&Reactor::DealRead, this, &clients_[fd]));
      }
    }
  }
}

void Reactor::Finish() {
  isClosed_ = true;
}

void Reactor::DealConnect() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int clientFd = accept(serverFd_, (struct sockaddr *)&addr, &len);
  AddFd(clientFd, EPOLLIN | EPOLLET);
  SetNonblock(clientFd);

  char ipAddress[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr.sin_addr), ipAddress, INET_ADDRSTRLEN);

  std::cout << "Client connection--- " << ipAddress << ":" << addr.sin_port << " as fd " << clientFd << std::endl;

  assert(clientFd > 0);
  Client client(clientFd, addr);
  clients_[clientFd] = client;
}

void Reactor::CloseClient(Client* client){
  DelFd(client->fd);
  std::cout << "Client leave--- " << client->fd << std::endl;
  clients_.erase(clients_.find(client->fd));
}

void Reactor::DealRead(Client* client) {
  char buf[1024];
  while(true){    //由于使用非阻塞IO，需要不断读取，直到全部读取完毕
    int fd = client->fd;
    ssize_t bytes_read = read(fd, buf, sizeof(buf));

    if(bytes_read > 0){
      std::cout << "Receive msg from " << fd << " :" << buf << std::endl;
      write(fd, buf, bytes_read);
    } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
      continue;
    } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
      //该fd上数据读取完毕
      break;
    } else if(bytes_read == 0){  //EOF事件，一般表示客户端断开连接
      CloseClient(client);   //关闭socket会自动将文件描述符从epoll树上移除
      break;
    }
  }

}


int Reactor::SetNonblock(int fd) {
    assert(fd >= 0);
    int oldOpt = fcntl(fd, F_GETFD, 0);
    fcntl(fd, F_SETFL, oldOpt |  O_NONBLOCK);
    return oldOpt;
}

} // namespace toyps
