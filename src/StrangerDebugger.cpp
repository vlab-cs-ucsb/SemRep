/*
 * StrangerDebugger.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: baki
 */

#include "StrangerDebugger.hpp"

const unsigned char DEBUG_LEVEL_WRAPPER = 0x0;

const unsigned char DEBUG_MASK_VALIDATION_PHASE = 0x1;
const unsigned char DEBUG_MASK_1ST_FORWARD = 0x2;
const unsigned char DEBUG_MASK_LENGTH_BACKWARD = 0x4;
const unsigned char DEBUG_MASK_LENGTH_FORWARD = 0x8;
const unsigned char DEBUG_MASK_LAST_BACKWARD = 0x10;
const unsigned char DEBUG_MASK_INITIALIZATION = 0x20;

unsigned char DEBUG_ENABLED_VP = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_VALIDATION_PHASE;
unsigned char DEBUG_ENABLED_1F = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_1ST_FORWARD;
unsigned char DEBUG_ENABLED_LENB = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_LENGTH_BACKWARD;
unsigned char DEBUG_ENABLED_LENF = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_LENGTH_FORWARD;
unsigned char DEBUG_ENABLED_LASTB = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_LAST_BACKWARD;
unsigned char DEBUG_ENABLED_INIT = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_INITIALIZATION;

void DEBUG_MESSAGE(std::string msg) {
	std::cout << std::endl << "----------->>> DEBUGGER says: " << msg << std::endl;
}



