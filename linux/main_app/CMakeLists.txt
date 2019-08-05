add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ../../google-test/googletest/googletest/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ../../boost_1_70_0/)

target_link_libraries (${PROJECT_NAME} app_management)
