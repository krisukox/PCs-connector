
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_PREFIX_PATH /home/krisukox/Qt/5.14.1/gcc_64)
#set(QT_QMAKE_EXECUTABLE /home/krisukox/Qt/5.14.1/gcc_64/bin/qmake)
#set(Qt5Core_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Core)
#set(Qt5Gui_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Gui)
#set(Qt5Widgets_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets)
#set(Qt5_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5)

#find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries (${PROJECT_NAME} app_management Qt5::Widgets)
