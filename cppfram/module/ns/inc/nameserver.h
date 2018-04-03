#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include <string>
#include <list>

namespace NS{

#define LOCAL_DOMAIN    0
#define REMOTE_DOMAIN   1

typedef struct _endpoint{

}ENDPOINT;

typedef struct _domain{
    int type;
    std::map<std::string, ENDPOINT> ep_map;
}DOMAIN;

class nameserver {
private:
    std::map<std::string, DOMAIN> domain_map;
}

}
