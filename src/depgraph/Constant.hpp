/*
 * Constant.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef CONSTANT_HPP_
#define CONSTANT_HPP_

#include "TacPlace.hpp"

using namespace std;

class Constant: public TacPlace {
public:
	Constant(){ label = ""; };
	Constant(string label){ this->label = label;};
	Constant(const Constant& other):label(other.label){};
	virtual ~Constant(){};
	string toString() const {return label;};
	bool equals(const TacPlace* place) const {
			const Constant* o = dynamic_cast<const Constant*>(place);
			if ( o == NULL) return false;
			return o->label == this->label;
	};
	Constant* clone() const { return new Constant(*this);};
private:
	string label;
};



#endif /* CONSTANT_HPP_ */
