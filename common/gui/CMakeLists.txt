set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    include/gui/CursorManagement.h
    include/gui/MainWindow.h
    include/gui/GraphicsRectItem.h
    include/gui/GraphicsScene.h
    include/gui/ScreenResolutionMsg.h
    include/gui/ScreenIndicator.h
)

set(SOURCE_FILES
    src/CursorManagement.cpp
    src/MainWindow.cpp
    src/ScreenIndicator.cpp
    src/GraphicsRectItem.cpp
    src/GraphicsScene.cpp
    src/ScreenResolutionMsg.cpp
)

if(UNIX)
    set(CMAKE_PREFIX_PATH ${PREFIX_DIR}/Qt/5.14.1/gcc_64)
    set(QT_QMAKE_EXECUTABLE ${PREFIX_DIR}/Qt/5.14.1/gcc_64/bin/qmake)
    set(Qt5Core_DIR ${PREFIX_DIR}/Qt/5.14.1/gcc_64/lib/cmake/Qt5Core)
    set(Qt5Gui_DIR ${PREFIX_DIR}/Qt/5.14.1/gcc_64/lib/cmake/Qt5Gui)
    set(Qt5Widgets_DIR ${PREFIX_DIR}/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets)
    set(Qt5_DIR ${PREFIX_DIR}/Qt/5.14.1/gcc_64/lib/cmake/Qt5)
endif(UNIX)


if(WIN32)
    set(CMAKE_PREFIX_PATH ${PREFIX_DIR})
    if(DEFINED ENV{QTDIR})
        set(QTDIR $ENV{QTDIR})
        set(QT_QMAKE_EXECUTABLE ${QTDIR}/bin/qmake)
        set(Qt5Core_DIR ${QTDIR}/lib/cmake/Qt5Core)
        set(Qt5Gui_DIR ${QTDIR}/lib/cmake/Qt5Gui)
        set(Qt5_DIR ${QTDIR}/lib/cmake/Qt5)
        set(Qt5Widgets_DIR ${QTDIR}/lib/cmake/Qt5Widgets)
    else()
        set(QT_QMAKE_EXECUTABLE ${PREFIX_DIR}/Qt/5.14.1/msvc2017_64/bin/qmake)
        set(Qt5Core_DIR ${PREFIX_DIR}/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Core)
        set(Qt5Gui_DIR ${PREFIX_DIR}/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Gui)
        set(Qt5_DIR ${PREFIX_DIR}/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5)
        set(Qt5Widgets_DIR ${PREFIX_DIR}/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Widgets)
    endif()
endif(WIN32)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(gui ${HEADERS} ${SOURCE_FILES})
target_include_directories(gui PUBLIC include PRIVATE src)

target_link_libraries(gui
    Qt5::Widgets
    app_management
)
