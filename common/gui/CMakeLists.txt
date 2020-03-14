set(HEADERS
    include/gui/MainWindow.h
    include/gui/GraphicsRectItem.h
    include/gui/GraphicsScene.h
)

set(SOURCE_FILES
    src/MainWindow.cpp
    src/GraphicsRectItem.cpp
    src/GraphicsScene.cpp
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH D:/Qt/5.14.1/msvc2017_64)
set(QT_QMAKE_EXECUTABLE D:/Qt/5.14.1/msvc2017_64/bin/qmake)
set(Qt5Core_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Core)
set(Qt5Gui_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Gui)
set(Qt5_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5)
set(Qt5Widgets_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Widgets)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(gui ${HEADERS} ${SOURCE_FILES})
target_include_directories(gui PUBLIC include PRIVATE src)

target_link_libraries(gui
    Qt5::Widgets
    app_management
)
