/*
 * RegExp.hpp
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

#ifndef REGEXP_HPP_
#define REGEXP_HPP_

#include "StrangerAutomaton.hpp"
#include "StringBuilder.hpp"

#include <stdexcept>
#include <stdint.h>

class StrangerAutomaton;

class RegExp
{

public:
	RegExp();
	RegExp(std::string s);
	RegExp(std::string s, int syntax_flags);
	virtual ~RegExp(){};
	/**
	 * Syntax flag, enables intersection (<tt>&amp;</tt>).
	 */
	static int const INTERSECTION = 0x0001;

	/**
	 * Syntax flag, enables complement (<tt>~</tt>).
	 */
	static int const COMPLEMENT = 0x0002;

	/**
	 * Syntax flag, enables empty language (<tt>#</tt>).
	 */
	static int const EMPTY = 0x0004;

	/**
	 * Syntax flag, enables anystring (<tt>@</tt>).
	 */
	static int const ANYSTRING = 0x0008;

	/**
	 * Syntax flag, enables named automata (<tt>&lt;</tt>identifier<tt>&gt;</tt>).
	 */
	static int const AUTOMATON = 0x0010;

	/**
	 * Syntax flag, enables numerical intervals (<tt>&lt;<i>n</i>-<i>m</i>&gt;</tt>).
	 */
	static int const INTERVAL = 0x0020;

	/**
	 * Syntax flag, enables all optional regexp syntax.
	 */
	static int const ALL = 0xffff;

	/**
	 * Syntax flag, enables no optional regexp syntax.
	 */
	static int const NONE = 0x0000;
    enum Kind {
    		REGEXP_UNION,
    		REGEXP_CONCATENATION,
    		REGEXP_INTERSECTION,
    		REGEXP_OPTIONAL,
    		REGEXP_REPEAT_STAR,
    		REGEXP_REPEAT_PLUS,
    		REGEXP_REPEAT_MIN,
    		REGEXP_REPEAT_MINMAX,
    		REGEXP_COMPLEMENT,
    		REGEXP_CHAR,
    		REGEXP_CHAR_RANGE,
    		REGEXP_ANYCHAR,
    		REGEXP_EMPTY,
    		REGEXP_STRING,
    		REGEXP_ANYSTRING,
    		REGEXP_AUTOMATON,
    		REGEXP_INTERVAL
    };

    static void restID();
    Kind kind;
    RegExp* exp1;
    RegExp* exp2;
    std::string s;
    char c;
    int min;
    int max;
    int digits;
    char from, to;
    std::string b;
    int flags;
    std::string::size_type pos;
    void simplify();
    void copy(RegExp* e);
    StrangerAutomaton* toAutomaton();
    std::string toString();
    std::string toStringBuilder(std::string b);
//    virtual void getIdentifiers(::java::util::Set* set);
    static RegExp* makeUnion(RegExp* exp1, RegExp* exp2);
    static RegExp* makeConcatenation(RegExp* exp1, RegExp* exp2);
    static RegExp* makeIntersection(RegExp* exp1, RegExp* exp2);
    static RegExp* makeOptional(RegExp* exp);
    static RegExp* makeRepeatStar(RegExp* exp);
    static RegExp* makeRepeatPlus(RegExp* exp);
    static RegExp* makeRepeat(RegExp* exp, int min);
    static RegExp* makeRepeat(RegExp* exp, int min, int max);
    static RegExp* makeComplement(RegExp* exp);
    static RegExp* makeChar(char c);
    static RegExp* makeCharRange(char from, char to);
    static RegExp* makeAnyChar();
    static RegExp* makeEmpty();
    static RegExp* makeString(std::string s);
    static RegExp* makeAnyString();
    static RegExp* makeAutomaton(std::string s);
    static RegExp* makeInterval(int min, int max, int digits);
    RegExp* parseUnionExp() /* throws(IllegalArgumentException) */;
    RegExp* parseInterExp() /* throws(IllegalArgumentException) */;
    RegExp* parseConcatExp() /* throws(IllegalArgumentException) */;
    RegExp* parseRepeatExp() /* throws(IllegalArgumentException) */;
    RegExp* parseComplExp() /* throws(IllegalArgumentException) */;
    RegExp* parseCharClassExp() /* throws(IllegalArgumentException) */;
    RegExp* parseCharClasses() /* throws(IllegalArgumentException) */;
    RegExp* parseCharClass() /* throws(IllegalArgumentException) */;
    RegExp* parseSimpleExp() /* throws(IllegalArgumentException) */;
    char parseCharExp() /* throws(IllegalArgumentException) */;



private:
    static RegExp* makeString(RegExp* exp1, RegExp* exp2);
    void init(std::string s, int syntax_flags);
    static std::string iToStr(int i);
    static std::string::size_type indexOf(std::string s, char c);
//    static int parseInt(std::string s);
    bool peek(std::string s);
	bool match(char c);
	bool more();
	char next() /* throws(IllegalArgumentException) */;
	bool check(int flag);
	static int id;

};

int to_int(std::string input);


#endif /* REGEXP_HPP_ */
