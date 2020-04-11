#pragma once

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
    void run(T event) const
    {
        if (functionType)
        {
            functionType(event);
        }
    }

private:
    FunctionType functionType;
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
        //        std::lock_guard<std::mutex> _(mutex);
        //        firstHandler = std::make_unique<Handler<T>>(successfulCallback, HandlerType::once);
        handlers.insert({typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback, HandlerType::once)});
    }

    template <class T>
    void registerMultiple(SuccessfulCallback<T> successfulCallback)
    {
        //        std::lock_guard<std::mutex> _(mutex);
        //        secondHandler = std::make_unique<Handler<T>>(successfulCallback, HandlerType::multiple);
        handlers.insert(
            {typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback, HandlerType::multiple)});
    }

private:
    void start();

    template <class T, typename = std::enable_if<std::is_convertible<T, internal_types::DecodedType>::value>>
    void handleReceivedType(const T& _data)
    {
        //        std::unique_lock<std::mutex> uniqueLock(mutex);
        auto it = handlers.find(typeid(T).hash_code());
        if (it != handlers.end())
        {
            Handler<T>* handler;
            if (it->second->handlerType == HandlerType::once)
            {
                handler = static_cast<Handler<T>*>(it->second.release());
                handlers.erase(it);
                //                uniqueLock.unlock();
                //                if (handler)
                //                {
                //                    boost::asio::post(boost::asio::bind_executor(ioContext, [handler, data =
                //                    std::move(_data)]() {
                //                        handler->run(data);
                //                        delete handler;
                //                    }));
                //                    ioContext.run();
                handler->run(_data);
                delete handler;
                return;
                //                }
            }
            else
            {
                handler = static_cast<Handler<T>*>(it->second.get());
                //                uniqueLock.unlock();
                //                if (handler)
                //                {
                //                    boost::asio::post(boost::asio::bind_executor(
                //                        ioContext, [handler, data = std::move(_data)]() { handler->run(data); }));
                //                    ioContext.run();
                handler->run(_data);
                return;
                //                }
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
    std::mutex mutex;

    boost::asio::io_context ioContext;
};
} // namespace connection
