#pragma once

#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "internal_types/CommonTypes.hpp"
#include "server_app/ServerAppTypes.hpp"
#include <variant>

namespace internal_types {
class IDeserializer {
public:
  virtual ~IDeserializer() = default;

  virtual std::variant<KeyEvent, MouseEvent>
  decode(const internal_types::Buffer &) const = 0;
};
} // namespace internal_types
