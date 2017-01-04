#ifndef __PACKAGE_MESSAGE_H__
#define __PACKAGE_MESSAGE_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "dynamic_message_protocol.h"

class package_message
{
    public:
        enum  {header_length = sizeof(HyDynamicDmpHeader)};
        enum  {max_body_length = 65535};

        package_message():body_length_(0)
    {

    }

        const char * data() const
        {
            return data_;
        }

        char * data()
        {
            return data_;
        }

        size_t length() const
        {
            return header_length + body_length_;
        }

        const char* body() const
        {
            return data_ + header_length;
        }

        char* body()
        {
            return data_ + header_length;
        }

        size_t body_length() const
        {
            return body_length_;
        }

        bool body_length(size_t new_length)
        {
            body_length_ = new_length;
            if(body_length_ > max_body_length){
                return false;
            }

            return true;
        }

        bool decode_header()
        {
            HyDynamicDmpHeader *reqHead = (HyDynamicDmpHeader *)data_;
            HyDynamicDmpHeader::net2host(reqHead);

            body_length_ = reqHead->wLength - header_length;
            if(body_length_ > max_body_length)
            {
                body_length_ = 0;
                return false;
            }

            return true;
        }

        bool encode_header()
        {
            return true;
        }

        bool clear()
        {
            // set header to zero
            memset(data_, 0, header_length);
            body_length_ = 0;
            return true;
        }

    private:
        char   data_[header_length + max_body_length];
        size_t body_length_;
};

#endif
