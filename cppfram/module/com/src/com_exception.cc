#include <com_exception.h>

namespace COM{
std::map<std::string, std::string> ComErrCode = { 
    { "BAD_ARGS", "Bad arguments" }
};

ComException::ComException(std::string errmark):Global::BaseException(errmark){

}

ComException::ComException(std::exception& e):Global::BaseException(e){

}

ComException::~ComException() throw(){

}

}
