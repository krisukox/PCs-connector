set(HEADERS
    include/event_consumer/IKeyboardReceiver.hpp
    include/event_consumer/IMouseReceiver.hpp
    include/event_consumer/KeyboardReceiver.hpp
    include/event_consumer/MouseReceiver.hpp
    include/event_consumer/TestKeyboardReceiver.hpp
)

set(SOURCE_FILES
    src/KeyboardReceiver.cpp
    src/MouseReceiver.cpp
    src/TestKeyboardReceiver.cpp
)

add_library(event_consumer ${HEADERS} ${SOURCE_FILES})
target_include_directories(event_consumer PUBLIC include PRIVATE src)

target_link_libraries(event_consumer
    Xtst
    internal_types
    connection
)

add_subdirectory(tests)
