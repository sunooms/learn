//
//  File name: module.h
//  Author: xu.guangming
//  Project: PBS reconstruction
//
//  Revision history
//  2017/04/25  init version
//
//
#ifndef   INTERFACE_MODULE_H_ 
#define   INTERFACE_MODULE_H_

#include <stdio.h>
#include <map>
#include <list>
#include <string>
#include "common/global_define.h"


class Module
{
public:
    enum RETURN_TYPE
    {
        // must break
        RET_TYPE_ERROR       = -1;  // must break;
        RET_TYPE_CONTINUE    = 0;
        RET_TYPE_NEXT_STEP   = 1;   // skip other hook of this step
        RET_TYPE_FINAL_STEP  = 50;  // goto final step, skip other steps
    };
#define RET_ISFAILED(rel) (rel <= RET_TYPE_ERROR)
#define RET_ISSUCCESSED(rel) (rel >= RET_TYPE_CONTINUE)

public:
    Module(const char* module_name):module_name_(module_name){};
    virtual ~Module(){
        DebugLog("[%s]~Module()\n", module_name_.c_str());
    };
    inline const std::string module_name() const {return module_name_;};
protected:
    std::string module_name_;
};


class ModuleFactory
{
public:
    virtual Module* Build() = 0;
    virtual ~ModuleFactory(){};
};


class ModuleRegistry
{
public:
    ModuleRegistry();
    virtual ~ModuleRegistry();

    static bool Add(ModuleFactory* mod_factory, const char* mod_name);
    static bool Remove(ModuleFactory* mod_factory, const char* mod_name);
    static Module* CreateModule(const char* mod_name);
};


// template module factory
template<class M>
class SimpleModuleFactory : public ModuleFactory
{
public:
    SimpleModuleFactory(const char* mod_name):mod_name_(mod_name)
    {
        ModuleRegistry::Add(this, mod_name_.c_str());
    };
    virtual ~SimpleModuleFactory()
    {
        ModuleRegistry::Remove(this, mod_name.c_str());

        DebugLog("[%s] ~SimpleModuleFacotry mod_size=%zd\n", mod_name.c_str(), modules_.size());

        M* mod = NULL;
        typename std::list<M*>::iterator it = modules_.begin();
        for(; it!=modules_.end(); it++)
        {
            if( NULL != (mod=(*it)))
            {
                // here delete mod
            }
        }
        modules_.clear();
    };

    virtual Module* Build()
    {
        /*typename*/ M* mod = new M(mod_name.c_str());
        modules_.push_back(mod);

        return dynamic_cast<Module*>(mod);
    };

private:
    std::list<M*> modules_;
    std::string   mod_name_;
};


#endif   // end of INTERFACE_MODULE_H_
