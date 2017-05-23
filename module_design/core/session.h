#ifndef CORE_SESSION_H_
#define CORE_SESSION_H_

#include <string>
#include <map>

class Module;

class SessionData
{
public:
    SessionData(){};
    virtual ~SessionData(){};
};

class Session
{
public:
    Session(){};
    virtual ~Session(){};

    // shared data, key = original
    bool SetSharedData(const std::string& key, SessionData* data);
    SessionData* GetSharedData(const std::string& key);

    // private data, key = mod_name + original
    bool SetPrivateData(const Module* mod, const std::string& key, SessionData* data);
    SessionData* GetPrivateData(const Module* mod, const std::string& key);

protected:
    std::map<std::string, SessionData*> shared_data_ ;
    std::map<std::string, SessionData*> private_data_ ;
};

#endif // CORE_SESSION_H_
