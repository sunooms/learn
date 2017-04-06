#include "sample2.h"
#include <gtest/gtest.h>

// test the default c'tor
TEST(MyString, DefaultConstructor)
{
    const MyString s;

    EXPECT_STREQ(NULL, s.c_string());
    EXPECT_EQ(0u, s.Length());
}

const char kHelloString[] = "Hello, world!";

// tests the c'tor that accepts a c string
TEST(MyString, ConstructorFromCString)
{
    const MyString s(kHelloString);

    EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));
    EXPECT_EQ(sizeof(kHelloString)/sizeof(kHelloString[0]) - 1, s.Length());
}

// test the copy c'tor
TEST(MyString, copyConstructor)
{
    const MyString s1(kHelloString);
    const MyString s2 = s1;

    EXPECT_EQ(0, strcmp(s2.c_string(), kHelloString));
}

// test the set method
TEST(MyString, Set)
{
    MyString s;

    s.Set(kHelloString);
    EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

    // set should work when the input pointer is the same as the one
    // already in the MyString object
    s.Set(s.c_string());
    EXPECT_EQ(0, strcmp(s.c_string(), kHelloString));

    // can wi set MyString to NULL
    s.Set(NULL);
    EXPECT_STREQ(NULL, s.c_string());
}
