#pragma once

#include <stdio.h>
#include "title.h"

typedef struct titles_s* titles_l;

titles_l titles_init();
void titles_read(const FILE* fp, titles_l titles);
void titles_free(titles_l titles);

title_t titles_search(titles_l titles, TTLKEY title_cod);