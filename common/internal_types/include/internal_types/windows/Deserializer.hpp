#pragma once

#include "internal_types/CommonTypes.hpp"
#include "internal_types/IDeserializer.hpp"
#include "internal_types/ScreenResolution.hpp"

namespace internal_types
{
class Deserializer : public IDeserializer
{
public:
    Deserializer();

    ~Deserializer() override = default;
    std::optional<internal_types::DecodedType> decode(const internal_types::Buffer&) const override;
    internal_types::Event decodeEvent(const internal_types::Buffer&) const;
    internal_types::ScreenResolution decodeScreenResolution(const internal_types::Buffer&) const;
};
} // namespace internal_types
