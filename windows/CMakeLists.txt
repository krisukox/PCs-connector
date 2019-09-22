get_filename_component(PREFIX_DIR "../.." ABSOLUTE)

add_definitions(-DWIN32_LEAN_AND_MEAN=1) # in case of winsock implicit include

add_subdirectory(app_management)
add_subdirectory(event_vendor)
add_subdirectory(main_app)
