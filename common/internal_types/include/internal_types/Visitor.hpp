#pragma once

namespace internal_types
{
template <class... Ts>
struct Visitor : Ts...
{
    using Ts::operator()...;
};
// template <class... Ts>
// Visitor(Ts...)->Visitor<Ts...>;
} // namespace internal_types
