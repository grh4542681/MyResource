#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>

#include <com_log.h>
#include <com_base_socket.h>
#include <com_exception.h>

#include <iostream>

#define COM_MAX_LINK (100)

namespace COM{

/*ComBaseSockArgs*/

ComBaseSockArgs::ComBaseSockArgs(SockInfo* info){
    this->info = *info;
}

ComBaseSockArgs::~ComBaseSockArgs(){
    std::map<int, SockOpt>::iterator it = this->opt.begin();
    while(it != this->opt.end()){
        delete[] static_cast<const char*>(it->second.optval);
        this->opt.erase(it);
        it++;
    }
}

void ComBaseSockArgs::add_opt(int optname, int level, const void *optval,unsigned int optvallen) throw(ComException){
    if (!optval) {
        throw ComException(ComErrCode["BAD_ARGS"]);
    }
    SockOpt new_optval;
    new_optval.optname = optname;
    new_optval.level = level;
    new_optval.optval = NULL;

    try{
        new_optval.optval = new char[optvallen];
    } catch (std::exception& e) {
        throw ComException(e);
    }
    memset(const_cast<void*>(new_optval.optval), 0x00, optvallen);
    memcpy(const_cast<void*>(new_optval.optval), optval, optvallen);

    std::map<int, SockOpt>::iterator it = this->opt.find(optname);
    if (it != this->opt.end()) {
        delete[] static_cast<const char*>(it->second.optval);
        this->opt.erase(it);
        this->opt.insert(std::pair<int, SockOpt>(optname, new_optval));
    } else {
        this->opt.insert(std::pair<int, SockOpt>(optname, new_optval));
    }
}

void ComBaseSockArgs::del_opt(int optname) throw(ComException){
    std::map<int, SockOpt>::iterator it = this->opt.begin();
    while(it != this->opt.end()){
        if (it->first == optname) {
            delete[] static_cast<const char*>(it->second.optval);
            this->opt.erase(it);
            return;
        }
        it++;
    }
    throw ComException(ComErrCode["UNKNOW_OPTNAME"]);
}

void ComBaseSockArgs::print_opt(void) noexcept{
    std::map<int, SockOpt>::iterator it = this->opt.begin();
    while(it != this->opt.end()){
        std::cout<<it->second.optname<<"   "<<it->second.level<<"     "<<*((int*)(it->second.optval))<<std::endl;
        it++;
    }
}

/*ComBaseSocket*/

ComBaseSocket::ComBaseSocket(){

}

ComBaseSocket::ComBaseSocket(SockInfo* info){
    this->sp_sockargs = std::make_shared<ComBaseSockArgs>(info);
}

ComBaseSocket::~ComBaseSocket(){

}

void ComBaseSocket::_open_s(){
    int ret = 0;

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->sp_sockargs->info.port);
    addr.sin_addr.s_addr = inet_addr((this->sp_sockargs->info.addr).c_str());

    switch(this->sp_sockargs->info.protocol) {
        case COM::PROTOCOL::TCP:
            this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (this->sock_fd < 0) {
                throw ComException(ComErrCode["SOCK_INI_ERR"], ComErrMark::SOCK_INI_ERR);
            }
            ret = bind(this->sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
            if (ret < 0) {
                throw ComException(ComErrCode["BIND_INI_ERR"], ComErrMark::BIND_INI_ERR);
            }
            ret = listen(this->sock_fd, COM_MAX_LINK);
            if (ret < 0) {
                throw ComException(ComErrCode["LISTEN_INI_ERR"], ComErrMark::LISTEN_INI_ERR);
            }
            this->status = STATUS::S_READY;
            break;
        case COM::PROTOCOL::UDP:
            break;
        default:
            throw ComException(ComErrCode["UNKNOW_PROTOCOL"], ComErrMark::UNKNOW_PROTOCOL);
    }
}

void ComBaseSocket::_open_c(){
    switch(this->sp_sockargs->info.protocol) {
        case COM::PROTOCOL::TCP:
            break;
        case COM::PROTOCOL::UDP:
            break;
        default:
            throw ComException(ComErrCode["UNKNOW_PROTOCOL"], ComErrMark::UNKNOW_PROTOCOL);
    }
}

void ComBaseSocket::open() throw(ComException){
    switch (this->sp_sockargs->info.mode) {
        case COM::MODE::SERVER:
            this->_open_s();
            break;
        case COM::MODE::CLIENT:
            this->_open_c();
            break;
        default:
            throw ComException(ComErrCode["UNKNOW_RUNMODE"], ComErrMark::UNKNOW_RUNMODE);
    }
}

void ComBaseSocket::close() throw(ComException){

}

void ComBaseSocket::write() throw(ComException){

}

void ComBaseSocket::read() throw(ComException){

}

void ComBaseSocket::change_opt(SockOpt* opt) throw(ComException){

}

}
