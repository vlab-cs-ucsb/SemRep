/*
 * StrangerStringAnalysisException.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef STRANGERSTRINGANALYSISEXCEPTION_HPP_
#define STRANGERSTRINGANALYSISEXCEPTION_HPP_

#include <string>

class StrangerStringAnalysisException {
public:
    StrangerStringAnalysisException ( const std::string err){ error = err;};
    const std::string what() const { return error;};

private:
    std::string error;

};


#endif /* STRANGERSTRINGANALYSISEXCEPTION_HPP_ */
