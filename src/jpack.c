/******************************************************************************/
/*                                                                            */
/* Copyright (C) 2013 Jonathan Nilsson                                        */
/* Contact: l.a.jonathan.nilsson@gmail.com                                    */
/*               	                                                             */
/* This file is part of JPack.                                                */
/*                                                                            */
/* JPack is free software: you can redistribute it and/or modify              */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation, either version 3 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              */
/* GNU General Public License for more details.                               */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef TEST
#include <stdio.h>
#endif // TEST

#include "jpack.h"

static int is_big_endian(void) {
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };
	return bint.c[0] == 1; 
}

static uint16_t swap_bytes_16(uint16_t num) {
	return
		(num >> 8) |
		(num << 8);
}

static uint32_t swap_bytes_32(uint32_t num) {
	return
		((num >> 24) & 0x000000ff) |
		((num >> 8)  & 0x0000ff00) |
		((num << 8)  & 0x00ff0000) |
		((num << 24) & 0xff000000;
}

static uint64_t swap_bytes_64(uint64_t num) {
	return
		((num >> 56) & 0x00000000000000ff) |
		((num >> 40) & 0x000000000000ff00) |
		((num >> 24) & 0x0000000000ff0000) |
		((num >> 8)  & 0x00000000ff000000) |
		((num << 8)  & 0x000000ff00000000) |
		((num << 24) & 0x0000ff0000000000) |
		((num << 40) & 0x00ff000000000000) |
		((num << 56) & 0xff00000000000000);
}

int jpack(char * buf, size_t size, const char * format, ...) {
	int offset = 0;

	while (*format) {
		switch (*format) {
			case 'x':
			case 'b':
			case 'B':
				length += 1;
				break;
			case 'h':
			case 'H':
				length += 2;
				break;
			case 'i':
			case 'I':
			case 'f':
				length += 4;
				break;
			case 'l':
			case 'L':
			case 'd':
				length += 8;
				break;
			case '<':
			case '>':
			case '!':
				break;
			default:
				return -1;
		}
		format++;
	}
	return 0;
}

void junpack(const char * buf, size_t size, const char * format, ...) {
	uint32_t offset = 0;

	while (*format) {
		switch (*format) {
			case 'x':
			case 'b':
			case 'B':
				length += 1;
				break;
			case 'h':
			case 'H':
				length += 2;
				break;
			case 'i':
			case 'I':
			case 'f':
				length += 4;
				break;
			case 'l':
			case 'L':
			case 'd':
				length += 8;
				break;
			case '<':
			case '>':
			case '!':
				break;
			default:
				return -1;
		}
	}
	format++;
}

int jpack_format_length(const char * format) {
	if (format == NULL) {
		return 0;
	}

	int length = 0;

	while (*format) {
		switch (*format) {
			case 'x':
			case 'b':
			case 'B':
				length += 1;
				break;
			case 'h':
			case 'H':
				length += 2;
				break;
			case 'i':
			case 'I':
			case 'f':
				length += 4;
				break;
			case 'l':
			case 'L':
			case 'd':
				length += 8;
				break;
			case '<':
			case '>':
			case '!':
				break;
			default:
				return -1;
		}
		format++;
	}

	return length;
}

#ifdef TEST
int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
	
	if (jpack_format_length("IiB") != 9) {
		fprintf(stderr, "Failed to get buffer length for format");
		return EXIT_FAILIURE;
	}

	char buffer[9];

	uint32_t a = 1, a2;
	int32_t b = -1, b2;
	uint8_t c = 1, c2;

	if (jpack(buffer, 9, "IiB", a, b, c) < 0) {
		fprintf(stderr, "Failed to pack data");
		return EXIT_FAILIURE;
	}

	junpack(buffer, 9, "IiB", &a2, &b2, &c2);

	if (a != a2 ||
	    b != b2 ||
	    c != c2) {
		fprintf(stderr, "Unpacked data is not the same as packed data");
		return EXIT_FAILIURE;
	}

	return EXIT_SUCCESS;
}
#endif // TEST