/*
 * Literal.h
 *
 *  Created on: Aug 19, 2013
 *      Author: muath
 */

#ifndef LITERAL_H_
#define LITERAL_H_

#include "TacPlace.h"
#include "string"

class Literal: public TacPlace {
public:
	Literal(){ literalValue = ""; };
	Literal(std::string literalVal){ this->literalValue = literalVal;};
	Literal(const Literal& other):literalValue(other.literalValue){};
	virtual ~Literal(){};
	std::string toString() const {return this->literalValue;};
    std::string getLiteralValue() const { return this->literalValue;};
	bool equals(const TacPlace* place) const {
		const Literal* o = dynamic_cast<const Literal*>(place);
		if (o == NULL) return false;
		return o->literalValue == this->literalValue;
	};
	Literal* clone() const { return new Literal(*this);};

private:
	std::string literalValue;
};

#endif /* LITERAL_H_ */
