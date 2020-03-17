set(HEADERS
    include/commons/IApp.hpp
    include/commons/CursorGuard.hpp
)

set(SOURCE_FILES
    src/IApp.cpp
    src/CursorGuard.cpp
)

add_library(commons ${HEADERS} ${SOURCE_FILES})

target_include_directories(commons PUBLIC include PRIVATE src)
target_link_libraries(commons
    internal_types
    Boost::system
)

