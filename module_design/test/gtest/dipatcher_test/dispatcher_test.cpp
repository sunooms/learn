#include "driver/dispatcher.h"
#include "driver/mod_config_helper.h"

using namespace std;

int main()
{
    ModuleConfigHelper::s_mod_conf_path_ = "dsp_modules.xml";

    DispatcherMgr dispatcher_mgr;

    return 0;
}
