#include "driver/dispatcher.h"
#include "driver/mod_config_helper.h"

using namespace std;

int main()
{
    ModuleConfigHelper::s_mod_conf_path_ = "gtest_modules.xml";

    DispatcherMgr dispatcher_mgr;

    int mod_count = dispatcher_mgr.InitInstance();

    printf("now mod_count :%d\n", mod_count);

    return 0;
}
