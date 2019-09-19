get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

set(Boost_ARCHITECTURE      "-x64")
set(Boost_COMPILER          "-vc141")
set(BOOST_ROOT              "${PREFIX_DIR}/boost_1_70_0/")
set(BOOST_INCLUDEDIR        "${PREFIX_DIR}/boost_1_70_0/")
set(BOOST_LIBRARYDIR        "${PREFIX_DIR}/boost_1_70_0/stage/lib/")
set(BOOST_MIN_VERSION       "1.70.0")
set(Boost_USE_STATIC_LIBS   ON)

add_definitions(-DWIN32_LEAN_AND_MEAN=1) # in case of winsock implicit include

find_package(Boost 1.70.0 COMPONENTS system date_time regex)

include_directories(boost-path ${PREFIX_DIR}/boost_1_70_0/)

add_subdirectory(app_management)
add_subdirectory(event_vendor)
add_subdirectory(connection)
add_subdirectory(main_app)
