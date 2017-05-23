#include "core/session.h"
#include "interface/module.h"

bool Session::SetSharedData(const std::string& key, SessionData* data)
{
    std::map<std::string, SessionData*>::iterator it = shared_data_.find(key);
    if(it != shared_data_.end())
    {
        it->second = data;
        return true;
    }

    std::pair<std::map<std::string, SessionData*>::iterator, bool> rel =
        shared_data_.insert(std::map<std::string, SessionData*>::value_type(key, data));

    return rel.second;
}

SessionData* Session::GetSharedData(const std::string& key)
{
    std::map<std::string, SessionData*>::iterator it =  shared_data_.find(key);

    return (it != shared_data_.end())?it->second:NULL;
}

bool Session::SetPrivateData(const Module* mod, const std::string& key, SessionData* data)
{
    std::string prefix_key((mod)?mod->module_name():"");
    prefix_key += key;

    std::map<std::string, SessionData*>::iterator it = private_data_.find(prefix_key);
    if(it != private_data_.end())
    {
        it->second = data;
        return true;
    }

    std::pair<std::map<std::string, SessionData*>::iterator, bool> rel =
        private_data_.insert(std::map<std::string, SessionData*>::value_type(prefix_key, data));

    return rel.second;
}

SessionData* Session::GetPrivateData(const Module* mod, const std::string& key)
{
    std::string prefix_key((mod)?mod->module_name():"");
    prefix_key += key;

    std::map<std::string, SessionData*>::iterator it =  private_data_.find(prefix_key);

    return (it != private_data_.end())?it->second:NULL;
}

