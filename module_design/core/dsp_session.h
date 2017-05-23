#ifndef CORE_DSP_SESSION_H_
#define CORE_DSP_SESSION_H_

#include "core/session.h"
#include "interface/request.h"
#include "interface/response.h"

class DSPSession : public Session
{
public:
    DSPSession(Request* req, Response* res);
    virtual ~DSPSession();

    Request*  request();
    Response* response();

private:
    Request*  request_;  //bid request
    Response* response_; //bid response
};

#endif //CORE_DSP_SESSION_H_
