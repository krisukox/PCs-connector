#pragma once

#include <gmock/gmock.h>
#include <optional>
#include "connection/IReceiver.hpp"

namespace mocks
{
struct ReceiverMock : connection::IReceiver
{
    void receive(
        connection::SuccessfulCallback successfulCallback_,
        connection::UnsuccessfulCallback unsuccessfulCallback_) override
    {
        startReceive();

        successfulCallback = std::move(successfulCallback_);
        unsuccessfulCallback = std::move(unsuccessfulCallback_);
    }

    connection::SuccessfulCallback successfulCallback;
    connection::UnsuccessfulCallback unsuccessfulCallback;

    MOCK_METHOD0(startReceive, void());
};
} // namespace mocks
