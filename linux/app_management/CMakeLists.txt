set(MODULE_NAME app_management)

set(HEADERS
    include/app_management/App.hpp
    include/app_management/Consumer.hpp
)

set(SOURCE_FILES
    src/App.cpp
)

add_library(app_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(app_management PUBLIC include src)

target_link_libraries(app_management
    internal_types
    connection
    event_consumer
    X11::X11
    commons
)

add_subdirectory(tests)
