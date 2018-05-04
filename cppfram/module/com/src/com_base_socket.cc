#include <string.h>

#include <com_log.h>
#include <com_base_socket.h>
#include <com_exception.h>

#include <iostream>

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
   switch(this->sp_sockargs->info.protocol) {
    case COM::PROTOCOL::TCP:
        break;
    case COM::PROTOCOL::UDP:
        break;
    default:
        throw ComException(ComErrCode["UNKNOW_PROTOCOL"]);
   }
}

void ComBaseSocket::_open_c(){

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
            throw ComException(ComErrCode["UNKNOW_RUNMODE"]);
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
