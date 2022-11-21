# pragma once

#include <memory>

namespace toyps {

class ConnectorBase {
 public:
  virtual int Start() = 0;

  virtual int Send(char* data, size_t len) = 0;
	virtual int Receive() = 0;
  virtual int GetFd() = 0;
};

using spConnectorBase = std::unique_ptr<ConnectorBase>;

} // namespace toyps
