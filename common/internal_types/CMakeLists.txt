set(MODULE_NAME internal_type)

if(UNIX)
    set(HEADERS
        include/internal_types/CommonTypes.hpp
        include/internal_types/linux/Deserializer.hpp
        include/internal_types/IDeserializer.hpp
        include/internal_types/KeyEvent.hpp
        include/internal_types/MouseEvent.hpp
        include/internal_types/Point.hpp
        include/internal_types/Serializer.hpp
        include/internal_types/Visitor.hpp
        include/internal_types/SerializedValues.hpp
    )

    set(SOURCE_FILES
        src/Deserializer.cpp
        src/Serializer.cpp
    )

    add_library(internal_types ${HEADERS} ${SOURCE_FILES})
    target_include_directories(internal_types PUBLIC include include/internal_types/linux PRIVATE src)
endif(UNIX)

if(WIN32)
    set(HEADERS
        include/internal_types/CommonTypes.hpp
        include/internal_types/windows/Deserializer.hpp
        include/internal_types/ISerializer.hpp
        include/internal_types/IDeserializer.hpp
        include/internal_types/KeyEvent.hpp
        include/internal_types/MouseEvent.hpp
        include/internal_types/Point.hpp
        include/internal_types/Serializer.hpp
        include/internal_types/Visitor.hpp
        include/internal_types/ScreenResolution.hpp
        include/internal_types/SerializedValues.hpp
    )

    set(SOURCE_FILES
        src/windows/Deserializer.cpp
        src/Serializer.cpp
    )

    add_library(internal_types ${HEADERS} ${SOURCE_FILES})
    target_include_directories(internal_types PUBLIC include include/internal_types/windows PRIVATE src src/windows)
endif(WIN32)

#add_library(internal_types ${HEADERS} ${SOURCE_FILES})
#target_include_directories(internal_types PUBLIC include PRIVATE src)

add_subdirectory(tests)
