#ifndef GTEST_SAMPLES2_H_
#define GTEST_SAMPLES2_H_

#include <string.h>

// a simple string class
class MyString
{
    private:
        const char* c_string_;
        const MyString& operator=(const MyString& rhs);

    public:
        // clones a 0-terminated c string, allocating memory using new.
        static const char* CloneCString(const char* a_c_string);

        // c'tors
        // the default c'tor constructs a NULL string
        MyString():c_string_(NULL){}

        // constructs a mystring by coneing a 0-ternimated c string
        explicit MyString(const char* a_c_string):c_string_(NULL){
            Set(a_c_string);
        }

        // copy c'tor
        MyString(const MyString& string):c_string_(NULL){
            Set(string.c_string_);
        }

        // d'tor. MyString is intended to be a final class. so the d'tor
        //  doesn't need to be virtual
        ~MyString(){delete[] c_string_;}

        // gets the 0-terminated c string this MyString object represents.
        const char* c_string() const{return c_string_;};

        size_t Length() const{
            return c_string_ == NULL ? 0 : strlen(c_string_);
        }

        // set the 0-terminated c string this MyString object represents
        void Set(const char* c_string);
};

#endif
