set(HEADERS
	include/event_consumer/KeyboardReceiverMock.hpp
	include/event_consumer/MouseReceiverMock.hpp
)

add_library(event_consumer_mocks ${HEADERS})
set_target_properties(event_consumer_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(event_consumer_mocks
    event_consumer
    gmock
)

target_include_directories(event_consumer_mocks
    PUBLIC
        include
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
