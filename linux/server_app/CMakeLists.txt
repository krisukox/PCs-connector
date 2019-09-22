set(MODULE_NAME server_app)

set(HEADERS
        include/server_app/ConcreteReceiver.hpp
        include/server_app/IReceiver.hpp
	include/server_app/Server.hpp
        include/server_app/ServerAppTypes.hpp
	include/server_app/ServerSession.hpp
)

set(SOURCE_FILES
        src/ConcreteReceiver.cpp
	src/Server.cpp
	src/ServerSession.cpp
)

add_library(server_app ${HEADERS} ${SOURCE_FILES})
target_include_directories(server_app PUBLIC include PRIVATE src)

target_link_libraries(server_app
    key_management
    mouse_management
    pthread
    Boost::system
    internal_types
)

add_subdirectory(tests)
