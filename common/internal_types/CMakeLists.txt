set(MODULE_NAME internal_type)

set(HEADERS
    include/internal_types/Deserializer.hpp
    include/internal_types/IDeserializer.hpp
    include/internal_types/KeyEvent.hpp
    include/internal_types/MouseEvent.hpp
    include/internal_types/Visitor.hpp
)

set(SOURCE_FILES
    src/Deserializer.cpp
)

add_library(internal_types ${HEADERS} ${SOURCE_FILES})
target_include_directories(internal_types PUBLIC include PRIVATE src)

target_link_libraries(internal_types
    server_app
)

add_subdirectory(tests)
