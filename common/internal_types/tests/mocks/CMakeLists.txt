set(HEADERS include/internal_types/DeserializerMock.hpp)

add_library(internal_types_mocks ${HEADERS})
set_target_properties(internal_types_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(internal_types_mocks
    internal_types
    gmock
)

target_include_directories(internal_types_mocks
    PUBLIC
        include
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
