#include "elaboratori.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* nome, * rete;
} node_info;

struct edge {
	int v, weight;
	struct edge* next;
};

typedef struct edge* link;

struct elab_s {
	int** mat_adj, V, E;
	node_info* tab_names;
	link* list_adj;
};

int elab_get_index_lin(elaboratori_t elabs, const char* node_name, int right) {
	int i;
	for (i = 0; i < min(elabs->V, right); i++)
		if (strcmp(node_name, elabs->tab_names[i].nome) == 0)
			return i;
	return -1;
}

int elab_get_index_bin(elaboratori_t elabs, const char* node_name) {
	int right = elabs->V - 1, left = 0, medium, cmp;

	while (left <= right) {
		medium = left + (right - left) / 2;
		cmp = strcmp(elabs->tab_names[medium].nome, node_name);

		if (cmp == 0)
			return medium;
		else if (cmp < 0)
			left = medium + 1;
		else
			right = medium - 1;
	}

	return -1;
}

const char* elab_get_name(elaboratori_t elabs, int index) {
	if (index < elabs->V)
		return elabs->tab_names[index].nome;
	else
		return NULL;
}

const char* elab_get_net(elaboratori_t elabs, int index) {
	if (index < elabs->V)
		return elabs->tab_names[index].rete;
	else
		return NULL;
}

void merge(node_info* tab, node_info* aux, int left, int center, int right) {
	int i = left, j = center + 1, k = 0;

	while (i <= center && j <= right) {
		if (strcmp(tab[i].nome, tab[j].nome) <= 0)
			aux[k++] = tab[i++];
		else
			aux[k++] = tab[j++];
	}

	while (i <= center)
		aux[k++] = tab[i++];

	while (j <= right)
		aux[k++] = tab[j++];


	for (k = left; k <= right; k++)
		tab[k] = aux[k - left];
}

void merge_sort_r(node_info* tab, node_info* aux, int left, int right) {
	int med = (left + right) / 2;
	if (right > left) {
		merge_sort_r(tab, aux, left, med);
		merge_sort_r(tab, aux, med + 1, right);
		merge(tab, aux, left, med, right);
	}
}

void merge_sort_tab_names(elaboratori_t elabs) {
	node_info* aux = (node_info*)malloc(elabs->V * sizeof(node_info));
	merge_sort_r(elabs->tab_names, aux, 0, elabs->V - 1);
	free(aux);
}

//int elab_insert_node(elaboratori_t elabs, const char* node_name) { // funzione di servizio, assume che ci sia già spazio allocato
//	int index = elab_get_index(elabs, node_name);
//	if (index == -1) {
//		index = elabs->V;
//
//	}
//}

elaboratori_t elab_init(const char* filename) {
	int i = 0, weight, w, v;
	char nome1[MAXSTR], nome2[MAXSTR], rete1[MAXSTR], rete2[MAXSTR];
	elaboratori_t elabs = (struct elab_s*)malloc(sizeof(struct elab_s));
	if (elabs == NULL)
		exit(-1);
	FILE* infile = fopen(filename, "r");

	elabs->E = 0;
	elabs->V = 20;
	elabs->tab_names = (node_info*)malloc(elabs->V * sizeof(node_info));
	if (elabs->tab_names == NULL)
		exit(-1);

	while (fscanf(infile, "%s %s %s %s %*d", nome1, rete1, nome2, rete2) == 4) {
		elabs->E++;
		v = elab_get_index_lin(elabs, nome1, i);
		if (v == -1) {
			i++;
			if (i > elabs->V) {
				elabs->V *= 2;
				elabs->tab_names = (node_info*)realloc(elabs->tab_names, elabs->V * sizeof(node_info));
				if (elabs->tab_names == NULL)
					exit(-1);
			}
			elabs->tab_names[i - 1].nome = strdup(nome1);
			elabs->tab_names[i - 1].rete = strdup(rete1);
		}

		w = elab_get_index_lin(elabs, nome2, i);
		if (w == -1) {
			i++;
			if (i > elabs->V) {
				elabs->V *= 2;
				elabs->tab_names = (node_info*)realloc(elabs->tab_names, elabs->V * sizeof(node_info));
				if (elabs->tab_names == NULL)
					exit(-1);
			}
			elabs->tab_names[i - 1].nome = strdup(nome2);
			elabs->tab_names[i - 1].rete = strdup(rete2);
		}
	}

	elabs->V = i;
	elabs->tab_names = (node_info*)realloc(elabs->tab_names, elabs->V * sizeof(node_info));
	if (elabs->tab_names == NULL)
		exit(-1);

	elabs->mat_adj = (int**)malloc(elabs->V * sizeof(int*));
	if (elabs->mat_adj == NULL)
		exit(-1);

	for (i = 0; i < elabs->V; i++) {
		elabs->mat_adj[i] = (int*)calloc(elabs->V, sizeof(int));
		if (elabs->mat_adj[i] == NULL)
			exit(-1);
	}

	fseek(infile, 0, SEEK_SET);
	merge_sort_tab_names(elabs);

	for (i = 0; i < elabs->E; i++) {
		fscanf(infile, "%s %*s %s %*s %d", nome1, nome2, &weight);
		w = elab_get_index_bin(elabs, nome1);
		v = elab_get_index_bin(elabs, nome2);

		if (w == -1 || v == -1)
			exit(-1);
		else if (w != v) {
			elabs->mat_adj[w][v] = weight;
			elabs->mat_adj[v][w] = weight;
		}
	}

	elabs->list_adj = NULL;

	fclose(infile);

	return elabs;
}

void elab_free(elaboratori_t elabs) {
	int i;
	link li, next;
	for (i = 0; i < elabs->V; i++) {
		free(elabs->tab_names[i].nome);
		free(elabs->tab_names[i].rete);
		free(elabs->mat_adj[i]);
		if (elabs->list_adj != NULL) {
			for (li = elabs->list_adj[i]; li != NULL; li = next) {
				next = li->next;
				free(li);
			}
		}
	}
	free(elabs->tab_names);
	free(elabs->mat_adj);
	if (elabs->list_adj != NULL)
		free(elabs->list_adj);
	free(elabs);
}

void elab_make_list_adj(elaboratori_t elabs) {
	int i, j;
	link tmp;
	if (elabs->list_adj == NULL) {
		elabs->list_adj = (link*)malloc(elabs->V * sizeof(link));
		if (elabs->list_adj == NULL)
			exit(-1);

		for (i = 0; i < elabs->V; i++) {
			elabs->list_adj[i] = NULL;
			for (j = 0; j < elabs->V; j++) {
				if (elabs->mat_adj[i][j] != 0) {
					tmp = (link)malloc(sizeof(*tmp));
					if (tmp == NULL)
						exit(-1);

					tmp->v = j;
					tmp->weight = elabs->mat_adj[i][j];
					tmp->next = elabs->list_adj[i];
					elabs->list_adj[i] = tmp;
				}
			}
		}
	}
}

void elab_reverse_print_adj_l(const FILE* fp, elaboratori_t elabs, link list, const char* node_start) {
	if (list != NULL) {
		elab_reverse_print_adj_l(fp, elabs, list->next, node_start);
		fprintf(fp, "- %s ---> %s - %s (%d)\n", node_start, elab_get_name(elabs, list->v), elab_get_net(elabs, list->v), list->weight);
	}
}

void elab_print_edges(const FILE* fp, elaboratori_t elabs) {
	int i, j;
	if (elabs->list_adj == NULL) {
		for (i = 0; i < elabs->V; i++) {
			fprintf(fp, "----------\nNODO: %s - %s\n", elab_get_name(elabs, i), elab_get_net(elabs, i));
			for (j = 0; j < elabs->V; j++)
				if (elabs->mat_adj[i][j] != 0)
					fprintf(fp, "- %s --> %s - %s (%d)\n", elab_get_name(elabs, i), elab_get_name(elabs, j), elab_get_net(elabs, j), elabs->mat_adj[i][j]);
		}
	}
	else {
		for (i = 0; i < elabs->V; i++) {
			fprintf(fp, "----------\nNODO: %s - %s\n", elab_get_name(elabs, i), elab_get_net(elabs, i));
			elab_reverse_print_adj_l(fp, elabs, elabs->list_adj[i], elab_get_name(elabs, i));
		}
	}
}

int elab_pair_adj_m(elaboratori_t elabs, const char* a, const char* b, const char* c) {
	int index_a = elab_get_index_bin(elabs, a), index_b = elab_get_index_bin(elabs, b), index_c = elab_get_index_bin(elabs, c);
	if (index_a != -1 && index_b != -1 && index_c != -1)
		return elabs->mat_adj[index_a][index_b] != 0 && elabs->mat_adj[index_a][index_c] != 0 && elabs->mat_adj[index_b][index_c] != 0;
	else
		return 0;
}

int elab_pair_adj_l(elaboratori_t elabs, const char* a, const char* b, const char* c) {
	link i;
	int index_a = elab_get_index_bin(elabs, a), index_b = elab_get_index_bin(elabs, b), index_c = elab_get_index_bin(elabs, c), found_1, found_2;
	if (index_a != -1 && index_b != -1 && index_c != -1 && index_a != index_b && index_a != index_c && index_b != index_c) {
		found_1 = found_2 = 0; 
		for (i = elabs->list_adj[index_a]; i != NULL && (!found_1 || !found_2); i = i->next) {
			if (i->v == index_b)
				found_1 = 1;
			else if (i->v == index_c)
				found_2 = 1;
		}
		if (!found_1 || !found_2)
			return 0;

		found_2 = 0;
		for (i = elabs->list_adj[index_b]; i != NULL && found_2; i = i->next)
			if (i->v == index_c)
				found_2 = 1;
		if (!found_2)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}
