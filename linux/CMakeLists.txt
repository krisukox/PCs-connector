cmake_minimum_required(VERSION 2.8)

project(pcs_connector)

find_package(X11 REQUIRED)

set(BOOST_ROOT "/home/krisukox/project-cpp/boost_1_70_0/")
set(BOOST_LIBRARYDIR "/home/krisukox/project-cpp/boost_1_70_0/stage/lib/")

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

add_executable(${PROJECT_NAME} src/FakeKey.cpp
                               src/Server.cpp
                               src/ServerSession.cpp
                               include/FakeKey.hpp
                               include/Server.hpp
                               include/ServerSession.hpp
                               main.cpp)


target_link_libraries (${PROJECT_NAME} X11)
target_link_libraries (${PROJECT_NAME} Xtst)
target_link_libraries (${PROJECT_NAME} boost_system)
target_link_libraries (${PROJECT_NAME} pthread)

INCLUDE_DIRECTORIES(boost-path ../../boost_1_70_0/)


add_custom_target(build_run
                  COMMAND make
                  COMMAND pcs_connector
                  DEPENDS build
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
