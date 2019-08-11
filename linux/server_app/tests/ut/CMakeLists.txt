add_executable(server_app_ut
    ServerAppTests.cpp
)

target_include_directories(server_app_ut
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googletest/include/
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)

target_link_libraries(server_app_ut
    key_management_mocks
    server_app
    gmock
    gtest
    gtest_main
)

