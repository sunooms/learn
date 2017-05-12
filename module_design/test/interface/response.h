#ifndef INTERFACE_RESPONSE_H_
#define INTERFACE_RESPONSE_H_

#include <string>

class Response
{
public:
    Response(){};
    virtual ~Response(){};

    std::string  ip;
    std::string  userid;
};

#endif // INTERFACE_RESPONSE_H_
