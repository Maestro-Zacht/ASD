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
void date_print(const FILE* fp, data_t data);
void dateora_print(const FILE* fp, data_t data);

int datecmp(data_t a, data_t b);
int oracmp(ora_t a, ora_t b);
int dateoracmp(data_t a, data_t b);

int date_in_range(data_t min, data_t max, data_t cmp);