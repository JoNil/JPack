/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2013 Jonathan Nilsson                                        */
/* Contact: l.a.jonathan.nilsson@gmail.com                                    */
/*                                                                            */
/*                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a    */
/* copy of this software and associated documentation files (the "Software"), */
/* to deal in the Software without restriction, including without limitation  */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,   */
/* and/or sell copies of the Software, and to permit persons to whom the      */
/* Software is furnished to do so, subject to the following conditions:       */
/*                                                                            */
/* The above copyright notice and this permission notice shall be included    */
/* in all copies or substantial portions of the Software.                     */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    */
/* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING   */
/*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER       */
/*  DEALINGS IN THE SOFTWARE.                                                 */
/*                                                                            */
/******************************************************************************/

#ifndef JPACK_H_
#define JPACK_H_

#include <stdlib.h>

// Packs the data given into the buffer according to the format
// Returns the number of bytes written to buf. If buf was to short it returns
// the number of bytes that would have been written if there was enought space.
// It will return -1 if you gave an invalid format

// Option, description
// <       little-endian (default)
// >       big-endian
// !       network (big)

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

int jpack(uint8_t * buf, size_t size, const char * format, ...);

// Unpacks the buffer according to the format to the addresses provided.
// It will return -1 if you gave an invalid format
int junpack(const uint8_t * buf, size_t size, const char * format, ...);

// Returns the buffer needed to hold the format or -1 on invalid format and
// -2 if the resault is unknown i.e. the format contains a string
int jpack_format_length(const char * format);

#endif // JPACK_H_
