#pragma once

#include <stdio.h>
#include "edge_lists.h"

#define MAXSTR 31

typedef struct graph_s* graph_t;

graph_t graph_init(const FILE* fp);
void graph_free(graph_t graph);

graph_t graph_remove_edges(graph_t graph, edgelist_t elist);
set_edgelist_t graph_convert_to_dag(graph_t graph);

void graph_print(graph_t graph, const FILE* fp);
void graph_print_edges_list(graph_t graph, set_edgelist_t elist_set, const FILE* fp);
void graph_print_edges(graph_t graph, edgelist_t elist, const FILE* fp);
void dag_print_max_dist(graph_t dag, const FILE* fp);