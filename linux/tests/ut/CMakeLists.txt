add_executable(unitTests GtestMain.cpp
                         FirstTest.cpp)

set(GTEST_DIR "/home/krisukox/project-cpp/google-test/googletest/build/lib/")
set(GTEST_ROOT "/home/krisukox/project-cpp/google-test/googletest/build/")
set(GTEST_INCLUDE_DIR "/home/krisukox/project-cpp/google-test/googletest/googletest/include/")

find_package(GTest)
target_link_libraries(unitTests PRIVATE GTest::GTest)
