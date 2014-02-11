/*
 * TacPlace.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef TACPLACE_HPP_
#define TACPLACE_HPP_

#include <string>

class TacPlace {
public:
	TacPlace(){};
	virtual ~TacPlace(){};
	virtual std::string toString() const = 0;
	virtual bool equals(const TacPlace* place) const = 0;
	virtual TacPlace* clone() const = 0;
};


#endif /* TACPLACE_HPP_ */
