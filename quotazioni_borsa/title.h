#pragma once

#include <stdio.h>

#define MAXSTR 21

typedef char* TTLKEY;

typedef struct title_s* title_t;

title_t title_read(const FILE* fp);
void title_free(title_t title);

TTLKEY title_key(title_t title);