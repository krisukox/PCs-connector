set(MODULE_NAME mouse_management)

set(HEADERS
    include/mouse_management/FakeMouse.hpp
    include/mouse_management/IMouse.hpp
)

set(SOURCE_FILES
    src/FakeMouse.cpp
)

add_library(mouse_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(mouse_management PUBLIC include PRIVATE src)

target_link_libraries(mouse_management
    X11
    Xtst
    internal_types
)

add_subdirectory(tests)
