#pragma once
#include <stdio.h>

#define MAXSTR 31

typedef struct elab_s* elaboratori_t;

elaboratori_t elab_init(const char* filename);
void elab_free(elaboratori_t elabs);

void elab_make_list_adj(elaboratori_t elabs);

void elab_print_edges(const FILE* fp, elaboratori_t elabs);

int elab_pair_adj_m(elaboratori_t elabs, const char* a, const char* b, const char* c);
int elab_pair_adj_l(elaboratori_t elabs, const char* a, const char* b, const char* c);