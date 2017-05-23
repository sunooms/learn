#include "interface/module.h"

// see: https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/C_002b_002b-Attributes.html
// GNU C++ allows users to control the order of initialization of objects defined
//     at namesapce scope with the init_priority attribute by specifying a relative
//     priority, a constant integral expression currently bounded between 101 and 65535
//     inclusive. Lower numbers indicate a highter priority
//
// If is dynamic lib, global instances are constructed during dlopen(),
//     destructed at dlclose()
static std::map<std::string, ModuleFactory*> __attribute__((init_priority(101))) modules_registry_;

ModuleRegistry::ModuleRegistry()
{

}

ModuleRegistry::~ModuleRegistry()
{

}

bool ModuleRegistry::Add(ModuleFactory* mod_factory, const char* mod_name)
{
    if(mod_factory && mod_name)
    {
        std::pair<std::map<std::string, ModuleFactory*>::iterator,bool> rel =
            modules_registry_.insert(std::map<std::string, ModuleFactory*>::value_type(mod_name, mod_factory));

        DebugLog("ModuleRegistry:Add:%s %s\n", mod_name, (rel.second)?"success":"failure");
        return rel.second;
    }

    DebugLog("ModuleRegistry:Add:%s failure\n", mod_name);
    return false;
}

bool ModuleRegistry::Remove(ModuleFactory* mod_factory, const char* mod_name)
{
    DebugLog("ModuleRegistry:remove:%s start\n", mod_name);

    if(mod_factory && mod_name)
    {
        std::map<std::string, ModuleFactory*>::iterator it = modules_registry_.find(mod_name);
        if(it != modules_registry_.end() && it->second == mod_factory)
        {
            modules_registry_.erase(it);
            DebugLog("ModuleRegistry:remove:%s success\n", mod_name);
            return true;
        }
    }

    DebugLog("ModuleRegistry:remove:%s failure\n", mod_name);
    return false;
}

ModuleFactory* ModuleRegistry::Lookup(const char* mod_name)
{
    std::map<std::string, ModuleFactory*>::iterator it = modules_registry_.find(mod_name);
    return (it != modules_registry_.end()&&it->second!=NULL )?it->second:NULL;
}


Module* ModuleRegistry::CreateModule(const char* mod_name)
{
    ModuleFactory* mod_factory = Lookup(mod_name);
    return mod_factory ? mod_factory->Build() : NULL;
}


