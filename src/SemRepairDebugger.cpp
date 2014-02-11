/*
 * SemRepairDebugger.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: baki
 */

#include "SemRepairDebugger.hpp"

const unsigned char DEBUG_LEVEL_WRAPPER = 0x20;

const unsigned char DEBUG_MASK_VALIDATION_PATCH = 0x1;
const unsigned char DEBUG_MASK_SINK_COMPUTATION = 0x2;
const unsigned char DEBUG_MASK_LENGTH_PATCH = 0x4;
const unsigned char DEBUG_MASK_SANITIZATION_PATCH = 0x8;
const unsigned char DEBUG_MASK_INITIALIZATION = 0x10;
const unsigned char DEBUG_MASK_RESULTS = 0x20;

unsigned char DEBUG_ENABLED_VP = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_VALIDATION_PATCH;
unsigned char DEBUG_ENABLED_SC = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_SINK_COMPUTATION;
unsigned char DEBUG_ENABLED_LP = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_LENGTH_PATCH;
unsigned char DEBUG_ENABLED_SP = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_SANITIZATION_PATCH;
unsigned char DEBUG_ENABLED_INIT = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_INITIALIZATION;
unsigned char DEBUG_ENABLED_RESULTS = DEBUG_LEVEL_WRAPPER & DEBUG_MASK_RESULTS;

void DEBUG_MESSAGE(std::string msg) {
	std::cout << std::endl << "----------->>> DEBUGGER says: " << msg << std::endl;
}



