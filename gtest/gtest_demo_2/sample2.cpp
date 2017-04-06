#include "sample2.h"
#include <string.h>

// clone a 0-terminated c string, allocating memory using new.
const char* MyString::CloneCString(const char* a_c_string)
{
    if(a_c_string == NULL)
        return NULL;

    const size_t len = strlen(a_c_string);
    char *const clone = new char[len + 1];
    memcpy(clone, a_c_string, len+1);

    return clone;
}

// set the 0-terminated c string this MyString object represents
void MyString::Set(const char *a_c_string)
{
    const char * const temp = MyString::CloneCString(a_c_string);
    delete[] c_string_;

    c_string_ = temp;
}
