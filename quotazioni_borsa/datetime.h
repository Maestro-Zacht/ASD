#pragma once

#include <stdio.h>

typedef struct {
	int hour, min;
} ora_t;

typedef struct {
	int year, month, day;
	ora_t time;
} data_t;

data_t date_read(const FILE* fp);

int datecmp(data_t a, data_t b);
int oracmp(ora_t a, ora_t b);
int dateoracmp(data_t a, data_t b);