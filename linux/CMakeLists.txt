cmake_minimum_required(VERSION 2.8)

#add_library(MathFunctions /usr/include/X11/Xlib.h)

#find_package(Xlib REQUIRED)
#find_package(Xlib config required)
#FIND_LIBRARY(APUE_LIB apue)

#ADDITIONAL_MAKE_CLEAN_FILES
project(pcs_connector)

#add_library(LibsModule
#            src/nowy.cpp)

find_package(X11 REQUIRED)
#include_directories( /usr/include/X11/extensions )
#file (GLOB heade /usr/include/X11/extensions/*.h )

#find_package(CxxTest REQUIRED)

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
                               include/FakeKey.hpp
                               src/Server.cpp
                               include/Server.hpp
                               main.cpp)


target_link_libraries (${PROJECT_NAME} X11)
#target_link_libraries (${PROJECT_NAME} Threads)
target_link_libraries (${PROJECT_NAME} Xtst)



add_custom_target(build_run
                  COMMAND make
                  COMMAND pcs_connector
                  DEPENDS build
                  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
