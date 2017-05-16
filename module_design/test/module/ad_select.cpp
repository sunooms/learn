#include "module/ad_select.h"
#include "core/session.h"
#include "interface/module.h"

SimpleModuleFactory<AdSelect> selecting("dsp.ad_select");

AdSelect::AdSelect(const char* module_name):FlowComponent(module_name)
{
    default_order_ = 999;
}

AdSelect::~AdSelect()
{
}

RESULT AdSelect::InitModule(const ComponentConfig& cfg)
{
    comp_conf_ = cfg;
    default_order_ = 999; // get from FlowConfig

    return RESULT_SUCCESS;
}

RESULT AdSelect::Attach(FlowBase* flow)
{
    DebugLog("%s in AdSelect::attach()\n", module_name_.c_str());

    std::string hook_name;

    hook_name = "ad_select";
    flow->Attach(hook_name.c_str(), this, &AdSelect::Selecting, comp_conf_.GetHookOrder(hook_name));

    hook_name = "end_handling";
    flow->Attach(hook_name.c_str(), this, &AdSelect::Selecting, comp_conf_.GetHookOrder(hook_name));
    DebugLog("%s out AdSelect::attach()\n", module_name_.c_str());

    return RESULT_SUCCESS;
}

RESULT AdSelect::Detach(FlowBase* flow)
{
    DebugLog("%s in AdSelect::Detach()\n", module_name_.c_str());

    std::string hook_name;

    hook_name = "ad_select";
    flow->Detach(hook_name.c_str(), this, &AdSelect::Selecting);

    hook_name = "end_handling";
    flow->Detach(hook_name.c_str(), this, &AdSelect::Selecting);
    DebugLog("%s out AdSelect::detach()\n", module_name_.c_str());

    return RESULT_SUCCESS;
}

Module::RETURN_TYPE AdSelect::Selecting(Session* session) const
{
    DebugLog("%s in AdSelect::Selecting()\n", module_name_.c_str());

    return RET_TYPE_CONTINUE;
}

Module::RETURN_TYPE AdSelect::EndHandle(Session* session) const
{
    DebugLog("[%s]in AdSelect::EndHandle()\n",module_name_.c_str()) ;
    return RET_TYPE_CONTINUE ;
}


