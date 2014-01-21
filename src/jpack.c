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
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                                  */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#ifdef TEST
#include <stdio.h>
#endif // TEST

#include "jpack.h"

static int system_big_endian = -1;

static int is_system_big_endian(void) {
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };
	return bint.c[0] == 1;
}

static uint16_t swap_bytes_16(uint16_t num) {
	return
		(uint16_t)(num >> 8) |
		(uint16_t)(num << 8);
}

static uint32_t swap_bytes_32(uint32_t num) {
	return
		((num >> 24) & 0x000000ff) |
		((num >> 8)  & 0x0000ff00) |
		((num << 8)  & 0x00ff0000) |
		((num << 24) & 0xff000000);
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

static size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

int jpack(uint8_t * buf, size_t size, const char * format, ...) {
	if (system_big_endian == -1) {
		system_big_endian = is_system_big_endian();
	}

	size_t offset = 0;
	int next_is_big_endian = 0;
	va_list arg_list;

	va_start(arg_list, format);

	while (*format) {
		switch (*format) {
			case 'x': {
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'b': {
				int8_t val = (int8_t)va_arg(arg_list, int);
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'B': {
				uint8_t val = (uint8_t)va_arg(arg_list, int);
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'h': {
				int16_t val = (int16_t)va_arg(arg_list, int);
				if (next_is_big_endian != system_big_endian) {
					val = (int16_t)swap_bytes_16((uint16_t)val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 2;
				next_is_big_endian = 0;
				break;
			}
			case 'H': {
				uint16_t val = (uint16_t)va_arg(arg_list, int);
				if (next_is_big_endian != system_big_endian) {
					val = swap_bytes_16(val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 2;
				next_is_big_endian = 0;
				break;
			}
			case 'i': {
				int32_t val = va_arg(arg_list, int32_t);
				if (next_is_big_endian != system_big_endian) {
					val = (int32_t)swap_bytes_32((uint32_t)val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'I': {
				uint32_t val = va_arg(arg_list, uint32_t);
				if (next_is_big_endian != system_big_endian) {
					val = swap_bytes_32(val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'f': {
				union { float fval; uint32_t ival; } val;
				val.fval = (float)va_arg(arg_list, double);
				if (next_is_big_endian != system_big_endian) {
					val.ival = swap_bytes_32(val.ival);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val.fval, min(sizeof(val), size - offset));
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'l': {
				int64_t val = va_arg(arg_list, int64_t);
				if (next_is_big_endian != system_big_endian) {
					val = (int64_t)swap_bytes_64((uint64_t)val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 'L': {
				uint64_t val = va_arg(arg_list, uint64_t);
				if (next_is_big_endian != system_big_endian) {
					val = swap_bytes_64(val);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val, min(sizeof(val), size - offset));
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 'd': {
				union { double fval; uint64_t ival; } val;
				val.fval = va_arg(arg_list, double);
				if (next_is_big_endian != system_big_endian) {
					val.ival = swap_bytes_64(val.ival);
				}
				if (size - offset > 0) {
					memcpy(buf + offset, &val.fval, min(sizeof(val), size - offset));
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 's': {
				const char * val = va_arg(arg_list, const char *);
				size_t length = strlen(val) + 1;
				if (size - offset > 0) {
					memcpy(buf + offset, val, min(length, size - offset));
				}
				offset += length;
				next_is_big_endian = 0;
				break;
			}
			case '<': {
				next_is_big_endian = 0;
				break;
			}
			case '>': {
				next_is_big_endian = 1;
				break;
			}
			case '!': {
				next_is_big_endian = 1;
				break;
			}
			default: {
				va_end(arg_list);
				return -1;
			}
		}
		format++;
	}

	va_end(arg_list);

	return (int)offset;
}

int junpack(const uint8_t * buf, size_t size, const char * format, ...) {
	if (system_big_endian == -1) {
		system_big_endian = is_system_big_endian();
	}

	size_t offset = 0;
	int next_is_big_endian = 0;
	va_list arg_list;

	va_start(arg_list, format);

	while (*format) {
		switch (*format) {
			case 'x': {
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'b': {
				int8_t * val = va_arg(arg_list, int8_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'B': {
				uint8_t * val = va_arg(arg_list, uint8_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				offset++;
				next_is_big_endian = 0;
				break;
			}
			case 'h': {
				int16_t * val = va_arg(arg_list, int16_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = (int16_t)swap_bytes_16((uint16_t)*val);
				}
				offset += 2;
				next_is_big_endian = 0;
				break;
			}
			case 'H': {
				uint16_t * val = va_arg(arg_list, uint16_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = swap_bytes_16(*val);
				}
				offset += 2;
				next_is_big_endian = 0;
				break;
			}
			case 'i': {
				int32_t * val = va_arg(arg_list, int32_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = (int32_t)swap_bytes_32((uint32_t)*val);
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'I': {
				uint32_t * val = va_arg(arg_list, uint32_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = swap_bytes_32(*val);
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'f': {
				float * val = va_arg(arg_list, float *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					union { float fval; uint32_t ival; } temp;
					temp.fval = *val;
					temp.ival = swap_bytes_32(temp.ival);
					*val = temp.fval;
				}
				offset += 4;
				next_is_big_endian = 0;
				break;
			}
			case 'l': {
				int64_t * val = va_arg(arg_list, int64_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = (int64_t)swap_bytes_64((uint64_t)*val);
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 'L': {
				uint64_t * val = va_arg(arg_list, uint64_t *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					*val = swap_bytes_64((uint64_t)*val);
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 'd': {
				double * val = va_arg(arg_list, double *);
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(*val), size - offset));
				}
				if (next_is_big_endian != system_big_endian) {
					union { double fval; uint64_t ival; } temp;
					temp.fval = *val;
					temp.ival = swap_bytes_64(temp.ival);
					*val = temp.fval;
				}
				offset += 8;
				next_is_big_endian = 0;
				break;
			}
			case 's': {
				char * val = va_arg(arg_list, char *);
				size_t length = strlen((char *)buf + offset) + 1;
				if (size - offset > 0) {
					memcpy(val, buf + offset, min(sizeof(length), size - offset));
				}
				offset += length;
				next_is_big_endian = 0;
				break;
			}
			case '<': {
				next_is_big_endian = 0;
				break;
			}
			case '>': {
				next_is_big_endian = 1;
				break;
			}
			case '!': {
				next_is_big_endian = 1;
				break;
			}
			default: {
				va_end(arg_list);
				return -1;
			}
		}
		format++;
	}

	va_end(arg_list);

	return (int)offset;
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
			case 's':
				return -2;
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

 	{ // TEST 1
		int length = jpack_format_length("IiB");
		if (length != 9) {
			fprintf(stderr, "IiB format length should be 9 not: %i\n", length);
			return EXIT_FAILURE;
		}
	} fprintf(stderr, "TEST1 Succeeded\n");

	{ // TEST 2
		uint8_t * buffer;

		int8_t a = -1, a2;
		uint8_t b = 2, b2;
		int16_t c = -3, c2;
		uint16_t d = 4, d2;
		int32_t e = -5, e2;
		uint32_t f = 6, f2;
		int64_t g = -7, g2;
		uint64_t h = 8, h2;
		float i = 1.0f, i2;
		double j = 2.0, j2;
		char k[5] = "Test";
		char k2[5];

		size_t length = (size_t)jpack_format_length("bBxhHxiIxlLxfd") + 5;
		buffer = malloc(length);

		if (jpack(buffer, length, "bBxhHxiIxlLxfds", a, b, c, d, e, f, g, h, i, j, k) < 0) {
			fprintf(stderr, "Failed to pack data\n");
			return EXIT_FAILURE;
		}

		junpack(buffer, length, "bBxhHxiIxlLxfds", &a2, &b2, &c2, &d2, &e2, &f2, &g2, &h2, &i2, &j2, k2);

		if (a != a2 ||
				b != b2 ||
				c != c2 ||
				d != d2 ||
				e != e2 ||
				f != f2 ||
				g != g2 ||
				h != h2 ||
				i != i2 ||
				j != j2 ||
				strcmp(k, k2) != 0) {
			fprintf(stderr, "Unpacked data is not the same as packed data\n");
			return EXIT_FAILURE;
		}

		free(buffer);
	} fprintf(stderr, "TEST2 Succeeded\n");

	{ // TEST 3
		uint8_t * buffer;

		int8_t a = -1, a2;
		uint8_t b = 2, b2;
		int16_t c = -3, c2;
		uint16_t d = 4, d2;
		int32_t e = -5, e2;
		uint32_t f = 6, f2;
		int64_t g = -7, g2;
		uint64_t h = 8, h2;
		float i = 1.0f, i2;
		double j = 2.0, j2;
		char k[5] = "Test";
		char k2[5];

		size_t length = (size_t)jpack_format_length("!b!Bx!h!Hx!i!Ix!l!Lx!f!d") + 5;
		buffer = malloc(length);

		if (jpack(buffer, length, "!b!Bx!h!Hx!i!Ix!l!Lx!f!ds", a, b, c, d, e, f, g, h, i, j, k) < 0) {
			fprintf(stderr, "Failed to pack data\n");
			return EXIT_FAILURE;
		}

		junpack(buffer, length, "!b!Bx!h!Hx!i!Ix!l!Lx!f!ds", &a2, &b2, &c2, &d2, &e2, &f2, &g2, &h2, &i2, &j2, k2);

		if (a != a2 ||
				b != b2 ||
				c != c2 ||
				d != d2 ||
				e != e2 ||
				f != f2 ||
				g != g2 ||
				h != h2 ||
				i != i2 ||
				j != j2 ||
				strcmp(k, k2) != 0) {
			fprintf(stderr, "Unpacked data is not the same as packed data\n");
			return EXIT_FAILURE;
		}

		free(buffer);
	} fprintf(stderr, "TEST3 Succeeded\n");

	return EXIT_SUCCESS;
}
#endif // TEST
