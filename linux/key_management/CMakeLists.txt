set(MODULE_NAME key_management)

set(HEADERS
	include/key_management/FakeKey.hpp
	include/key_management/IKey.hpp
	include/key_management/TestKey.hpp
)

set(SOURCE_FILES
	src/FakeKey.cpp
	src/TestKey.cpp
)

add_library(key_management ${HEADERS} ${SOURCE_FILES})
target_include_directories(key_management PUBLIC include PRIVATE src)


