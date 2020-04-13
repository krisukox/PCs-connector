set(HEADERS
    include/connection/Socket.hpp
)

set(SOURCE_FILES
    src/Socket.cpp
)

add_library(connection ${HEADERS} ${SOURCE_FILES})
target_include_directories(connection PUBLIC include PRIVATE src)

target_link_libraries(connection
    Boost::system
    internal_types
)

add_subdirectory(tests)
