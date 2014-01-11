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
extern const unsigned char DEBUG_MASK_VALIDATION_PATCH;
extern const unsigned char DEBUG_MASK_SINK_COMPUTATION;
extern const unsigned char DEBUG_MASK_LENGTH_PATCH;
extern const unsigned char DEBUG_MASK_SANITIZATION_PATCH;
extern const unsigned char DEBUG_MASK_INITIALIZATION;
extern const unsigned char DEBUG_MASK_RESULTS;

extern unsigned char DEBUG_ENABLED_VP;
extern unsigned char DEBUG_ENABLED_SC;
extern unsigned char DEBUG_ENABLED_LP;
extern unsigned char DEBUG_ENABLED_SP;
extern unsigned char DEBUG_ENABLED_INIT;
extern unsigned char DEBUG_ENABLED_RESULTS;

void DEBUG_MESSAGE(std::string msg);

#endif /* STRANGERDEBUGGER_HPP_ */
