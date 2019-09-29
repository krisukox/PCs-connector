#pragma once

#include <boost/asio.hpp>
#include "internal_types/CommonTypes.hpp"

namespace connection
{
using SuccessfulCallback = std::function<void(internal_types::Event)>;
using UnsuccessfulCallback = std::function<void(boost::system::error_code)>;

class IReceiver
{
public:
    virtual void receive(SuccessfulCallback, UnsuccessfulCallback) = 0;
    virtual ~IReceiver() = default;
};
} // namespace connection
