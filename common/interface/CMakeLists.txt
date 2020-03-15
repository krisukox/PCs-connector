set(HEADERS
    include/interface/IApp.hpp
    include/interface/CursorGuard.hpp
)

set(SOURCE_FILES
    src/IApp.cpp
    src/CursorGuard.cpp
)

add_library(interface ${HEADERS} ${SOURCE_FILES})

target_include_directories(interface PUBLIC include PRIVATE src)
target_link_libraries(interface internal_types)

