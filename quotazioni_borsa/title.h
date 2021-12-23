#pragma once

#include <stdio.h>
#include "quotation.h"

#define MAXSTR 21

typedef const char* TTLKEY;

typedef struct title_s* title_t;

title_t title_init(TTLKEY key);

void title_read(const FILE* fp, title_t title);
void title_free(title_t title);

int title_cmp(title_t a, title_t b);
int title_cmp_key(title_t a, TTLKEY key);

TTLKEY title_key(title_t title);
void title_print_quotation(const FILE* fp, title_t title, QTKEY key);
void title_print_quotations_range(const FILE* fp, title_t title, QTKEY key_min, QTKEY key_max);
void title_print_quotations_min_max(const FILE* fp, title_t title);

void title_balance(title_t title, float soglia);