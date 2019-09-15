cmake_minimum_required(VERSION 3.14.5)
set (CMAKE_CXX_STANDARD 17)
project(pcs_connector)

if(UNIX)
    add_subdirectory(linux)
endif(UNIX)

if(WIN32)
    add_subdirectory(windows)
endif(WIN32)

add_custom_target(build_run
    DEPENDS pcs_connector
    COMMAND pcs_connector
    USES_TERMINAL
)

add_subdirectory(common)
