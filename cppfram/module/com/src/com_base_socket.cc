#include <string.h>

#include <com_base_socket.h>
#include <com_exception.h>

namespace COM{

/*ComBaseSockArgs*/

ComBaseSockArgs::ComBaseSockArgs(SockInfo* info){
    memcpy(&(this->info), info, sizeof(SockInfo));
}

ComBaseSockArgs::~ComBaseSockArgs(){

}

void ComBaseSockArgs::add_opt(int optname, int level, const void *optval){
    std::map<int, SockOpt>::iterator it = this->opt.find(optname);
    if (it != this->opt.end()) {

        SockOpt new_optval;
        new_optval.optname = optname;
        new_optval.level = level;
        try{
            new_optval.optval = new char[sizeof(optval)];
        } catch (std::exception& e) {
            throw ComException(e);
        }
        memset(&new_optval, 0x00, sizeof(optval));
        memcpy(&new_optval, optval, sizeof(optval));

        delete[] static_cast<const char*>(it->second.optval);
        this->opt.erase(it);
        
        this->opt.insert(std::pair<int, SockOpt>(optname, new_optval));
    }
}

void ComBaseSockArgs::del_opt(SockOpt* opt){

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
