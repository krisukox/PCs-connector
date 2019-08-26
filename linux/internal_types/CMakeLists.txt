set(MODULE_NAME internal_type)

set(HEADERS
    include/internal_types/KeyEvent.hpp
    include/internal_types/MouseEvent.hpp
)

set(SOURCE_FILES
)

add_custom_target(internal_types_headers SOURCES ${HEADERS})

add_library(internal_types INTERFACE)
target_include_directories(internal_types INTERFACE include)
