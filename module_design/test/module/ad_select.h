#ifndef MODULE_AD_SELECT_H_
#define MODULE_AD_SELECT_H_

#include "interface/flow.h"
#include <map>
#include <string>
#include <stdio.h>


class AdSelect : public FlowComponent
{
public:
    AdSelect(const char* module_name);
    virtual ~AdSelect();

    virtual RESULT InitModule(const ComponentConfig& cfg);
    virtual RESULT Attach(FlowBase* flow);
    virtual RESULT Detach(FlowBase* flow);

    RETURN_TYPE Selecting(Session *) const;
    RETURN_TYPE EndHandle(Session *) const;

protected:
    ComponentConfig comp_conf_;
    int default_order_;
}

#endif // MODULE_AD_SELECT_H_

