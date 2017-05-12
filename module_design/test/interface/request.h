#ifndef INTERFACE_REQUEST_H_
#define INTERFACE_REQUEST_H_

#include <string>
#include <map>

#include "common/global_define.h"

class Request
{
public:
    Request();
    virtual ~Request();


    virtual const std::string& GetAttribute(const std::string& attr_name, const std::string& default_val) const;

    const struct timeval& request_time() const;
    const RequestType     request_type() const;

protected:
    std::string action_;
    struct timeval request_time_;

    std::map<std::string, std::string> attr_;  // URL's attribute
};


inline const struct timeval& Request::request_time() const
{
    return request_time_;
}

inline const std::string& Request::GetAttribute(const std::string& attr_name, const std::string& default_val) const
{
    std::map<std::string, std::string>::const_iterator it = attrs_find(attr_name);
    return (it!=attr_.end())?it->second:default_val;
}

inline const RequestType& Request::request_type() const
{
    return request_type_;
}

#endif // INTERFACE_REQUEST_H_
