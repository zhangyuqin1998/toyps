#include "core/worker_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     8888
#define BUFSIZE            1024
int main() {
  toyps::WorkerNode worker_node(SERVER_IP, SERVER_PORT);
  worker_node.Start();

  while(true){
    char buf[BUFSIZE];
    memset(buf, 0, sizeof(buf));
    printf("ps < ");

    std::cin.getline(buf, BUFSIZE);

    worker_node.EchoSend(buf, strlen(buf));
    worker_node.EchoReceive();
  }
}