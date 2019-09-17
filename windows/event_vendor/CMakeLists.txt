set(HEADERS
    include/event_vendor/KeyboardSender.hpp
    include/event_vendor/MouseSender.hpp
)

set(SOURCE_FILES
    src/KeyboardSender.cpp
    src/MouseSender.cpp
)

add_library(event_vendor ${HEADERS} ${SOURCE_FILES})
target_include_directories(event_vendor PUBLIC include PRIVATE src)

target_link_libraries(event_vendor
    internal_types
    connection
)
