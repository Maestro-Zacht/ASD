#pragma once

#include "elemento.h"

#define MAXDIAGELEM 5
#define MINDIAGELEM 1

#define NDIAGS 3

typedef struct programma_s* programma_t;

programma_t prog_init();
void prog_free(programma_t prog);
void prog_make(programma_t prog, int DD, int DP, elementi_l elementi);
void prog_print(const FILE* fp, programma_t prog);
float prog_val(programma_t prog);