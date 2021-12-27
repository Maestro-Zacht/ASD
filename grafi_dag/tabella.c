#include "tabella.h"
#include <stdlib.h>
#include <string.h>


struct table_s {
	char** tabella;		// implementazione tramite vettore perchè la dimensione del problema non richiede una struttura dati più flessibile come un DAG
	int maxN, N, sorted;
};


st_table st_init(int N) {
	st_table table = (struct table_s*)malloc(sizeof(struct table_s));
	table->maxN = N;
	table->N = 0;
	table->sorted = 0;
	table->tabella = (char**)malloc(N * sizeof(char*));
	return table;
}

st_table st_copy(st_table table) {
	int i;
	st_table new_table = st_init(table->maxN);
	new_table->N = table->N;
	new_table->sorted = table->sorted;
	for (i = 0; i < table->N; i++)
		new_table->tabella[i] = strdup(table->tabella[i]);

	return new_table;
}

void st_free(st_table table) {
	int i;
	for (i = 0; i < table->N; i++)
		free(table->tabella[i]);
	free(table->tabella);
	free(table);
}

const char* st_search_by_index(st_table table, int index) {
	return (index < table->N && index >= 0) ? table->tabella[index] : NULL;
}

int st_search_by_id(st_table table, const char* id) {
	int right = table->N - 1, left = 0, medium, cmp;

	if (!table->sorted)
		st_sort(table);

	while (left <= right) {
		medium = left + (right - left) / 2;
		cmp = strcmp(table->tabella[medium], id);

		if (cmp == 0)
			return medium;
		else if (cmp < 0)
			left = medium + 1;
		else
			right = medium - 1;
	}

	return -1;
}


void merge(char** tab, char** aux, int left, int center, int right) {
	int i = left, j = center + 1, k = 0;

	while (i <= center && j <= right) {
		if (strcmp(tab[i], tab[j]) <= 0)
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

void merge_sort_r(char** tab, char** aux, int left, int right) {
	int med = (left + right) / 2;
	if (right > left) {
		merge_sort_r(tab, aux, left, med);
		merge_sort_r(tab, aux, med + 1, right);
		merge(tab, aux, left, med, right);
	}
}

void st_sort(st_table table) {
	char** aux = (char**)malloc(table->N * sizeof(char*));
	merge_sort_r(table->tabella, aux, 0, table->N - 1);
	free(aux);
	table->sorted = 1;
}

void st_insert(st_table table, const char* id) {
	if (table->N < table->maxN) {
		table->tabella[table->N++] = strdup(id);
		table->sorted = 0;
	}
}





//typedef struct bst_node* link;
//
//struct bst_node {
//	char* id;
//	link p, l, r;
//	int N;
//};
//
//struct table_s {
//	link root, z;
//};
//
//link new_node(const char* id, link p, link r, link l, int N) {
//	link n = (link)malloc(sizeof(struct bst_node));
//	n->l = l;
//	n->r = r;
//	n->p = p;
//	n->N = N;
//	n->id = (id != NULL) ? strdup(id) : NULL;
//	return n;
//}
//
//void node_free(link l) {
//	if (l->id != NULL)
//		free(l->id);
//	free(l);
//}
//
//st_table st_init() {
//	st_table table = (struct table_s*)malloc(sizeof(struct table_s));
//	table->z = new_node(NULL, NULL, NULL, NULL, 0);
//	table->root = table->z;
//	return table;
//}
//
//void st_free_r(link l, link z) {
//	if (l != z) {
//		st_free_r(l->r, z);
//		st_free_r(l->l, z);
//		node_free(l);
//	}
//}
//
//void st_free(st_table table) {
//	if (table != NULL) {
//		st_free_r(table->root, table->z);
//		node_free(table->z);
//		free(table);
//	}
//}
//
//link select_r(link l, int k, link z) {
//	int t;
//	if (l == z)
//		return z;
//	t = l->l->N;
//	if (k < t)
//		return select_r(l->l, k, z);
//	if (k > t)
//		return select_r(l->r, k - t - 1, z);
//	return l;
//}
//
//const char* st_search_by_index(st_table table, int index) {
//	link node = select_r(table->root, index, table->z);
//	return node->id;
//}
//
//
//
//int st_search_by_id(st_table table, const char* id) {
//	//TODO
//}
