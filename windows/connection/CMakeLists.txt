set(HEADERS
    include/connection/Socket.hpp
    include/connection/Sender.hpp
)

set(SOURCE_FILES
    src/Socket.cpp
    src/Sender.cpp
)

add_library(connection ${HEADERS} ${SOURCE_FILES})
target_include_directories(connection PUBLIC include PRIVATE src)

target_link_libraries(connection
    internal_types
)
