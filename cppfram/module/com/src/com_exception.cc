#include <com_exception.h>

namespace COM{
std::map<std::string, std::string> ComErrCode = { 
    { "BAD_ARGS", "Bad arguments" },
    { "UNKNOW_OPTNAME", "Unknow optname" },
    { "UNKNOW_RUNMODE", "Unknow run mode" },
    { "UNKNOW_PROTOCOL", "Unknow procotol" },
};

ComException::ComException(std::string errmark):Global::BaseException(errmark){

}

ComException::ComException(std::exception& e):Global::BaseException(e){

}

ComException::~ComException() throw(){

}

}
