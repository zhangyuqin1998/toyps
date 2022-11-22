#include "core/server_node.h"

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     8888

int main() {
  toyps::ServerRpcHandle* rpc_handle = toyps::ServerRpcHandle::GetInstance();
  toyps::ServerNode server_node(SERVER_IP, SERVER_PORT);
  rpc_handle->RegisterService(&server_node, false);
  server_node.Start();
}