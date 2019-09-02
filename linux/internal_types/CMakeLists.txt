set(MODULE_NAME internal_type)

set(HEADERS
    include/internal_types/Deserializer.hpp
    include/internal_types/IDeserializer.hpp
    include/internal_types/KeyEvent.hpp
    include/internal_types/MouseEvent.hpp
)

set(SOURCE_FILES
    src/Deserializer.cpp
)

add_library(internal_types ${HEADERS} ${SOURCE_FILES})
target_include_directories(internal_types PUBLIC include PRIVATE src)

target_link_libraries(internal_types
    X11
    Xtst
    server_app
)

add_subdirectory(tests)
