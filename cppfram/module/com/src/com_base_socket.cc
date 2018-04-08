#include <com_base_socket.h>
#include <com_exception.h>

namespace COM{

/*ComBaseSockArgs*/

ComBaseSockArgs::ComBaseSockArgs(){

}

ComBaseSockArgs::ComBaseSockArgs(SockInfo* info, map<int, SockOpt>* opt){

}

ComBaseSockArgs::~ComBaseSockArgs(){

}

void ComBaseSockArgs::add_opt(SockOpt* opt){

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
