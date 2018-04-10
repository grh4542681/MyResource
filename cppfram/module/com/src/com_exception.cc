#include <com_exception.h>

namespace COM{

ComException::ComException(std::string errmark):Global::BaseException(errmark){

}

ComException::ComException(std::exception& e):Global::BaseException(e){

}

ComException::~ComException() throw(){

}

}
