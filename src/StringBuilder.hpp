/*
 * StringBuilder.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef STRINGBUILDER_HPP_
#define STRINGBUILDER_HPP_

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


#endif /* STRINGBUILDER_HPP_ */
