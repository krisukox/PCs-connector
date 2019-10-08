#pragma once

#include "IDeserializer.hpp"
#include "internal_types/CommonTypes.hpp"

namespace internal_types
{
class Deserializer : public IDeserializer
{
public:
    Deserializer();

    ~Deserializer() override = default;
    internal_types::Event decode(const internal_types::Buffer&) const override;
};
} // namespace internal_types
