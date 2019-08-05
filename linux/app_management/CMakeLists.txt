set(MODULE_NAME app_management)

set(HEADERS include/app_management/App.hpp)

set(SOURCE_FILES src/App.cpp)

add_library(app_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(app_management PUBLIC include PRIVATE src)

target_link_libraries(app_management server_app key_management)
