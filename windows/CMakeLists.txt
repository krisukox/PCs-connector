get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

add_definitions(-DWIN32_LEAN_AND_MEAN=1) # in case of winsock implicit include

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH D:/Qt/5.14.1/msvc2017_64)
set(QT_QMAKE_EXECUTABLE D:/Qt/5.14.1/msvc2017_64/bin/qmake)
set(Qt5Core_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Core)
set(Qt5Gui_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Gui)
set(Qt5_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5)
set(Qt5Widgets_DIR D:/Qt/5.14.1/msvc2017_64/lib/cmake/Qt5Widgets)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(app_management)
add_subdirectory(event_vendor)
add_subdirectory(main_app)
add_subdirectory(gui)
