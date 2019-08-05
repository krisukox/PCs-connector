find_package(Python3 COMPONENTS Interpreter)

set(INTEGR_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_custom_target(integr-tests
    COMMAND Python3::Interpreter -m pytest
    DEPENDS pcs_connector
    WORKING_DIRECTORY ${INTEGR_TEST_DIRECTORY}
)
