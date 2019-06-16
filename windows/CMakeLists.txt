cmake_minimum_required(VERSION 3.7)

project(pcs_connector)

#find_package(Qt5Core)
#find_package(Qt5Core CONFIG REQUIRED)
set (Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_ROOT "D:/pcs-connector/boost_1_62_0/")
set(BOOST_INCLUDEDIR     "D:/pcs-connector/boost_1_62_0/")
set(BOOST_LIBRARYDIR "D:/pcs-connector/boost_1_62_0/stage/lib/")
set(Boost_ADDITIONAL_VERSIONS "1.62" "1.62.0")
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.62.0 COMPONENTS regex system date_time)
if(Boost_FOUND)
    MESSAGE( STATUS ">>>>>>>>>>>>" )
endif(Boost_FOUND)


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

#target_link_libraries(
#    ${PROJECT_NAME}
#    LINK_PUBLIC
#    ${Boost_LIBRARIES} )

#target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui)

#get_target_property(QtCore_location Qt5::Core LOCATION)

