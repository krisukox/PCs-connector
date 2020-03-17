set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(UNIX)
    set(HEADERS
        include/gui/MainWindowLin.h
        include/gui/GraphicsRectItem.h
        include/gui/GraphicsScene.h
    )

    set(SOURCE_FILES
        src/MainWindowLin.cpp
        src/GraphicsRectItem.cpp
        src/GraphicsScene.cpp
    )

    set(CMAKE_PREFIX_PATH /home/krisukox/Qt/5.14.1/gcc_64)
    set(QT_QMAKE_EXECUTABLE /home/krisukox/Qt/5.14.1/gcc_64/bin/qmake)
    set(Qt5Core_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Core)
    set(Qt5Gui_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Gui)
    set(Qt5Widgets_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets)
    set(Qt5_DIR /home/krisukox/Qt/5.14.1/gcc_64/lib/cmake/Qt5)

    find_package(Qt5 COMPONENTS Widgets REQUIRED)

#    qt5_wrap_ui(costam src/MainWindowLin.ui)
endif(UNIX)


if(WIN32)
    set(HEADERS
        include/gui/MainWindowWin.h
        include/gui/GraphicsRectItem.h
        include/gui/GraphicsScene.h
    )

    set(SOURCE_FILES
        src/MainWindowWin.cpp
        src/GraphicsRectItem.cpp
        src/GraphicsScene.cpp
    )


    set(CMAKE_PREFIX_PATH D:/Qt/5.14.1/msvc2017_64)
    set(QT_QMAKE_EXECUTABLE D:/Qt/5.14.1/msvc2017_64/bin/qmake)
    set(Qt5Core_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Core)
    set(Qt5Gui_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Gui)
    set(Qt5_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5)
    set(Qt5Widgets_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Widgets)

    find_package(Qt5 COMPONENTS Widgets REQUIRED)

#    qt5_wrap_ui(costam src/MainWindowWin.ui)
endif(WIN32)

add_library(gui ${HEADERS} ${SOURCE_FILES} ${costam})
target_include_directories(gui PUBLIC include PRIVATE src)

target_link_libraries(gui
    Qt5::Widgets
    app_management
)
