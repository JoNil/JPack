/*******************************************************************************
 *
 * Copyright (C) 2013 Jonathan Nilsson
 * Contact: l.a.jonathan.nilsson@gmail.com
 *
 * This file is part of JPack.
 *
 * JPack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 ******************************************************************************/

#ifndef JPACK_H_
#define JPACK_H_

#include <stdlib.h>

// Returns the number of bytes written to buf. If buf was to short it returns
// the number of bytes that would have been written if there was enought space

// Format char, type,     length
// x,           padbyte,  1
// b,           int8_t,   1
// B,           uint8_t,  1
// h,           int16_t,  2
// H,           uint16_t, 2
// i,           int32_t,  4
// I,           uint32_t, 4
// l,           int64_t,  8
// L,           uint64_t, 8
// f,           float,    4
// d,           double,   8
// s,           string,   -

int jpack(char * buf, size_t size, const char * format, ...);
void junpack(const char * buf, size_t size, const char * format, ...);

#endif
