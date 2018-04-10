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
}

void ComBaseSockArgs::add_opt(int optname, int level, const void *optval,unsigned int optvallen) throw(ComException){
    SockOpt new_optval;
    new_optval.optname = optname;
    new_optval.level = level;
    new_optval.optval = NULL;
    if (optval) {
        try{
            new_optval.optval = new char[optvallen];
        } catch (std::exception& e) {
            throw ComException(e);
        }
        memset(const_cast<void*>(new_optval.optval), 0x00, optvallen);
        memcpy(const_cast<void*>(new_optval.optval), optval, optvallen);
    }

    std::map<int, SockOpt>::iterator it = this->opt.find(optname);
    if (it != this->opt.end()) {
        delete[] static_cast<const char*>(it->second.optval);
        this->opt.erase(it);
        this->opt.insert(std::pair<int, SockOpt>(optname, new_optval));
    } else {
        this->opt.insert(std::pair<int, SockOpt>(optname, new_optval));
    }
}

void ComBaseSockArgs::del_opt(SockOpt* opt){
    std::map<int, SockOpt>::iterator it = this->opt.begin();
    while(it != this->opt.end()){
        std::cout<<it->second.optname<<"   "<<it->second.level<<std::endl;
        it++;
    }
}

/*ComBaseSocket*/

ComBaseSocket::ComBaseSocket(){

}

ComBaseSocket::ComBaseSocket(ComBaseSockArgs* sockargs){

}

ComBaseSocket::~ComBaseSocket(){

}

void ComBaseSocket::open() throw(ComException){

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