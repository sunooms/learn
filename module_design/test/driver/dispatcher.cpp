#include <sys/types.h>
#include <sys/stat.h>

#include "driver/dispatcher.h"
#include "interface/module.h"
#include "interface/flow.h"

Dispatcher::FlowInfo::FlowInfo(std::string name, const FlowConfig& conf, Flow* f, Session* s)
    :Flow_class_(name), flow_conf_(conf), instance_(f), globale_session_(s)
{
}

Dispatcher::FlowInfo::~FlowInfo()
{
    GlobalExit();
}

bool Dispatcher::FlowInfo::GlobalExit()
{
    if(instance_)
    {
        instance_->GlobalExit(global_session_);
        delete instance_;
        instance_ = NULL;
    }

    if(global_session_)
    {
        delete global_session_;
        global_session_ = NULL;
    }

    return true;
}

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
    ReleaseAllModule();
}

bool Dispatcher::ReleaseAllModule()
{
    // 1 release flows resource and global_exit etc.
    FlowInfo* flow_info=NULL;
    std::map<std::string, FlowInfo*>::iterator it_flow = flows_begin();
    for(; it_flow!=flows_.end(); it_flow++)
    {
        if(NULL != (flow_info=it_flow->second))
        {
            flow_info->GlobalExit();
            delete flow_info;
        }
    }
    flows_.clear();

    // 2 dlclose all so
    std::list<ModuleInfo>::iterator it = modules_list_.begin();
    for(; it!=modules_list_.end(); it++)
    {
        ReleaseModule(*it);
    }
    module_list_.clear();

    return true;
}

bool Dispatcher::ReleaseModule(ModuleInfo& mod_info)
{
    return true;
}

int Dispatvher::LoadAllModule()
{
    ModuleConfigHelper conf_helper;
    if(!conf_helper.LoadModuleConfig(module_config_))
    {
        return 0;
    }

    for(size_t i=0; i<module_config_.modules().size(); ++i)
    {
        if(!LoadModule(module_config_.modules()[i]))
        {
            DebugLog("load flow(%s), ignore component of this flow\n", module_config_.modules()[i].c_str());
            continue;
        }
    }

    std::map<std::string, FlowConfig>::const_iterator it_flow = module_config_.flows().begin();
    for(; it_flow!=module_config_.flow().end(); ++it_flow)
    {
        InitModule(it_flow->second);
    }

    return modules_list_.size();
}

bool Dispatcher::InitModule(const FlowConfig& flow_config)
{
    Module* module = ModuleRegistry::CreateModule(flow_config.class_name().c_str());
    Flow*   flow   = dynamic_cast<Flow *>(module);

    if(NULL == flow){
        DebugLog("%s create flow failure, skip this flow.\n", flow_conf.class_name().c_str());
        return false;
    }

    if(flow->InitModule(flow_config) != 0)
    {
        DebugLog("%s flow init failure, skip this flow.\n", flow_conf.class_name().c_str());
        return false;
    }

    // global session object managed by FlowInfo for GlobalExit
    Session* session = flow->CreateSession(NULL, NULL);
    if(flow->GlobalInit(session) == -1)
    {
        DebugLog("Error: global init failed\n");
    }

    FlowInfo* flow_info = new FlowInfo(flow_conf.class_name(), flow_conf, flow, session);
    std::pair<std::map<std::string, FlowInfo*>::iterator, bool> rel = flows_.insert(std::map<std::string, FlowInfo*>::value_type(flow_conf.mod_name(), flow_info));
    if(!rel.second)
    {
        DebugLog("Error: try create the same class name flow(%s)(mod name:%s). ignored the later\n"
                , flow_confg.class_name().c_str(), flow_conf.mod_name().c_str());
        delete flow_info;
        flow_info = NULL;

        return false;
    }

    return true;
}

RESULT Dispatcher::HandleRequest(const Request* req, Response* resp)
{
    RESULT rel = RESULT_ERROR;

    if(NULL == req || NULL == resp)
    {
        return RESULT_ERROR;
    }

    std::map<std::string, FlowInfo*>::iterator it = flows_.find(req->action());
    FlowInfo* flow_info = (it != flows_.end())?it->second:NULL;

    if(flow_info && flow_info->instance_)
    {
        Session* session = flow_info->instance_->CreateSession(req, resp);
        if(NULL == session)
        {
            DebugLog("error: create session failure for the request %s\n", req->action().c_str());
            return RESULT_ERROR;
        }

        rel = flow_info->instance_->ProcessRequest(session);
        flow_info->instance_->ProcessEndRequest(session);

        delete session;
        session = NULL;
    }
    else{
        DebugLog("error: not found flow for action(%s)\n", req->action().c_str());
    }
}


DispatcherMgr::DispatcherMgr():dipatcher_(NULL),config_version_("0"),last_timestamp_(0)
{
    exit_thread_ = false;
}

DispatcherMgr::~DispatcherMgr()
{
    ExitInstance();
}

// thread initialization
int DispatcherMgr::InitInstance()
{
    int rel = 0;

    dispatcher_ = CheckAndLoadNewDispatcher();
    if(NULL == dispatcher_){
        return RESULT_ERROR;
    }

    return rel;
}

int DispatcherMgr::ExitInstance()
{
    Dispatcher *tmp_disp = dispatcher_;
    dispatcher_ = NULL;

    if(tmp_disp){
        tmp_disp->ReleaseAllModule();
        delete tmp_disp;
        tmp_disp = NULL;
    }

    return 0;
}

time_t DispatcherMgr::GetConfigFileTimestamp()
{
    //conf/modules.xml
    std::string module_conf_file(ModuleConfigHelper::GetConfFilePath());
    struct stat stat_info;

    if(stat(module_conf_file.c_str(), &stat_info) != -1){
        return stat_info.st_mtime;
    }

    return 0;
}

Dispatcher* DispatcherMgr::CheckAndLoadNewDispatcher()
{
    time_t timestamp = GetConfigFileTimestamp();

    Dispatcher* disp = NULL;

    if(timstamp > last_timestamp_ || last_timestamp_ == 0){
        last_timestamp_ = timestamp;
        disp = new Dispatcher();
        disp->LoadAllModule();
    }

    return disp;
}

// running and idle processing
// here use thread to update dispatcher
int DispatcherMgr::Run()
{
    Dispatcher* new_disp = NULL;
    Dispatcher* old_disp = dispatcher_;

    while (!exit_thread_)
    {
        // check modules.xml and load new version
        new_disp = CheckAndLoadNewDispatcher();
        if(new_disp && new_disp != dispatcher_)
        {
            // todo something
        }

        usleep(100 * 1000);
    }

    Dispatcher* tmp_disp = dispatcher_ ;
    dispatcher_ = NULL;

    usleep(100 * 1000);
    if(tmp_disp){
        delete tmp_disp;
        tmp_disp = NULL;
    }

    return 0;
}

