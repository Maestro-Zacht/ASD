#pragma once

#include <stdio.h>

#include "pacco.h"

typedef struct corridoi_s* Magazzino;

Magazzino MAGAZZINOinit(FILE* fp);

void MAGAZZINOaddPacco(Magazzino m, pacco_item data);
void MAGAZZINOremovePacco(Magazzino m, char* pacco);
void MAGAZZINOinsertPacco(Magazzino m, pacco_item data);
void MAGAZZINOmovePacco(Magazzino m, pacco_item data);