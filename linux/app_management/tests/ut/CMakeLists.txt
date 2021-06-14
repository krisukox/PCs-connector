add_executable(app_management_ut
    ConsumerTests.cpp
)

target_include_directories(app_management_ut
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googletest/include/
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)

target_link_libraries(app_management_ut
    event_consumer_mocks
    connection_mocks
    app_management
    gmock
    gtest
    gtest_main
)

add_custom_target(UT
    DEPENDS app_management_ut
    COMMAND app_management_ut
    USES_TERMINAL
)
