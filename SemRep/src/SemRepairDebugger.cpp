/*
 * SemRepairDebugger.cpp
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

#include "SemRepairDebugger.hpp"
// see the mask below
const unsigned char DEBUG_LEVEL_WRAPPER = 0x0;

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



