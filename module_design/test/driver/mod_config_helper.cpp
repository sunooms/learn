#include "common/global_define.h"
#include "driver/mod_config_helper.h"
#include "third/rapidxml/rapidxml_utils.hpp"

std::string ModuleConfigHelper::s_mod_conf_path_ = DSP_MOD_CONFIG_PATH ;

ModuleConfigHelper::ModuleConfigHelper()
{

}

ModuleConfigHelper::~ModuleConfigHelper()
{

}

std::string ModuleConfigHelper::GetConfFilePath()
{
    return (!s_mod_conf_path_.empty())?s_mod_conf_path_:DSP_MOD_CONFIG_PATH;
}

bool ModuleConfigHelper::LoadModuleConfig(ModuleConfig& mod_cfg)
{
    rapidxml::file<> conf_file(ModuleConfigHelper::GetConfFilePath().c_str());

    DebugLog("config info: %s\n", conf_file.data());

    rapidxml::xml_document<> conf_doc;
    conf_doc.parse<0>(conf_file.data());

    rapidxml::xml_node<>* root_node = conf_doc.first_node();

    if(NULL == root_node)
    {
        DebugLog("invalid config file: invalid root node\n");
        return false;
    }

    rapidxml::xml_attribute<>* attr_node = root_node->first_attribute("name");
    mod_cfg.flows_name_ = (attr_node!=NULL)?attr_node->value():"";

    DebugLog("Load config file success: [%s] name:%s\n", root_node->name(),
            mod_cfg.flows_name_.c_str());

    // load flow
    rapidxml::xml_node<>* flow_node = root_node->first_node("flow");
    if(NULL == flow_node)
    {
        DebugLog("invalid config file: invalid first flow node\n");
        return false;
    }

    typedef std::map<std::string, FlowConfig> FlowConfigMap;
    do
    {
        FlowConfig  flow_conf;

        // flow name
        attr_node = flow_node->first_attribute("name");
        flow_conf.mod_name_ = (attr_node!=NULL)?attr_node->value():"";

        // flow class name
        attr_node = flow_node->first_attribute("class");
        flow_conf.class_name_ = (attr_node!=NULL)?attr_node->value():"";

        DebugLog(" %s %s\n", flow_conf.mod_name_.c_str(), flow_confg.class_name.c_str());

        std::pair<FlowConfigMap::iterator, bool> rel
            = mod_cfg.flows_.insert(FlowConfigMap::value_type(flow_conf.mod_name_, flow_conf));
        if(!rel.second)
        {
            DebugLog("  [%s] insert to map failure\n", flow_conf.mod_name_.c_str());
            return false;
        }

        FlowConfig& tmp_flow = rel.first->second;
        if(!_LoadFlowConfig(flow_node, tmp_flow))
        {
            mod_cfg.flows_.erase(rel.first);
            DebugLog("  [%s] load flow config failure\n", flow_conf.mod_name_.c_str());
        }

    }while( NULL != (flow_node=flow_node->next_sibling("flow")) );

    return (mod_cfg.flows_.size() > 0);
}

bool ModuleConfigHelper::_LoadFlowConfig(rapidxml::xml_node<>* flow_node, FlowConfig& flow_cfg)
{
    // flow name
    rapidxml::xml_attribute<>* attr_node = flow_node->first_attribute("name");
    flow_cfg.mod_name_ = (attr_node!=NULL)?attr_node->value():"";

    // flow class name
    attr_node = flow_node->first_attribute("class");
    flow_cfg.class_name_ = (attr_node!=NULL)?attr_node->value():"";

    // load component info
    rapidxml::xml_node<>* tmp_node = flow_node->first_node("components");
    if(NULL == tmp_node)
    {
        DebugLog(" load flow:%s components root node failure\n", flow_cfg.mod_name_.c_str());
        return false;
    }

    rapidxml::xml_node<>* param_node = NULL;
    std::string param_name;
    std::string param_value;
    std::string hook_name;
    int order = 0;

    for(tmp_node=tmp_node->first_node("component"); tmp_node != NULL; tmp_node=tmp_node->next_sibling("component"))
    {
        ComponentConfig* comp = new ComponentConfig;
        flow_cfg.components_.push_back(comp);

        // default order for all hooks of this component in the flow
        comp->hook_default_order_ = 0;

        // name attribute
        attr_node  = tmp_node->first_attribute("name");
        comp->mod_name_ = (attr_node!=NULL)?attr_node->value():"";

        // class attribute
        attr_node  = tmp_node->first_attribute("class");
        comp->class_name_ = (attr_node!=NULL)?attr_node->value():"";

        DebugLog(" load component:%s, class=%s\n", comp->mod_name.c_str(), comp->class_name_.c_str());

        for(param_node=tmp_node->first_node("param"); param_node!=NULL; param_node=param_node->next_sibling("param"))
        {
            //param name
            attr_node = param_node->first_attribute("name");
            param_name = (attr_node!=NULL)?attr_node->value():"";

            //param value
            param_value = param_node->value();

            if(param_name.compare("order") == 0)
            {
                attr_node = param_node->first_attribute("hook");
                hook_name = (attr_node!=NULL)?attr_node->value():"";
                order = atoi(param_value.c_str());

                if(hook_name.empty() && order != ComponentConfig::HOOK_ORDER_DEFAULT)
                {
                    //default order for all hooks of this component in the flow
                    comp->hook_default_order_ = order;
                }else{
                    comp->hook_order_.insert(std::map<std::string,int>::value_type(hook_name, order));
                }

                DebugLog("  load order(hook:%s)=%d\n", hook_name.c_str(), order);
            }else{
                comp->conf_data_.insert(std::map<std::string,std::string>::value_type(param_name, param_value));
                DebugLog("  load param(%s)=%s\n", param_name.c_str(), param_value.c_str());
            }
        }
        flow_cfg.components_map_.insert(std::map<std::string, ComponentConfig*>::value_type(comp->class_name_, comp));
    }

    DebugLog(" load flow(%s) components info %s: comp count=%zd(%zd)\n------\n\n",
            flow_cfg.mod_name_.c_str(),
            (flow_cfg.components_map_.size()>0)?"success":"failure",
            flow_cfg.components_.size(),flow_cfg.components_map_.size()) ;

    return true;
}



