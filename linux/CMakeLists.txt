cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 17)
project(pcs_connector)

enable_testing()

get_filename_component(PREFIX_DIR "../.." ABSOLUTE)
message(${PREFIX_DIR})

link_directories(${PREFIX_DIR}/google-test/googletest/build/lib)

set(BOOST_ROOT "${PREFIX_DIR}/boost_1_70_0/")

find_package(Boost 1.70.0 COMPONENTS system REQUIRED)
find_package(Python3 COMPONENTS Interpreter)
find_package(X11 REQUIRED)

add_subdirectory(app_management)
add_subdirectory(main_app)
add_subdirectory(key_management)
add_subdirectory(server_app)
add_subdirectory(tests)

add_custom_target(build_run
                  COMMAND $(MAKE)
                  COMMAND pcs_connector
                  DEPENDS build
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
