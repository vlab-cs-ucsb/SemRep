/*
 * StringBuilder.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_
#include <iostream>
#include <string>
#include <sstream>

struct stringbuilder
{
   std::stringstream ss;
   template<typename T>
   stringbuilder & operator << (const T &data)
   {
        ss << data;
        return *this;
   }
   operator std::string() { return ss.str(); }
};

#endif /* STRINGBUILDER_H_ */
