#include "tabella.h"
#include <stdlib.h>
#include <string.h>


struct table_s {
	char** tabella;		// implementazione tramite vettore perchè la dimensione del problema non richiede una struttura dati più flessibile come un BST
	int maxN, N;
};


st_table st_init(int N) {
	st_table table = (struct table_s*)malloc(sizeof(struct table_s));
	table->maxN = N;
	table->N = 0;
	table->tabella = (char**)malloc(N * sizeof(char*));
	return table;
}

st_table st_copy(st_table table) {
	int i;
	st_table new_table = st_init(table->maxN);
	new_table->N = table->N;
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
}

void st_insert(st_table table, const char* id) {
	if (table->N < table->maxN)
		table->tabella[table->N++] = strdup(id);
}