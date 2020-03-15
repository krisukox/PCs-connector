set(HEADERS
    include/app_management/App.hpp
    include/app_management/Console.hpp
    include/app_management/Vendor.hpp
)

set(SOURCE_FILES
    src/App.cpp
    src/Console.cpp
    src/Vendor.cpp
)

add_library(app_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(app_management PUBLIC include PRIVATE src)

target_link_libraries(app_management
    Boost::system
    internal_types
    event_vendor
    connection
    commons
)
