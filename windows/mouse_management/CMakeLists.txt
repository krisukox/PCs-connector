set(HEADERS
    include/mouse_management/MouseHandler.hpp
    #include/mouse_management/IMouseHandler.hpp
)

set(SOURCE_FILES
    src/MouseHandler.cpp
)

add_library(mouse_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(mouse_management PUBLIC include PRIVATE src)

target_link_libraries(mouse_management
    internal_types
)

#add_subdirectory(tests)
