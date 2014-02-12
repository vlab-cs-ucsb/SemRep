/*
 * TacPlace.hpp
 *
 * Copyright (C) 2013-2014 University of California Santa Barbara.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335,
 * USA.
 *
 * Authors: Abdulbaki Aydin, Muath Alkhalaf
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
