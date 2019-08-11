add_executable(server_app_ut
    ServerAppTests.cpp
)

target_include_directories(server_app_ut
    PRIVATE
        /home/krisukox/project-cpp/google-test/googletest/googletest/include/
        /home/krisukox/project-cpp/google-test/googletest/googlemock/include/
)

target_link_libraries(server_app_ut
    key_management_mocks
    server_app
    gmock
    gmock_main
    gtest
    gtest_main
)

