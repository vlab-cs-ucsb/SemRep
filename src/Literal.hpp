/*
 * Literal.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef LITERAL_HPP_
#define LITERAL_HPP_

#include "TacPlace.hpp"
#include <string>

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


#endif /* LITERAL_HPP_ */
