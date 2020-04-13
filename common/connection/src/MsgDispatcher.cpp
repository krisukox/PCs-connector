#include "connection/MsgDispatcher.hpp"
#include <sys/syscall.h>
#include <unistd.h>
#include "internal_types/Visitor.hpp"

#define gettid() syscall(SYS_gettid)

namespace connection
{
MsgDispatcher::MsgDispatcher(std::unique_ptr<internal_types::Deserializer> _deserializer)
    : deserializer{std::move(_deserializer)}
{
    //    std::thread t([this]() { start(); });
    //    t.detach();
}

void MsgDispatcher::start()
{
    //    while (true)
    //        ioContext.run();
}

void MsgDispatcher::handleReceivedData(const internal_types::Buffer& _buffer)
{
    std::cout << "handleReceivedData 11 " << gettid() << std::endl;
    auto decoded = deserializer->decode(_buffer);
    std::cout << "handleReceivedData 22 " << gettid() << std::endl;
    if (decoded)
    {
        std::cout << "handleReceivedData 33 " << gettid() << std::endl;
        //        std::visit(
        //            internal_types::Visitor{
        //                [this](const auto& value) { handleReceivedType(value); },
        //            },
        //            decoded.value());
        std::visit(
            internal_types::Visitor{
                [this](const internal_types::Event& value) {
                    auto handler = static_cast<Handler<internal_types::Event>*>(secondHandler.get());
                    handler->run(value);
                },
                [this](const internal_types::ScreenResolution& value) {
                    auto handler = static_cast<Handler<internal_types::ScreenResolution>*>(firstHandler.get());
                    handler->run(value);
                },
            },
            decoded.value());
    }
    else
    {
        //        unsuccessfulCallback(boost::system::error_code());
    }
    std::cout << "end " << gettid() << std::endl;
}
} // namespace connection
