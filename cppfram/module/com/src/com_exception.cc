#include <com_exception.h>

namespace COM{

ComException::ComException(int code):Global::BaseException(code){

}

ComException::ComException(std::exception& e):Global::BaseException(e){

}

ComException::~ComException() throw(){

}

}
