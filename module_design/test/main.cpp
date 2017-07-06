#include <iostream>
#include "interface/module.h"
#include "module/flow_component/ad_select.h"
#include "driver/dispatcher.h"
#include "driver/mod_config_helper.h"

using namespace std;

class template_test
{
    public:
        template_test(const char* mod_name):mod_name_(mod_name)
        {};
        virtual ~template_test(){};

    protected:
        std::string mod_name_;
};

int main()
{
    SimpleModuleFactory<template_test> ranking("dsp.template_test");
    ModuleConfigHelper::s_mod_conf_path_ = "gtest_modules.xml";

    DispatcherMgr dispatcher_mgr;

    int mod_count = dispatcher_mgr.InitInstance();
    printf("now mod_count :%d\n", mod_count);

    printf("in test\n");
    return 0;
}
