set(HEADERS include/key_management/KeyMock.hpp)

add_library(key_management_mocks ${HEADERS})
set_target_properties(key_management_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(key_management_mocks
    key_management
    gmock
)

target_include_directories(key_management_mocks
    PUBLIC
        include
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
