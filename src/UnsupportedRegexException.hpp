// Generated from /testJ2c/src/UnsupportedRegexException.java

#pragma once

#include <fwd-testJ2c.hpp>
#include <string>

class UnsupportedRegexException
{
public:
    UnsupportedRegexException(){throw new std::runtime_error;};
    UnsupportedRegexException(std::string message){throw new std::runtime_error(message);};
};
