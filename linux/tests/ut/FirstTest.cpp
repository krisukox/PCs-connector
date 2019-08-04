#include <gtest/gtest.h> // googletest header file

#include <string>
using std::string;
namespace
{
const char* actualValTrue = "hello gtest";
// const char* actualValFalse = "hello world";
const char* expectVal = "hello gtest";
} // namespace

TEST(StrCompare, CStrEqual)
{
    EXPECT_STREQ(expectVal, actualValTrue);
}

TEST(StrCompare, CStrEqualSub)
{
    EXPECT_STREQ(expectVal, actualValTrue);
}

// TEST(StrCompare, CStrNotEqual)
//{
//    EXPECT_STREQ(expectVal, actualValFalse);
//}
