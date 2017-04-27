#ifndef INTERFACE_FLOW_BASE_H_
#define INTERFACE_FLOW_BASE_H_

#include "interface/module.h"
#include <stdio.h>
#include <map>
#include <list>
#include <string>
#include <memory>



class FlowBase : public Flow
{
public:
    FlowBase(const char* module_name);
    virtual ~FlowBase();

    virtual RESULT InitModule(const FlowConfig& cfg);
    virtual RESULT GlobalInit(Session* session) const = 0;
    virtual RESULT GlobalExit(Session* session) const = 0;

    virtual RESULT ProcessRequest(Session*) const = 0;
    virtual RESULT ProcessEndRequest(Session*) const = 0;

public:
    template<class c>
    RESULT Attach(const char* hook_name, C* comp, RETURN_TYPE (C::*hook_func)(Session*) const, int o)
    {
        //soft copy ,need release the SimpleHookFunc pointer resource
        HookInfo hook_info(hook_name, comp, new SimpleHookFunc<C>(hook_func));
        std::map<HookInfo,Hook*>::const_iterator it = hooks_mgr_.find(hook_info);
        if(it != hooks_mgr_.end()){
            DebugLog("duplicate attach the same hook in the same component(%s)",hook_name) ;
            hook_info.clear() ;//soft copy ,release the SimpleHookFunc pointer resource
            return RESULT_ERROR ;
        }

        Hook* hook = new SimpleHook<C>(comp, hook_func, o) ;
        std::pair<std::map<HookInfo,Hook*>::iterator,bool> rel = hooks_mgr_.insert(std::map<HookInfo,Hook*>::value_type(hook_info,hook)) ;
        if (!rel.second) {
            DebugLog("attach hook to step(%s) failure.",hook_name) ;
            hook_info.clear() ;//soft copy ,release the SimpleHookFunc pointer resource
            delete hook ;
            hook = NULL ;
            return RESULT_ERROR ;
        }

        return Attach(hook_name, hook, o);
    };

    template<class C>
    RESULT Detach(const char* hook_name, C* comp, RETURN_TYPE (C::*hook_func)(Session*) const)
    {
        //soft copy ,need release the SimpleHookFunc pointer resource
        HookInfo hook_info(hook_name,comp,new SimpleHookFunc<C>(hook_func)) ;
        Hook* hook = NULL;
        std::map<HookInfo,Hook*>::iterator it = hooks_mgr_.find(hook_info) ;

        //soft copy ,release the SimpleHookFunc pointer resource
        hook_info.clear();

        if(it==hooks_mgr_.end()){
            //TODO: need release the SimpleHookFunc pointer resource
        }
        if(it==hooks_mgr_.end() && (hook=it->second) == NULL){
            DebugLog("try detach a nonexistent hook from step(%s).",hook_name) ;
            return RESULT_ERROR ;
        }

        hooks_mgr_.erase(it);
        RESULT rel = Detach(hook_name, hook);
        delete hook;
        hook = NULL;

        return rel;
    };

    template<class c>
    RESULT GlobalAttach(const char* hook_name, C* comp, RETURN_TYPE (C::*hook_func)(Session*) const, int o)
    {
        //soft copy ,need release the SimpleHookFunc pointer resource
        HookInfo hook_info(hook_name, comp, new SimpleHookFuncW<C>(hook_func));
        std::map<HookInfo,Hook*>::const_iterator it = hooks_mgr_.find(hook_info);
        if(it != hooks_mgr_.end()){
            DebugLog("duplicate attach the same hook in the same component(%s)",hook_name) ;
            hook_info.clear() ;//soft copy ,release the SimpleHookFunc pointer resource
            return RESULT_ERROR ;
        }

        Hook* hook = new GSimpleHook<C>(comp, hook_func, o) ;
        std::pair<std::map<HookInfo,Hook*>::iterator,bool> rel = hooks_mgr_.insert(std::map<HookInfo,Hook*>::value_type(hook_info,hook)) ;
        if (!rel.second) {
            DebugLog("attach hook to step(%s) failure.",hook_name) ;
            hook_info.clear() ;//soft copy ,release the SimpleHookFunc pointer resource
            delete hook ;
            hook = NULL ;
            return RESULT_ERROR ;
        }

        return Attach(hook_name, hook, o);
    };

    template<class C>
    RESULT GlobalDetach(const char* hook_name, C* comp, RETURN_TYPE (C::*hook_func)(Session*) const)
    {
        //soft copy ,need release the SimpleHookFunc pointer resource
        HookInfo hook_info(hook_name,comp,new SimpleHookFuncW<C>(hook_func)) ;
        Hook* hook = NULL;
        std::map<HookInfo,Hook*>::iterator it = hooks_mgr_.find(hook_info) ;

        //soft copy ,release the SimpleHookFunc pointer resource
        hook_info.clear();

        if(it==hooks_mgr_.end()){
            //TODO: need release the SimpleHookFunc pointer resource
        }
        if(it==hooks_mgr_.end() && (hook=it->second) == NULL){
            DebugLog("try detach a nonexistent hook from step(%s).",hook_name) ;
            return RESULT_ERROR ;
        }

        hooks_mgr_.erase(it);
        RESULT rel = Detach(hook_name, hook);
        delete hook;
        hook = NULL;

        return rel;
    };


protected:
    class Hook
    {
    public:
        Hook(int order):order_(order){};
        virtual ~Hook(){};

        virtual Module::RETURN_TYPE Process(Session*) const = 0;
        inline int order(){return order_};

        const int order_;
    };

    template<class C>
    class SimpleHook : public Hook
    {
    public:
        SimpleHook(const C* comp, Module::RETURN_TYPE (C::*hook_func)(Session*) const, int order)
            : Hook(order), comp_(comp), hook_func_(hook_func){};

        virtual Module::RETURN Process(Session* s) const
        {
            DebugLog("[%s]SimpleHook::Process()\n",((Module*)comp_)->module_name().c_str()) ;
            return (comp_->*hook_func_)(s);
        }

    protected:
        const C* comp_;
        Module::RETURN_TYPE (C::*hook_func_)(Session*) const;
    };

    template<class C>
    class GSimpleHook : public Hook
    {
    public:
        SimpleHook(const C* comp, Module::RETURN_TYPE (C::*hook_func)(Session*) const, int order)
            : Hook(order), comp_(comp), hook_func_(hook_func){};

        virtual Module::RETURN Process(Session* s) const
        {
            DebugLog("[%s]SimpleHook::Process()\n",((Module*)comp_)->module_name().c_str()) ;
            return (comp_->*hook_func_)(s);
        }

    protected:
        const C* comp_;
        Module::RETURN_TYPE (C::*hook_func_)(Session*) const;
    };

    typedef std::list<Hook*> HookList;


protected:
    virtual RETURN_TYPE ProcessHook(const std::string hook_name,Session* session);
    virtual RESULT InitComponents(const FlowConfig& cfg) ;//notify attach to hook to components
    virtual RESULT InitHooks(const FlowConfig& cfg)=0 ;//create all hooks that can be attached by components .

    void CreateHook(const char* name) ;
    RESULT Attach(const char* name, Hook* h, int order);
    RESULT Detach(const char* name, Hook* h);

protected:
    std::map<std::string, HookList> hooks_;
    std::list<const FlowComponent*> components_;

private:
    class Hookfunc
    {
    public:
        HookFunc(){};
        virtual ~HookFunc(){};
    };

    template<class C>
    class SimpleHookFunc : public Hookfunc
    {
    public:
        SimpleHookFunc(Module::RETURN_TYPE (C::*hook_func)(Session*) const):HookFunc(),hook_func_(hook_func){} ;
        virtual ~SimpleHookFunc(){};

        //soft copy ,need release the pointer resource
        Module::RETURN_TYPE (C::*hook_func_)(Session*) const;
    };

    template<class C>
    class SimpleHookFuncW : public Hookfunc
    {
    public:
        SimpleHookFuncW(Module::RETURN_TYPE (C::*hook_func)(Session*) ):HookFunc(),hook_func_(hook_func){} ;
        virtual ~SimpleHookFuncW(){};

        //soft copy ,need release the pointer resource
        Module::RETURN_TYPE (C::*hook_func_)(Session*) ;
    };

    class HookInfo
    {
    public:
        HookInfo(const char* hook_name, Module* comp, HookFunc* hook_func)
            :hook_name_(hook_name), comp_(comp), hook_func_(hook_func){};

        HookInfo(const HookInfo& hk){
            hook_name_ = hk.hook_name_;
            comp_      = hk.comp_;
            hook_func_ = hk.hook_func_;
        }

        bool operator==(const HookInfo& dest)  {
            return (hook_name_==dest.hook_name_ && comp_==dest.comp_ && hook_func_==dest.hook_func_) ;
        }

        bool operator<(const HookInfo& dest) const{
            if (hook_name_>dest.hook_name_)  return false ;
            if (hook_name_<dest.hook_name_) return true ;

            // hook_name_ == dest.hook_name_
            if (comp_>dest.comp_) return false ;
            if (comp_<dest.comp_) return true ;

            // hook_name_ == dest.hook_name_ and comp_ == dest.comp_
            return (hook_func_ < dest.hook_func_);
        }

        void clear(){
            comp_ = NULL;
            if(hook_func_){
                delete hook_func_;
                hook_func_ = NULL;
            }
        }

    protected:
        std::string hook_name_;
        Module*     comp_;
        HookFunc*   hook_func_;
    };

    std::map<HookInfo, Hook*> hooks_mgr_;
}; // end of FlowBase


#endif // end of INTERFACE_FLOW_BASE_H_
