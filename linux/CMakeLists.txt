cmake_minimum_required(VERSION 3.14.5)
set (CMAKE_CXX_STANDARD 17)
project(pcs_connector)

enable_testing()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH /home/krisukox/Qt/5.14.1/gcc_64)
set(QT_QMAKE_EXECUTABLE /home/krisukox/Qt/5.14.1/gcc_64/bin/qmake)
set(Qt5Core_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Core)
set(Qt5Gui_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Gui)
set(Qt5Widgets_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets)
set(Qt5_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

link_directories(${PREFIX_DIR}/google-test/googletest/build/lib)

find_package(Python3 COMPONENTS Interpreter)
find_package(X11 REQUIRED)

target_link_libraries(X11::X11 INTERFACE pthread)

add_subdirectory(app_management)
add_subdirectory(main_app)
add_subdirectory(event_consumer)
add_subdirectory(gui)
add_subdirectory(tests)
