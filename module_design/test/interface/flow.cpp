#include "interface/flow.h"
#include <assert.h>

FlowBase::FlowBase(const char* module_name):Flow(module_name)
{
}

FlowBase::~FlowBase()
{
    // loop and delete all components/hooks
    DebugLog("~Flow(%s)\n",module_name_.c_str()) ;

    const FlowComponent* comp = NULL ;
    std::list<const FlowComponent*>::iterator it = components_.begin();
    for (; it!=components_.end(); it++)
    {
        if ((comp=(*it))!=NULL)
        {
            //comp->detach(this)
            //delete comp
        }
    }
    components_.clear();
    DebugLog("out ~Flow(%s)\n",module_name_.c_str());
}

void FlowBase::CreateHook(const char* hook_name)
{
    DebugLog("[%s]FlowBase::createHook(%s)\n",module_name_.c_str(),hook_name) ;
    HookList list ;
    hooks_.insert(std::map<std::string, HookList>::value_type(hook_name, list));
}

RESULT FlowBase::Attach(const char* hook_name, Hook* h, int order)
{
    RESULT rel = RESULT_ERROR;
    DebugLog("[%s]FlowBase::attach(%s) order(%d)\n",module_name_.c_str(),hook_name,order) ;
    std::map<std::string,HookList>::iterator it = hooks_.find(hook_name);
    if (it!=hooks_.end())
    {
        HookList & hook_list = (it->second) ;
        HookList::iterator it_list=hook_list.begin() ;
        for (; it_list!=hook_list.end(); it_list++) {
            Hook*& hook = (*it_list) ;
            if (hook && hook->order()>order) {
                hook_list.insert(it_list,h);
                rel = RESULT_SUCCESS ;
                break;
            }
        }
        if (rel==RESULT_ERROR) {
            hook_list.push_back(h) ;
            rel = RESULT_SUCCESS ;
        }
        DebugLog("[%s]FlowBase::attach(%s) size=%zd\n",module_name_.c_str(),hook_name,hook_list.size()) ;
    }
    else
    {
        DebugLog("[%s]FlowBase::attach(%s) not found hook\n",module_name_.c_str(),hook_name) ;
    }

    return rel;
}

RESULT FlowBase::Detach(const char* name, Hook* h)
{
    DebugLog("[%s]FlowBase::Detach(%s)\n",module_name_.c_str(),name) ;

    RESULT rel = RESULT_ERROR ;
    std::map<std::string,HookList>::iterator it = hooks_.find(name);
    if (it!=hooks_.end())
    {
        HookList & hook_list = (it->second) ;
        HookList::iterator it_list=hook_list.begin() ;
        for (; it_list!=hook_list.end(); it_list++) {
            Hook*& hook = (*it_list) ;
            if (hook==h) {
                hook_list.erase(it_list) ;
                rel = RESULT_SUCCESS ;
                break;
            }
        }
    }
    return rel ;
}

RESULT FlowBase::InitModule(const FlowConfig& cfg)
{
    DebugLog("[%s]in FlowBase::InitModule()\n",module_name_.c_str()) ;

    // create step hook
    RESULT rel = InitHooks(cfg);

    if (RESULT_ISFAILED(rel)) {
        DebugLog("[%s]out FlowBase::InitModule(), init hooks failure(%d)\n",module_name_.c_str(),rel) ;
        return rel ;
    }

    RESULT rel_comp = InitComponents(cfg) ;
    if (RESULT_ISFAILED(rel_comp)) {
        DebugLog("[%s]out FlowBase::InitModule(), init components failure(%d)\n",module_name_.c_str(),rel_comp) ;
        return RESULT_ERROR ;
    }

    if (RESULT_ISSUCCESSED(rel)) {
        DebugLog("[%s]out FlowBase::InitModule(), init hooks warning\n",module_name_.c_str()) ;
        return rel_comp ;//when rel is success ,return rel_comp value
    }

    if (RESULT_ISSUCCESSED(rel_comp)) {
        DebugLog("[%s]out FlowBase::InitModule()\n, init components warning",module_name_.c_str()) ;
        return rel ; //when rel_comp is success ,return rel value
    }

    DebugLog("[%s]out FlowBase::InitModule()\n",module_name_.c_str()) ;

    return RESULT_SUCCESS ;
}

RESULT FlowBase::InitComponents(const FlowConfig& cfg)
{
    //create component and attach step hook
    FlowComponent* comp = NULL ;
    const ComponentConfig* comp_info = NULL ;
    for (size_t index=0; index<cfg.components().size(); index++) {
        if ((comp_info=cfg.components()[index])!=NULL) {
            comp = dynamic_cast<FlowComponent*>(ModuleRegistry::CreateModule(comp_info->class_name().c_str()));
            if (comp != NULL)
            {
                comp->InitModule(*comp_info);
                components_.push_back(comp);
                comp->Attach(this);
            } else{
                DebugLog("create module(%s)(%s) failure\n",comp_info->class_name().c_str(),comp_info->mod_name().c_str()) ;
            }
        }
    }

    return RESULT_SUCCESS ;
}

FlowBase::RETURN_TYPE FlowBase::ProcessHook(const std::string hook_name,Session* session) const
{
    RETURN_TYPE rel = RET_TYPE_CONTINUE ;
    std::map<std::string,HookList>::const_iterator it_hook=hooks_.find(hook_name) ;
    if (it_hook!=hooks_.end())
    {
        const HookList& hook_list = (it_hook->second) ;
        HookList::const_iterator it_list = hook_list.begin() ;
        for (; it_list!=hook_list.end() && rel==RET_TYPE_CONTINUE; it_list++) {
            if ((*it_list)!=NULL) {
                rel = (*it_list)->Process(session) ;
                DebugLog("    result:%d\n",rel) ;
            }
        }
        DebugLog("[%s]FlowBase::ProcessHook(%s) rel=%d (total:%zd)\n",module_name_.c_str(),hook_name.c_str(),rel,hooks_.size()) ;
    }else{
        DebugLog("[%s]FlowBase::ProcessHook(%s) not found this hook name(total:%zd)\n",module_name_.c_str(),hook_name.c_str(),hooks_.size()) ;
    }

    return rel ;
}


