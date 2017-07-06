#include "module/flow/youku_flow.h"
#include "module/flow_steps.h"
#include "interface/request.h"
#include "interface/response.h"
#include "core/dsp_session.h"

// see: https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/C_002b_002b-Attributes.html
// GNU C++ allows users to control the order of initialization of objects defined
//     at namesapce scope with the init_priority attribute by specifying a relative
//     priority, a constant integral expression currently bounded between 101 and 65535
//     inclusive. Lower numbers indicate a highter priority
//
// If is dynamic lib, global instances are constructed during dlopen(),
//     destructed at dlclose()
SimpleModuleFactory<YoukuDSPFlow> __attribute__((init_priority(200))) youku_dsp_flow("dsp.youku");

YoukuDSPFlow::YoukuDSPFlow(const char* module_name):FlowBase(module_name)
{
}

YoukuDSPFlow::~YoukuDSPFlow()
{
}

RESULT YoukuDSPFlow::InitModule(const FlowConfig& cfg)
{
    RESULT rel = FlowBase::InitModule(cfg);

    return rel;
}

RESULT YoukuDSPFlow::InitHooks(const FlowConfig& cfg)
{
    CreateHook(GlobalInitStepName);
    CreateHook(AdSelectStepName);
    CreateHook(EndRequestStepName);
    CreateHook(GlobalExitStepName);

    return RESULT_SUCCESS;
}

RESULT YoukuDSPFlow::GlobalInit(Session* session) const
{
    RETURN_TYPE rel = ProcessHook(GlobalInitStepName, session);

    return RET_ISSUCCESSED(rel)?RESULT_SUCCESS : RESULT_ERROR;
}

RESULT YoukuDSPFlow::GlobalExit(Session* session) const
{
    RETURN_TYPE rel = ProcessHook(GlobalExitStepName, session);

    return RET_ISSUCCESSED(rel)?RESULT_SUCCESS : RESULT_ERROR;
}

RESULT YoukuDSPFlow::ProcessEndRequest(Session* session) const
{
    RETURN_TYPE rel = ProcessHook(EndRequestStepName, session);

    return RET_ISSUCCESSED(rel)?RESULT_SUCCESS : RESULT_ERROR;
}

RESULT YoukuDSPFlow::ProcessRequest(Session* session) const
{
    RETURN_TYPE rel = ProcessHook(AdSelectStepName, session);

    return RET_ISSUCCESSED(rel)?RESULT_SUCCESS : RESULT_ERROR;
}

Session* YoukuDSPFlow::CreateSession(const Request* req, Response* resp)
{
    return new DSPSession(const_cast<Request*>(req), resp);
}



