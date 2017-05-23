#ifndef MODULE_WORKFLOW_YOUKU_FLOW_H_ 
#define MODULE_WORKFLOW_YOUKU_FLOW_H_ 

#include "interface/flow.h"

class YoukuDSPFlow : public FlowBase
{
public:
    YoukuDSPFlow(const char* module_name);
    virtual ~YoukuDSPFlow();

    virtual RESULT InitModule(const FlowConfig& cfg);
    virtual RESULT InitHooks(const FlowConfig& cfg);

    virtual RESULT GlobalInit(Session* session) const;
    virtual RESULT GlobalExit(Session* session) const;
    virtual RESULT ProcessRequest(Session* session) const;
    virtual RESULT ProcessEndRequest(Session* session) const;

    virtual Session* CreateSession(const Request* req, Response* resp);

protected:
};

#endif //MODULE_WORKFLOW_YOUKU_FLOW_H_
