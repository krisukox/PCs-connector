cmake_minimum_required(VERSION 3.14.5)
set (CMAKE_CXX_STANDARD 17)

project(pcs_connector)


#find_package(Qt5Core)
#find_package(Qt5Core CONFIG REQUIRED)
set(Boost_ARCHITECTURE "-x64")
set(Boost_COMPILER "-vc141")
set (Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_ROOT "D:/pcs-connector/boost_1_70_0/")
set(BOOST_INCLUDEDIR     "D:/pcs-connector/boost_1_70_0/")
set(BOOST_LIBRARYDIR "D:/pcs-connector/boost_1_70_0/stage/lib/")
SET (BOOST_MIN_VERSION "1.70.0")
set (Boost_NO_BOOST_CMAKE ON)
#set(Boost_ADDITIONAL_VERSIONS "1.70" "1.70.0")
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.70.0 COMPONENTS system date_time regex)
if(Boost_FOUND)
    MESSAGE( STATUS ">>>>>>>>>>>>" )
endif(Boost_FOUND)


add_executable(${PROJECT_NAME} main.cpp
                               src/Client.cpp
                               src/Console.cpp
                               src/Keyboard.cpp
                               include/Client.hpp
                               include/Console.hpp
                               include/Keyboard.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/)

include_directories(boost-path ../../boost_1_70_0/)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})



