#include "core/dsp_session.h"

DSPSession::DSPSession(Request* req, Response* res)
    :request_(req), response_(res)
{
}

DSPSession::~DSPSession()
{
}

Request* DSPSession::request()
{
    return request_;
}

Response* DSPSession::response()
{
    return response_;
}

