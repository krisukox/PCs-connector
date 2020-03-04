set(MODULE_NAME app_management)

set(HEADERS
    include/app_management/App.hpp
    include/app_management/Consumer.hpp
    include/app_management/MainWindow.h
    include/app_management/GraphicsRectItem.h
    include/app_management/GraphicsScene.h
)

set(SOURCE_FILES
    src/App.cpp
    src/Consumer.cpp
    src/MainWindow.cpp
    src/GraphicsRectItem.cpp
    src/GraphicsScene.cpp
)

add_library(app_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(app_management PUBLIC include PRIVATE src)

target_link_libraries(app_management
    internal_types
    connection
    event_consumer
    X11::X11
    Qt5::Widgets
    Qt5::X11Extras
)

add_subdirectory(tests)
