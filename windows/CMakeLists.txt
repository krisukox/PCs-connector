cmake_minimum_required(VERSION 3.7)

project(pcs_connector)

set(BOOST_INCLUDEDIR "D:/pcs-connector/boost_1_62_0/")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.62.0 COMPONENTS regex system date_time)

add_executable(${PROJECT_NAME} main.cpp
                               src/Client.cpp
                               src/Console.cpp
                               src/Keyboard.cpp
                               include/Client.hpp
                               include/Console.hpp
                               include/Keyboard.hpp)

add_library(include-path INTERFACE)

target_include_directories(include-path INTERFACE include/)
include_directories(boost-path ../../boost_1_62_0/)

target_link_libraries(${PROJECT_NAME} include-path ${Boost_LIBRARIES})
