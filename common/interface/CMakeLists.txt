add_library(interface INTERFACE)
add_custom_target(interface_headers SOURCES include/interface/IApp.hpp)

target_include_directories(interface INTERFACE include)
target_link_libraries(interface INTERFACE internal_types)

