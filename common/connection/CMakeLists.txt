set(HEADERS
    include/connection/IReceiver.hpp
    include/connection/Receiver.hpp
    include/connection/Socket.hpp
    include/connection/Sender.hpp
    include/connection/MsgDispatcher.hpp
)

set(SOURCE_FILES
    src/Receiver.cpp
    src/Socket.cpp
    src/Sender.cpp
    src/MsgDispatcher.cpp
)

add_library(connection ${HEADERS} ${SOURCE_FILES})
target_include_directories(connection PUBLIC include PRIVATE src)

target_link_libraries(connection
    Boost::system
    internal_types
    pthread
)

add_subdirectory(tests)
