cmake_minimum_required(VERSION 3.14.5)
set (CMAKE_CXX_STANDARD 17)
project(pcs_connector)

enable_testing()

get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

link_directories(${PREFIX_DIR}/google-test/googletest/build/lib)

find_package(Python3 COMPONENTS Interpreter)
find_package(X11 REQUIRED)

target_link_libraries(X11::X11 INTERFACE pthread)

add_subdirectory(app_management)
add_subdirectory(main_app)
add_subdirectory(event_consumer)
add_subdirectory(tests)
