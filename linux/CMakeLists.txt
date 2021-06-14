enable_testing()

#get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

link_directories(${PREFIX_DIR}/googletest/build/lib)

find_package(Python3 COMPONENTS Interpreter)
find_package(X11 REQUIRED)

target_link_libraries(X11::X11 INTERFACE pthread)

add_subdirectory(app_management)
add_subdirectory(main_app)
add_subdirectory(event_consumer)
add_subdirectory(tests)
