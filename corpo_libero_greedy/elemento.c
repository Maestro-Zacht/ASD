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
		if (v.elementi[i].conv >= v.elementi[j].conv)
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

void change_convenience(elemento_t* elemp, int tipologia, int ingresso, int uscita, int diff_cap, int iniziale, int finale, int diff_boost, int acr) {
	elemp->conv = (diff_boost) ? elemp->valore / (elemp->difficoltà * elemp->difficoltà * elemp->difficoltà * elemp->difficoltà * elemp->difficoltà) : elemp->valore / elemp->difficoltà;

	if (
		tipologia != -1 && tipologia != elemp->tipologia ||
		ingresso != -1 && ingresso != elemp->direzione_ingresso ||
		elemp->difficoltà > diff_cap ||
		iniziale && elemp->precedenza != 0 ||
		!finale && elemp->finale == 1 ||
		(uscita != -1) && uscita != elemp->direzione_uscita ||
		acr && elemp->tipologia == 0
		)
		elemp->conv = 0;
}

void elem_sort(elementi_l elem_l, int tipologia, int ingresso, int uscita, int diff_cap, int iniziale, int finale, int diff_boost, int acr) {
	elementi_l aux;
	int i;
	aux.N = elem_l.N;

	aux.elementi = (elemento_t*)malloc(aux.N * sizeof(elemento_t));

	for (i = 0; i < elem_l.N; i++) 
		change_convenience(&elem_l.elementi[i], tipologia, ingresso, uscita, diff_cap, iniziale, finale, diff_boost, acr);

	merge_r(elem_l, aux, 0, elem_l.N - 1);
	free(aux.elementi);
}

//int elem_easiest_of_type(elementi_l elems, int type, int min_i) {
//	int i;
//	for (i = min_i; i < elems.N; i++)
//		if (type == elems.elementi[i].tipologia)
//			return i;
//	return -1;
//}
//
int elem_easiest_initial(elementi_l elems) {
	int i, min = INT_MAX, i_min = -1;
	for (i = 0; i < elems.N; i++) {
		if (elems.elementi[i].precedenza == 0 && elems.elementi[i].difficoltà < min) {
			min = elems.elementi[i].difficoltà;
			i_min = i;
		}
	}
	if (i_min != -1)
		elems.elementi[i_min].conv = INT_MAX;
	return i_min;
}
