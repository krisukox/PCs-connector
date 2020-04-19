#pragma once

#include <gmock/gmock.h>
#include <optional>
#include "internal_types/CommonTypes.hpp"
#include "internal_types/Visitor.hpp"

namespace mocks
{
struct SocketMock
{
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;

    template <class T, typename = std::enable_if<std::is_convertible<T, internal_types::DecodedType>::value>>
    void receiveOnce(const SuccessfulCallback<T>& successfulCallback)
    {
        receiveOnce();
        receiveOnceHandler = successfulCallback;
    }

    void receive(SuccessfulCallback<internal_types::DecodedType> successfulCallback)
    {
        receive();
        receiveHandler = successfulCallback;
    }

    template <class T>
    void send(T msgToSend)
    {
        send();
    }

    template <class T>
    void receivedOnce(const T& receivedValue)
    {
        std::visit(
            internal_types::Visitor{
                [receivedValue](const SuccessfulCallback<T>& handler) { handler(receivedValue); },
                [](const auto& handler) {},
            },
            receiveOnceHandler);
    }

    void received(const internal_types::DecodedType& receivedValue) { receiveHandler(receivedValue); }

    std::variant<SuccessfulCallback<internal_types::Event>, SuccessfulCallback<internal_types::ScreenResolution>>
        receiveOnceHandler;

    SuccessfulCallback<internal_types::DecodedType> receiveHandler;

    MOCK_METHOD(void, receiveOnce, ());
    MOCK_METHOD(void, receive, ());
    MOCK_METHOD(void, send, ());
};
} // namespace mocks
