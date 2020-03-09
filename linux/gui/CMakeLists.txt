set(HEADERS
    include/gui/App.hpp
    include/gui/Consumer.hpp
    include/gui/MainWindow.h
    include/gui/GraphicsRectItem.h
    include/gui/GraphicsScene.h
)

set(SOURCE_FILES
    src/App.cpp
    src/Consumer.cpp
    src/MainWindow.cpp
    src/GraphicsRectItem.cpp
    src/GraphicsScene.cpp
)

add_library(gui ${HEADERS} ${SOURCE_FILES})
target_include_directories(gui PUBLIC include PRIVATE src)

target_link_libraries(gui
    internal_types
    connection
    event_consumer
    X11::X11
    Qt5::Widgets
    Qt5::X11Extras
)
