cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 17)
enable_testing()

project(pcs_connector)

find_package(X11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter)

set(BOOST_ROOT "/home/krisukox/project-cpp/boost_1_70_0/")
set(BOOST_LIBRARYDIR "/home/krisukox/project-cpp/boost_1_70_0/stage/lib/")
#set(Gtest_DIR "/home/krisukox/project-cpp/google-test/googletest/build/lib/")

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.70.0 COMPONENTS system REQUIRED)

find_path(XTEST_INCLUDE_DIR NAMES X11/extensions/XTest.h
          PATH_SUFFIXES X11/extensions
          PATHS /usr/include/X11
          DOC "The XTest include directory"
)

find_library(XTEST_LIBRARY NAMES Xtst
             PATHS /usr/lib/X11
             DOC "The XTest library"
)

add_executable(${PROJECT_NAME}
#                               src/FakeKey.cpp
                               src/Server.cpp
                               src/ServerSession.cpp
#                               src/TestKey.cpp
#                               include/FakeKey.hpp
#                               include/IKey.hpp
                               include/Server.hpp
                               include/ServerSession.hpp
#                               include/TestKey.hpp
                               main.cpp)



target_link_libraries (${PROJECT_NAME} X11)
target_link_libraries (${PROJECT_NAME} Xtst)
target_link_libraries (${PROJECT_NAME} boost_system)
target_link_libraries (${PROJECT_NAME} pthread)

INCLUDE_DIRECTORIES(boost-path ../../boost_1_70_0/)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ../../google-test/googletest/googletest/include/)

add_subdirectory(key_management)
add_subdirectory(tests)
target_link_libraries (${PROJECT_NAME} key_management)

add_custom_target(build_run
                  COMMAND make
                  COMMAND pcs_connector
                  DEPENDS build
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
