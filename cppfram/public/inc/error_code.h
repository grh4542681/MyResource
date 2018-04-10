#ifndef __RETURN_CODE_H__
#define __RETURN_CODE_H__

#include <array>
#include <vector>
#include <map>
#include <string>

namespace Global{

#define DEFAULT_ERROR    ("PUB_99999")
#define UNKNOW_ERROR    ("PUB_99998")


std::map<std::string, std::string> exception_code = {
    { "PUB_99999", "Default error message" },
    { "PUB_99998", "Unknow error message" }
};

inline const char* exception_message(std::string errcode)
{
    std::map<std::string, std::string>::iterator it;
    it = exception_code.find(errcode);
    if (it != exception_code.end()) {
        return it->second.c_str();
    }
    return NULL;
}

}


#endif
