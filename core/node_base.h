# pragma once

#include "core/connector_base.h"

namespace toyps {

class NodeBase {
 public:
  virtual void Pull() = 0;
  virtual void Push() = 0;

  virtual void Start() = 0;
  virtual void Finish() = 0;
  
  virtual ~NodeBase() = default;
 protected:
  spConnectorBase connector_;
};
} // namespace toyps

