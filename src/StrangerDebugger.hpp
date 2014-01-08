/*
 * StrangerDebugger.hpp
 *
 *  Created on: Dec 17, 2013
 *      Author: baki
 */

#ifndef STRANGERDEBUGGER_HPP_
#define STRANGERDEBUGGER_HPP_

#include <string>
#include <iostream>

#define PRINT_SINK 0
#define DEBUG_ASCII 1

#if DEBUG_ASCII==1
#define DEBUG_AUTO(p) p->toDotAscii(PRINT_SINK)
#else
#define DEBUG_AUTO(p) p->toDot()
#endif


extern const unsigned char DEBUG_LEVEL_WRAPPER;
extern const unsigned char DEBUG_MASK_VALIDATION_PHASE;
extern const unsigned char DEBUG_MASK_1ST_FORWARD;
extern const unsigned char DEBUG_MASK_LENGTH_BACKWARD;
extern const unsigned char DEBUG_MASK_LENGTH_FORWARD;
extern const unsigned char DEBUG_MASK_LAST_BACKWARD;
extern const unsigned char DEBUG_MASK_INITIALIZATION;

extern unsigned char DEBUG_ENABLED_VP;
extern unsigned char DEBUG_ENABLED_1F;
extern unsigned char DEBUG_ENABLED_LENB;
extern unsigned char DEBUG_ENABLED_LENF;
extern unsigned char DEBUG_ENABLED_LASTB;
extern unsigned char DEBUG_ENABLED_INIT;

void DEBUG_MESSAGE(std::string msg);

#endif /* STRANGERDEBUGGER_HPP_ */
