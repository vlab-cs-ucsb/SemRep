/*
 * TacPlace.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef TACPLACE_H_
#define TACPLACE_H_
#include <string>

class TacPlace {
public:
	TacPlace(){};
	virtual ~TacPlace(){};
	virtual std::string toString() const = 0;
	virtual bool equals(const TacPlace* place) const = 0;
	virtual TacPlace* clone() const = 0;
};

#endif /* TACPLACE_H_ */
