set(HEADERS include/mouse_management/MouseMock.hpp)

add_library(mouse_management_mocks ${HEADERS})
set_target_properties(mouse_management_mocks PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(mouse_management_mocks
    mouse_management
    gmock
)

target_include_directories(key_management_mocks
    PUBLIC
        include
    PRIVATE
        ${PREFIX_DIR}/google-test/googletest/googlemock/include/
)
