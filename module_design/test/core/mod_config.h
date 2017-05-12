#ifndef CORE_MOD_CONFIG_H_
#define CORE_MOD_CONFIG_H_

#include <vector>
#include <string>
#include <map>

class ComponentConfig
{
public:
    enum{
        HOOK_ORDER_LAST = -1,
        HOOK_ORDER_DEFAULT = 0,
        HOOK_ORDER_FIRST = 1,
    };

    ComponentConfig():hook_default_order_(HOOK_ORDER_DEFAULT){};
    ~ComponentConfig(){} ;

    const std::string& class_name() const;
    const std::string& mod_name() const;
    const std::string& ConfigData(std::string param_name) const ;
    int GetHookOrder(std::string hook_name) const ;

protected:
    friend class ModuleConfigHelper;

    std::string mod_name_;
    std::string class_name_;
    std::map<std::string, std::string> conf_data_;

    std::map<std::string ,int> hook_order_;
    int hook_default_order_;
};

class FlowConfig
{
public:
    FlowConfig(){};
    virtual ~FlowConfig(){
        ComponentConfig* comp = NULL;
        components_map_.clear();
        for(size_t i=0; i<components_.size(); i++)
        {
            comp = components[i];
            components[i] = NULL;
            if(comp){
                delete comp
            }
        }
        components_.clear();
    };

    const std::string& class_name() const;
    const std::string& mod_name() const;

    const std::vector<ComponentConfig*>& components() const ;
    const std::map<std::string,ComponentConfig*>& components_map() const ;

protected:
    friend class ModuleConfigHelper;

    std::string mod_name_;
    std::string class_name_;

    std::vector<ComponentConfig*> components_ ;
    std::map<std::string,ComponentConfig*> components_map_ ;
};

class ModuleConfig
{
public:
    ModuleConfig(){};
    virtual ~ModuleConfig(){};

    const std::string& flows_name() const;
    const std::map<std::string, FlowConfig>& flows() const;
    const std::vector<std::string>& modules() const;

protected:
    friend class ModuleConfigHelper;

    std::vector<std::string> modules_;
    std::map<std::string, FlowConfig> flows_;
    std::string flows_name_;
};


inline const std::string& ComponentConfig::class_name() const{
    return class_name_;
}

inline const std::string& ComponentConfig::mod_name() const{
    return mod_name_;
}

inline const std::string& FlowConfig::class_name() const{
    return class_name_;
}

inline const std::string& FlowConfig::mod_name() const{
    return mod_name_;
}

inline const std::vector<ComponentConfig*>& FlowConfig::components() const{
    return components_;
}

inlineconst std::map<std::string, ComponentConfig*>& FlowConfig::components_map() const{
    return components_map_;
}

inline const std::string& ComponentConfig::ConfigData(std::string param_name) const{
    static const std::string null_string("");
    std::map<std::string, std::string>::const_iterator it=conf_data_.find(param_name);
    return (it!=conf_data.end())?it->second:null_string;
}

inline int ComponentConfig::GetHookOrder(std::string hook_name) const{
    std::map<std::string, int>::const_iterator it=hook_order_.find(hook_name);
    return (it!=hook_order_.end())?it->second:hook_default_order_;
}

inline const std::string& ModuleConfig::flows_name() const{
    return flows_name_;
}

inline const std::map<std::string, FlowConfig>& ModuleConfig::flows() const{
    return flows_;
}

inline const std::vector<std::string>& ModuleConfig::modules() const{
    return modules_;
}

#endif // CORE_MOD_CONFIG_H__
