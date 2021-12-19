#include "elemento.h"

#include <stdlib.h>

elementi_l elem_l_init(int N) {
	elementi_l lista;
	if (N < 1)
		exit(-1);
	lista.N = N;
	lista.elementi = (elemento_t*)malloc(N * sizeof(elemento_t));

	return lista;
}

elementi_l elem_l_read(const FILE* fp) {
	int N, i;
	elementi_l lista;

	if (fscanf(fp, "%d", &N) != 1)
		exit(-1);

	lista = elem_l_init(N);

	for (i = 0; i < N; i++)
		elem_read(fp, &lista.elementi[i]);

	return lista;
}

void elem_l_free(elementi_l lista) {
	free(lista.elementi);
}


void elem_read(const FILE* fp, elemento_t* elem_p) {
	if (fscanf(fp, "%s %d %d %d %d %d %f %d", &elem_p->nome, &elem_p->tipologia, &elem_p->direzione_ingresso, &elem_p->direzione_uscita, &elem_p->precedenza, &elem_p->finale, &elem_p->valore, &elem_p->difficoltà) != 8)
		exit(-1);
}

void merge(elementi_l v, elementi_l aux, int left, int center, int right) {
	int i = left, j = center + 1, k = 0;

	while (i <= center && j <= right) {
		if (v.elementi[i].difficoltà <= v.elementi[j].difficoltà)
			aux.elementi[k++] = v.elementi[i++];
		else
			aux.elementi[k++] = v.elementi[j++];
	}

	while (i <= center)
		aux.elementi[k++] = v.elementi[i++];

	while (j <= right)
		aux.elementi[k++] = v.elementi[j++];


	for (k = left; k <= right; k++)
		v.elementi[k] = aux.elementi[k - left];
}

void merge_r(elementi_l v, elementi_l aux, int left, int right) {
	int med = (left + right) / 2;
	if (right > left) {
		merge_r(v, aux, left, med);
		merge_r(v, aux, med + 1, right);
		merge(v, aux, left, med, right);
	}
}

void elem_sort(elementi_l elem_l) {
	elementi_l aux;
	aux.N = elem_l.N;

	aux.elementi = (elemento_t*)malloc(aux.N * sizeof(elemento_t));

	merge_r(elem_l, aux, 0, elem_l.N - 1);
	free(aux.elementi);
}

int elem_easiest_of_type(elementi_l elems, int type, int min_i) {
	int i;
	for (i = min_i; i < elems.N; i++)
		if (type == elems.elementi[i].tipologia)
			return i;
	return -1;
}

int elem_easiest_initial(elementi_l elems, int uscita, int min_i) {
	int i;
	for (i = min_i; i < elems.N; i++)
		if (elems.elementi[i].precedenza == 0 && elems.elementi[i].direzione_uscita == uscita)
			return i;
	return -1;
}
