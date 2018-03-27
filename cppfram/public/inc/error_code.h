#ifndef __RETURN_CODE_H__
#define __RETURN_CODE_H__

#include <map>
#include <string>

namespace Global{

#define DEFAULT_ERROR    (99999)
#define UNKNOW_ERROR    (99998)


std::map<int, std::string> exception_code = {
    { 99999, "DEFAULT_ERROR" },
    { 99998, "UNKNOW_ERROR" }
};

inline const char* exception_message(int code)
{
    std::map<int, std::string>::iterator it;
    it = exception_code.find(code);
    if (it != exception_code.end()) {
        return it->second.c_str();
    }
    return NULL;
}

}


#endif
