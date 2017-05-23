#ifndef DRIVE_DISPATCHER_H_
#define DRIVE_DISPATCHER_H_

#include <string>
#include <list>
#include <map>
#include "common/global_define.h"
#include "core/mod_config.h"


class FlowConfig;
class Flow;
class Session;
class DispatcherMgr;
class Request;
class Response;

class Dispatcher
{
public:
    Dispatcher();
    virtual ~Dispatcher();

    // call during both initial and update
    // return modules count, >0 success; <=0 load failure
    virtual int    LoadAllModule();
    virtual RESULT HandleRequest(const Request* req, Response* resp);

protected:
    struct ModuleInfo
    {
        std::string mod_path;
        void* mod_handle;
    };

    bool LoadModule(std::string mod_filepath);
    bool ReleaseModule(ModuleInfo& mod);
    bool ReleaseAllModule();
    bool InitModule(const FlowConfig& cfg);

    ModuleConfig module_config_;
    std::list<ModuleInfo> modules_list_;

    class FlowInfo
    {
    public:
        FlowInfo(std::string name, const FlowConfig& conf, Flow* f, Session* s);
        virtual ~FlowInfo();

        bool GlobalExit();

    protected:
        friend class Dispatcher;
        std::string flow_class_;
        const FlowConfig& flow_conf_;
        Flow* instance_;
        Session* global_session_;
    };

    std::map<std::string, FlowInfo*> flows_; // flow class to flowinfo

    friend class DispatcherMgr;
};

class DispatcherMgr
{
public:
    DispatcherMgr();
    virtual ~DispatcherMgr();

    inline Dispatcher* GetDispatcher() const {return dispatcher_;};

    virtual int InitInstance();
    virtual int ExitInstance();

    // runing
    virtual int Run();

protected:
    time_t      GetConfigFileTimestamp();
    Dispatcher* CheckAndLoadNewDispatcher();

public:
    static std::string s_mod_conf_path_;

protected:
    Dispatcher* dispatcher_;
    std::string config_version_;

    time_t      last_timestamp_;
    bool        exit_thread_;
};

#endif // DRIVE_DISPATCHER_H_
