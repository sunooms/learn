#ifndef __DLIB_FLOW_H__
#define __DLIB_FLOW_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../include/dynamic_message_protocol.h"
#include "../include/package_message.h"

class dlib_flow
{
    public:
        dlib_flow(package_message& read_msg):read_msg_(read_msg)
    {}
        ~dlib_flow()
        {
        }

        bool dynamic_process(package_message& res_msg);

        int proto_parse(std::string& retmsg);

    private:
        package_message& read_msg_;
};

#endif
