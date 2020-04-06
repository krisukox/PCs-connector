#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <unordered_map>
#include "Deserializer.hpp"
#include "internal_types/Serializer.hpp"
#include "internal_types/Visitor.hpp"

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

template <class T, typename = std::enable_if_t<std::is_convertible<T, internal_types::DecodedType>::value>>
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

class Socket
{
public:
    template <class T>
    using SuccessfulCallback = std::function<void(T)>;
    using UnsuccessfulCallback = std::function<void(boost::system::error_code)>;

    Socket();

    Socket(const boost::asio::ip::address&, const std::string& port);
    Socket(const std::string& port);

    void start();
    void close();

    template <class T>
    void receive(SuccessfulCallback<T> successfulCallback)
    {
        //        std::unique_ptr<BaseHandler> handler = std::make_unique<Handler<T>>(successfulCallback);
        //        handlers.insert(std::make_pair(typeid(T).hash_code(), handler));//DLACZEGO TO DZIAŁA(NIE TRZEBA MOVE)

        handlers.insert(
            {typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback, HandlerType::multiple)});

        //        my_map.insert({s_t, std::move(handler)});

        //        socket.async_receive(
        //            boost::asio::buffer(buffer, 5),
        //            [this, successfulCallback, unsuccessfulCallback](boost::system::error_code errorCode, std::size_t
        //            size) {
        //                if (size > 0 && !errorCode)
        //                {
        //                    handleReceivedData(successfulCallback, unsuccessfulCallback);
        //                }
        //                else
        //                {
        //                    unsuccessfulCallback(errorCode);
        //                }
        //            });
    }

    template <class T>
    void receiveOnce(SuccessfulCallback<T> successfulCallback)
    {
        handlers.insert({typeid(T).hash_code(), std::make_unique<Handler<T>>(successfulCallback, HandlerType::once)});
        //        if (socket.receive(boost::asio::buffer(buffer, 5)) != 5)
        //        {
        //            unsuccessfulCallback(boost::system::error_code());
        //        }
        //        handleReceivedData(successfulCallback, unsuccessfulCallback);
        //        ioContext.stop();
    }

    template <class T>
    void send(T msgToSend)
    {
        boost::asio::async_write(
            socket,
            boost::asio::buffer(serializer.encode<T>(msgToSend), 5),
            [this](boost::system::error_code ec, std::size_t) {
                if (!ec)
                {
                    // LATER CHECK IF VECTOR OF MSGS HAS ANY MSGS LEFT
                }
                else
                {
                    socket.close();
                }
            });
    }

private:
    //    template <class T, typename = std::enable_if_t<std::is_convertible<T, internal_types::DecodedType>::value>>
    //    Handler<T>* getHandler(const T& data)
    //    {
    //        auto it = handlers.find(typeid(T).hash_code());
    //        if (it != handlers.end())
    //        {
    //            Handler<T>* handler;
    //            if (it->second->handlerType == HandlerType::once)
    //            {
    //                handler = dynamic_cast<Handler<T>*>(it->second.release());
    //                handlers.erase(it);
    //            }
    //            else
    //            {
    //                handler = dynamic_cast<Handler<T>*>(it->second.get());
    //            }
    //            if (handler)
    //            {
    //                if (handler->handlerType == HandlerType::once)
    //                {
    //                    handler->run(data);
    //                    handlers.erase(it);
    //                }
    //                else
    //                {
    //                    handler->run(data);
    //                }
    //            }
    //            else
    //            {
    //                //        unsuccessfulCallback(boost::system::error_code());
    //            }
    //        }
    //        else
    //        {
    //            //        unsuccessfulCallback(boost::system::error_code());
    //        }
    //    }

    template <class T, typename = std::enable_if_t<std::is_convertible<T, internal_types::DecodedType>::value>>
    void handleReceivedType(const T& data)
    {
        std::cout << "void handleReceivedData(const T& _data)" << std::endl;
        std::unique_lock<std::mutex> uniqueLock(mutex);
        auto it = handlers.find(typeid(T).hash_code());
        if (it != handlers.end())
        {
            Handler<T>* handler;
            if (it->second->handlerType == HandlerType::once)
            {
                handler = dynamic_cast<Handler<T>*>(it->second.release());
                handlers.erase(it);
                uniqueLock.unlock();
                if (handler)
                {
                    handler->run(data);
                    delete handler;
                    return;
                }
            }
            else
            {
                handler = dynamic_cast<Handler<T>*>(it->second.get());
                uniqueLock.unlock();
                if (handler)
                {
                    handler->run(data);
                    return;
                }
            }
        }
        else
        {
            std::cerr << "Cannot find handler for " << typeid(T).name() << " type" << std::endl;
        }
    }
    void handleReceivedData(const internal_types::Buffer&);

    void startReceiving();

    internal_types::Deserializer deserializer;
    internal_types::Serializer serializer;
    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::socket socket;

    internal_types::Buffer buffer;

    std::unordered_map<std::size_t, std::unique_ptr<BaseHandler>> handlers;
    std::vector<std::thread> handlerThreads;
    std::thread socketThread;

    boost::asio::io_context ioContextHandlers;
    std::thread handlerIoContextThread;

    std::mutex mutex;
};
} // namespace connection
