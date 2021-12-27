#pragma once

#include <stdio.h>

#include "tabella.h"

typedef struct edge_list_s* edgelist_t;
typedef struct edge_l_list* set_edgelist_t;

edgelist_t edgelist_init();
void edgelist_free(edgelist_t elist);

void edgelist_add_edge(edgelist_t elist, int v, int w, int weight);
void edgelist_remove_last(edgelist_t elist);
int edgelist_weight(edgelist_t elist);
void edgelist_update_mat_adj(int** mat_adj, edgelist_t elist);

void edgelist_print(edgelist_t elist, const FILE* fp, st_table table);

set_edgelist_t set_elist_init();
void set_elist_free(set_edgelist_t elist_set);

void set_elist_add_elist(set_edgelist_t elist_set, edgelist_t elist);
edgelist_t set_elist_best_elist(set_edgelist_t elist_set);
int set_elist_num_elists(set_edgelist_t elist_set);

void set_elist_print(set_edgelist_t elist_set, const FILE* fp, st_table table);