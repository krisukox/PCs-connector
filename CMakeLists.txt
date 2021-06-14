cmake_minimum_required(VERSION 3.14.5)
set (CMAKE_CXX_STANDARD 17)
project(pcs_connector)

if(DEFINED ENV{PREFIX_DIR})
    set(PREFIX_DIR $ENV{PREFIX_DIR})
else()
    get_filename_component(PREFIX_DIR ".." ABSOLUTE)
endif()

set(BOOST_ROOT ${PREFIX_DIR}/boost_1_70_0/)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.70.0 COMPONENTS system REQUIRED)

if(UNIX)
    add_subdirectory(linux)
endif(UNIX)

if(WIN32)
    target_link_directories(Boost::system INTERFACE ${PREFIX_DIR}/boost_1_70_0/stage/lib)

    add_subdirectory(windows)
endif(WIN32)

add_custom_target(build_run
    DEPENDS pcs_connector
    COMMAND pcs_connector
    USES_TERMINAL
)

add_subdirectory(common)
