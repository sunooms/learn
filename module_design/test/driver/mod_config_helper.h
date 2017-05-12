/*
 *  File name: mod_config_helper.h
 *  author:     xu.guangming
 *
 *   Description:  declaration of class moduleConfigHelper
 *
 * */

#ifndef DRIVER_MOD_CONFIG_HELPTER_H
#define DRIVER_MOD_CONFIG_HELPTER_H

#include <string>
#include "core/mod_config.h"

class ModuleConfigHelper
{
public:
    ModuleConfigHelper();
    virtual ~ModuleConfigHelper();

    static bool LoadModuleConfig(ModuleConfig& mod_cfg);
    static std::string GetConfFilePath();

    static std::string s_mod_conf_path_;

protected:
    static bool _LoadFlowConfig(rapidxml::xml_node<>* flow_node, FlowConfig& flow_cfg);
};


#endif // DRIVER_MOD_CONFIG_HELPER_H
