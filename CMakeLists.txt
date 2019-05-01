cmake_minimum_required(VERSION 3.6)

project(pcs_connector)

#find_package(Qt5Core)
#find_package(Qt5Core CONFIG REQUIRED)
#find_package(Qt5Gui CONFIG REQUIRED)


add_executable(${PROJECT_NAME} main.cpp
                               src/Client.cpp
                               src/Console.cpp
                               src/Keyboard.cpp
                               include/Client.hpp
                               include/Console.hpp
                               include/Keyboard.hpp)

add_library(include-path INTERFACE)

target_include_directories(include-path INTERFACE include/)

target_link_libraries(${PROJECT_NAME} include-path)

#target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui)

#get_target_property(QtCore_location Qt5::Core LOCATION)

