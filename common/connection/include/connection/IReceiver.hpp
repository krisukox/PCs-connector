#pragma once

#include <boost/asio.hpp>
#include "internal_types/CommonTypes.hpp"

namespace connection
{
using SuccessfulCallback_ = std::function<void(internal_types::Event)>;
using UnsuccessfulCallback_ = std::function<void(boost::system::error_code)>;

class IReceiver
{
public:
    virtual void receive(SuccessfulCallback_, UnsuccessfulCallback_) = 0;
    virtual ~IReceiver() = default;
};
} // namespace connection
