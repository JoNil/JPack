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

#include <stdlib.h>
#include <stdio.h>

#include "jpack.h"

int jpack(char * buf, size_t size, const char * format, ...) {
	(void)buf; (void)size; (void)format;
	return 0;
}

void junpack(const char * buf, size_t size, const char * format, ...) {
	(void)buf; (void)size; (void)format;
}

#ifdef TEST
int main(int argc, char *argv[]) {
	(void)argc; (void)argv;
	printf("Hej");
	return 0;
}
#endif