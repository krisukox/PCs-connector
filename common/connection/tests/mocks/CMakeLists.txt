set(HEADERS include/connection/SocketMock.hpp)

add_library(connection_mocks ${HEADERS})
set_target_properties(connection_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(connection_mocks
    gmock
    Boost::system
    connection
)

target_include_directories(connection_mocks
    PUBLIC
        include
    PUBLIC
        ${PREFIX_DIR}/google-test/googletest/googletest/include/
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
