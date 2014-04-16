/*
 * SemRepairDebugger.hpp
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

#ifndef SEMREPAIRDEBUGGER_HPP_
#define SEMREPAIRDEBUGGER_HPP_

#include <string>
#include <iostream>

#define PRINT_SINK 0
#define DEBUG_ASCII 1

#if DEBUG_ASCII==1
#define DEBUG_AUTO(p) p->toDotAscii(PRINT_SINK)
#define DEBUG_AUTO_TO_FILE(p, file_name) p->toDotFileAscii(file_name, PRINT_SINK)
#else
#define DEBUG_AUTO(p) p->toDot()
#define DEBUG_AUTO_TO_FILE(p, file_name) p->toDotFile(file_name)
#endif

#define DEBUG_AUTO_TO_FILE_MN(p, file_name) p->toDotFile(file_name)

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

#endif /* SEMREPAIRDEBUGGER_HPP_ */
