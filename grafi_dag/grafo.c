#include "grafo.h"

#include <stdlib.h>
#include <string.h>
#include "tabella.h"


struct graph_s {
	int V;
	st_table tab_names;
	int** mat_adj;
};


graph_t graph_init(const FILE* fp) {
	int i, u, v;
	char nome1[MAXSTR], nome2[MAXSTR];
	graph_t grafo = (struct graph_s*)malloc(sizeof(struct graph_s));
	if (fscanf(fp, "%d", &grafo->V) != 1)
		exit(-1);

	grafo->tab_names = st_init(grafo->V);
	grafo->mat_adj = (int**)malloc(grafo->V * sizeof(int*));
	for (i = 0; i < grafo->V; i++)
		grafo->mat_adj[i] = (int*)calloc(grafo->V, sizeof(int));

	for (i = 0; i < grafo->V; i++) {
		if (fscanf(fp, "%s", nome1) != 1)
			exit(-1);
		st_insert(grafo->tab_names, nome1);
	}

	st_sort(grafo->tab_names);

	while (!feof(fp) && fscanf(fp, "%s %s %d", nome1, nome2, &i) == 3) {
		v = st_search_by_id(grafo->tab_names, nome2);
		u = st_search_by_id(grafo->tab_names, nome1);
		if (u != -1 && v != -1)
			grafo->mat_adj[u][v] = i;
	}

	return grafo;
}

void graph_free(graph_t graph) {
	int i;
	for (i = 0; i < graph->V; i++)
		free(graph->mat_adj[i]);
	st_free(graph->tab_names);
	free(graph->mat_adj);
	free(graph);
}

int cycl_dfs_r(graph_t graph, int w, int* time, int* pre, int* post) {
	int i;
	pre[w] = (*time)++;
	for (i = 0; i < graph->V; i++) {
		if (graph->mat_adj[w][i] != 0) {
			if (pre[i] == -1) {
				if (cycl_dfs_r(graph, i, time, pre, post) == 1)
					return 1;
			}
			else {
				if (post[i] == -1)
					return 1;
			}
		}
	}
	post[w] = (*time)++;
	return 0;
}

int is_acyclic(graph_t graph) {
	int time = 0, * pre, * post, i, res = 0;

	pre = (int*)malloc(graph->V * sizeof(int));
	post = (int*)malloc(graph->V * sizeof(int));
	for (i = 0; i < graph->V; i++)
		pre[i] = post[i] = -1;

	for (i = 0; i < graph->V && !res; i++)
		if (pre[i] == -1)
			res = cycl_dfs_r(graph, i, &time, pre, post);

	free(pre);
	free(post);
	return !res;
}


graph_t graph_remove_edges(graph_t graph, edgelist_t elist) {
	graph_t new_g = (struct graph_s*)malloc(sizeof(struct graph_s));
	int i;
	new_g->V = graph->V;
	new_g->tab_names = st_copy(graph->tab_names);
	new_g->mat_adj = (int**)malloc(new_g->V * sizeof(int*));
	for (i = 0; i < new_g->V; i++) {
		new_g->mat_adj[i] = (int*)malloc(new_g->V * sizeof(int));
		new_g->mat_adj[i] = memcpy(new_g->mat_adj[i], graph->mat_adj[i], new_g->V * sizeof(int));
	}
	
	edgelist_update_mat_adj(new_g->mat_adj, elist);

	return new_g;
}

int check_condition(graph_t graph, edgelist_t elist) {
	graph_t new_g = graph_remove_edges(graph, elist);
	int res = is_acyclic(new_g);
	graph_free(new_g);
	return res;
}

void powerset_r(graph_t graph, int n_to_add, int row, int col, edgelist_t to_remove, set_edgelist_t elist_set) {
	if (n_to_add <= 0) {
		if (check_condition(graph, to_remove))
			set_elist_add_elist(elist_set, to_remove);
	}
	else {
		for (; row < graph->V; row++, col = 0) {
			for (; col < graph->V; col++) {
				if (graph->mat_adj[row][col] != 0) {
					edgelist_add_edge(to_remove, row, col, graph->mat_adj[row][col]);
					powerset_r(graph, n_to_add - 1, row, col + 1, to_remove, elist_set);
					edgelist_remove_last(to_remove);
				}
			}
		}
	}
}


set_edgelist_t graph_convert_to_dag(graph_t graph) {
	set_edgelist_t elist_set = set_elist_init();
	edgelist_t elist = edgelist_init();
	int i;
	if (!check_condition(graph, elist)) {
		for (i = 1; set_elist_num_elists(elist_set) == 0; i++) {
			powerset_r(graph, i, 0, 0, elist, elist_set);
		}
	}
	edgelist_free(elist);
	return elist_set;
}

void ts_dfs_r(graph_t dag, int v, int* sorted, int* pre, int* time) {
	int i;
	pre[v] = 0;
	for (i = 0; i < dag->V; i++)
		if (dag->mat_adj[i][v] != 0)
			if (pre[i] == -1)
				ts_dfs_r(dag, i, sorted, pre, time);

	sorted[(*time)++] = v;
}

int* dag_topological_sort(graph_t dag) {
	int* sorted, time = 0, * pre, i;
	pre = (int*)malloc(dag->V * sizeof(int));
	sorted = (int*)malloc(dag->V * sizeof(int));
	for (i = 0; i < dag->V; i++)
		pre[i] = sorted[i] = -1;

	for (i = 0; i < dag->V; i++)
		if (pre[i] == -1)
			ts_dfs_r(dag, i, sorted, pre, &time);

	free(pre);
	return sorted;
}

int dag_is_source(graph_t dag, int v) {
	int i;
	for (i = 0; i < dag->V; i++)
		if (dag->mat_adj[i][v] != 0)
			return 0;
	return 1;
}

int dag_is_sink(graph_t dag, int v) {
	int i;
	for (i = 0; i < dag->V; i++)
		if (dag->mat_adj[v][i] != 0)
			return 0;
	return 1;
}

void graph_print(graph_t graph, const FILE* fp) {
	int i, j;
	for (i = 0; i < graph->V; i++)
		for (j = 0; j < graph->V; j++)
			if (graph->mat_adj[i][j] != 0)
				fprintf(fp, "%s -> %s (%d)\n", st_search_by_index(graph->tab_names, i), st_search_by_index(graph->tab_names, j), graph->mat_adj[i][j]);
}

void graph_print_edges_list(graph_t graph, set_edgelist_t elist_set, const FILE* fp) {
	set_elist_print(elist_set, fp, graph->tab_names);
}

void graph_print_edges(graph_t graph, edgelist_t elist, const FILE* fp) {
	edgelist_print(elist, fp, graph->tab_names);
}

void dag_print_max_dist(graph_t dag, const FILE* fp) {
	int* topological_sort = dag_topological_sort(dag), i, j, v, w, * distances;
	char* node_start;
	distances = (int*)malloc(dag->V * sizeof(int));
	
	for (i = 0; i < dag->V; i++) {
		if (dag_is_source(dag, topological_sort[i])) {
			fprintf(fp, "--------------------\nCalcolo delle distanze dal nodo sorgente %s\n", st_search_by_index(dag->tab_names, topological_sort[i]));

			for (v = 0; v < dag->V; v++)
				distances[v] = INT_MIN;
			distances[topological_sort[i]] = 0;

			for (j = i; j < dag->V; j++) {
				v = topological_sort[j];
				if (distances[v] != INT_MIN)
					for (w = 0; w < dag->V; w++)
						if (dag->mat_adj[v][w] != 0 && distances[w] < distances[v] + dag->mat_adj[v][w])
							distances[w] = distances[v] + dag->mat_adj[v][w];
			}

			node_start = st_search_by_index(dag->tab_names, topological_sort[i]);
			for (j = 0; j < dag->V; j++) {
				fprintf(fp, "Massima distanza da %s a %s: ", node_start, st_search_by_index(dag->tab_names, j));
				if (distances[j] == INT_MIN)
					fprintf(fp, "non raggiungibile\n");
				else
					fprintf(fp, "%d\n", distances[j]);
			}
		}
	}

	free(topological_sort);
	free(distances);
}