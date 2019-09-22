set(HEADERS include/server_app/ReceiverMock.hpp)

add_library(server_app_mocks ${HEADERS})
set_target_properties(server_app_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(server_app_mocks
    gmock
    Boost::system
    server_app
)

target_include_directories(server_app_mocks
    PUBLIC
        include
    PUBLIC
        ${PREFIX_DIR}/google-test/googletest/googletest/include/
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
