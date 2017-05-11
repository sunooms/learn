#ifndef DRIVE_DISPATCHER_H_
#define DRIVE_DISPATCHER_H_

#include "common/global_consts.h"
#include <string>
#include <list>
#include <map>


class FlowConfig;
class Flow;
class Session;
class DispatcherMgr;

class Dispatcher
{
public:
    Dispatcher();
    virtural ~Dispatcher();

    // call during both initial and update
    // return modules count, >0 success; <=0 load failure
    virtual int    LoadAllModule();
    virtual RESULT HandleRequest(const IRequest* req, IResponse* resp);

protected:
    struct ModuleInfo
    {
        std::string mod_path;
        void* mod_handle;
    };

    bool LoadModule(std::string mod_filepath);
    bool ReleaseModule();
    bool InitModule();

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

protected:
    time_t      GetConfigFileTimestamp();
    Dispatcher* CheckAndLoadNewDispatcher();

public:
    static std::string s_mod_conf_path_;

protected:
    Dispatcher* dispatcher_;
    std::string config_version_;

    time_t      last_timestamp_;

};

#endif // DRIVE_DISPATCHER_H_
