/*
 * UnsupportedRegexException.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef UNSUPPORTEDREGEXEXCEPTION_HPP_
#define UNSUPPORTEDREGEXEXCEPTION_HPP_

#include <string>

class UnsupportedRegexException
{
public:
    UnsupportedRegexException(){throw new std::runtime_error;};
    UnsupportedRegexException(std::string message){throw new std::runtime_error(message);};
};


#endif /* UNSUPPORTEDREGEXEXCEPTION_HPP_ */
