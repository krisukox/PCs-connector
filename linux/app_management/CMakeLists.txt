set(MODULE_NAME app_management)

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

set(HEADERS
    include/app_management/App.hpp
    include/app_management/Consumer.hpp
#    include/app_management/MainWindow.h
#    include/app_management/MyGraphicsRectItem.h
#    include/app_management/MyGraphicsScene.h
)

set(SOURCE_FILES
    src/App.cpp
    src/Consumer.cpp
#    src/MainWindow.cpp
#    src/MyGraphicsRectItem.cpp
#    src/MyGraphicsScene.cpp
)


add_library(app_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(app_management PUBLIC include PRIVATE src)

target_link_libraries(app_management
    internal_types
    connection
    event_consumer
    X11::X11
    Qt5::Widgets
)

add_subdirectory(tests)
