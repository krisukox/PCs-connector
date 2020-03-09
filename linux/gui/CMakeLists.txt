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

add_library(gui ${HEADERS} ${SOURCE_FILES})
target_include_directories(gui PUBLIC include PRIVATE src)

target_link_libraries(gui
    Qt5::Widgets
    app_management
)
