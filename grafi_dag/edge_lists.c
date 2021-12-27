#include "edge_lists.h"
#include <stdlib.h>

struct edge {
	int v, w, weight;
	struct edge* next;
};

struct edge_list_s {
	struct edge* head;
};

struct edge_l_node {
	edgelist_t list;
	struct edge_l_node* next;
};

struct edge_l_list {
	struct edge_l_node* head;
	int N;
};

edgelist_t edgelist_init() {
	edgelist_t list = (struct edge_list_s*)malloc(sizeof(struct edge_list_s));
	list->head = NULL;
	return list;
}

void edgelist_free(edgelist_t elist) {
	struct edge* i, * next;
	for (i = elist->head; i != NULL; i = next) {
		next = i->next;
		free(i);
	}
	free(elist);
}

void edgelist_add_edge(edgelist_t elist, int v, int w, int weight) {
	struct edge* new_node = (struct edge*)malloc(sizeof(struct edge));

	new_node->v = v;
	new_node->w = w;
	new_node->weight = weight;
	new_node->next = elist->head;
	elist->head = new_node;
}

void edgelist_remove_last(edgelist_t elist) {
	struct edge* tmp = elist->head;
	if (tmp != NULL) {
		elist->head = tmp->next;
		free(tmp);
	}
}

int edgelist_weight(edgelist_t elist) {
	int r = 0;
	struct edge* i;
	if (elist == NULL)
		return -1;

	for (i = elist->head; i != NULL; i = i->next)
		r += i->weight;

	return r;
}

void edgelist_update_mat_adj(int** mat_adj, edgelist_t elist) {
	struct edge* i;
	if (elist != NULL)
		for (i = elist->head; i != NULL; i = i->next)
			mat_adj[i->v][i->w] = 0;
}

void edgelist_print(edgelist_t elist, const FILE* fp, st_table table) {
	struct edge* i;
	if (elist != NULL)
		for (i = elist->head; i != NULL; i = i->next)
			fprintf(fp, "%s -> %s (%d)\n", st_search_by_index(table, i->v), st_search_by_index(table, i->w), i->weight);
}

set_edgelist_t set_elist_init() {
	set_edgelist_t elist_set = (struct edge_l_list*)malloc(sizeof(struct edge_l_list));
	elist_set->head = NULL;
	elist_set->N = 0;
	return elist_set;
}

void set_elist_free(set_edgelist_t elist_set) {
	struct edge_l_node* i, * next;
	for (i = elist_set->head; i != NULL; i = next) {
		next = i->next;
		edgelist_free(i->list);
		free(i);
	}
	free(elist_set);
}

void set_elist_add_elist(set_edgelist_t elist_set, edgelist_t elist) {
	struct edge_l_node* new_node = (struct edge_l_node*)malloc(sizeof(struct edge_l_node));
	struct edge* i;

	new_node->list = edgelist_init();
	for (i = elist->head; i != NULL; i = i->next)
		edgelist_add_edge(new_node->list, i->v, i->w, i->weight);


	new_node->next = elist_set->head;
	elist_set->head = new_node;
	elist_set->N++;
}

edgelist_t set_elist_best_elist(set_edgelist_t elist_set) {
	edgelist_t best;
	int max, current;
	struct edge_l_node* i;
	if (elist_set == NULL || elist_set->head == NULL)
		return NULL;

	max = edgelist_weight(elist_set->head->list);
	if (max == -1)
		return NULL;
	best = elist_set->head->list;

	for (i = elist_set->head->next; i != NULL; i = i->next) {
		current = edgelist_weight(i->list);
		if (current > max) {
			best = i->list;
			max = current;
		}
	}

	return best;
}

int set_elist_num_elists(set_edgelist_t elist_set) {
	return elist_set->N;
}

void set_elist_print(set_edgelist_t elist_set, const FILE* fp, st_table table) {
	struct edge_l_node* i;
	int count;
	if (elist_set != NULL) {
		if (set_elist_num_elists(elist_set) == 0)
			fprintf(fp, "Nessun insieme\n\n");
		else {
			for (i = elist_set->head, count = 0; i != NULL; i = i->next, count++) {
				fprintf(fp, "Insieme #%d\n", count + 1);
				edgelist_print(i->list, fp, table);
				fprintf(fp, "\n");
			}
		}
	}
}
