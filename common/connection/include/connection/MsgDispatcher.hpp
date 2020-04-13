#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <type_traits>
#include "Deserializer.hpp"
#include "boost/asio.hpp"
#include "internal_types/CommonTypes.hpp"

namespace connection
{
enum class HandlerType
{
    once,
    multiple
};

class BaseHandler
{
public:
    BaseHandler(const HandlerType& _handlerType) : handlerType{_handlerType} {}
    virtual ~BaseHandler() {}
    const HandlerType handlerType;
};

template <class T, typename = std::enable_if<std::is_convertible<T, internal_types::DecodedType>::value>>
class Handler : public BaseHandler
{
    using FunctionType = std::function<void(T)>;

public:
    Handler() {}
    Handler(FunctionType _functionType, HandlerType _handlerType)
        : BaseHandler(_handlerType), functionType{_functionType}
    {
    }
    void run(T event)
    {
        //        if (handlerThread.joinable()) handlerThread.join();
        //        handlerThread = std::thread([this](T event) { functionType(event); }, event);
        //        std::unique_lock uniqueLock{mutex};
        //        if (valueToHandle)
        //        {
        //            conditionVariable.wait(uniqueLock, [this]() { return !valueToHandle; });
        //        }
        //        valueToHandle = event;
        //        uniqueLock.unlock();
        //        conditionVariable.notify_all();
        //
        //
        //        std::optional<T> val = std::nullopt;
        //        std::optional<T> eventOpt = event;
        //        //        auto eventOpt = std::make_optional(event);
        //        while (!value1.compare_exchange_strong(val, eventOpt))
        //        {
        //            val = std::nullopt;
        //        }
        //        value = std::move(event);
        if (functionType)
        {
            functionType(event);
        }
    }
    void runOnce(T event) const
    {
        if (functionType)
        {
            functionType(event);
        }
    }

    std::optional<T> valueToHandle;

private:
    FunctionType functionType;
    std::thread handlerThread;

    std::condition_variable conditionVariable;
    std::optional<T> anyValue = T();
    std::mutex mutex;
};

class MsgDispatcher
{
public:
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;

    MsgDispatcher(std::unique_ptr<internal_types::Deserializer>);

    void handleReceivedData(const internal_types::Buffer&);

    template <class T>
    void registerOnce(SuccessfulCallback<T> successfulCallback)
    {
        firstHandler = std::make_unique<Handler<T>>(successfulCallback, HandlerType::once);
        //        handlers.insert({typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback,
        //        HandlerType::once)});
    }

    template <class T>
    void registerMultiple(SuccessfulCallback<T> successfulCallback)
    {
        secondHandler = std::make_unique<Handler<T>>(successfulCallback, HandlerType::multiple);
        //        handlers.insert(
        //            {typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback, HandlerType::multiple)});
    }

private:
    void start();

    template <class T, typename = std::enable_if<std::is_convertible<T, internal_types::DecodedType>::value>>
    void handleReceivedType(const T& _data)
    {
        auto it = handlers.find(typeid(T).hash_code());
        if (it != handlers.end())
        {
            Handler<T>* handler;
            if (it->second->handlerType == HandlerType::once)
            {
                handler = static_cast<Handler<T>*>(it->second.release());
                handlers.erase(it);
                handler->runOnce(_data);
                delete handler;
                return;
            }
            else
            {
                handler = static_cast<Handler<T>*>(it->second.get());
                handler->run(_data);
                return;
            }
        }
        else
        {
            std::cerr << "Cannot find handler for " << typeid(T).name() << " type" << std::endl;
        }
    }

    std::unique_ptr<internal_types::Deserializer> deserializer;
    std::unordered_map<std::size_t, std::unique_ptr<BaseHandler>> handlers;

    std::unique_ptr<BaseHandler> firstHandler;
    std::unique_ptr<BaseHandler> secondHandler;
};
} // namespace connection
