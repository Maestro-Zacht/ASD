#include "programma.h"

#include <stdlib.h>

typedef struct {
	elemento_t* elementi[MAXDIAGELEM];
	int N;
} diagonale_t;

struct programma_s {
	diagonale_t diagonali[NDIAGS];
};

void diag_add_elem(diagonale_t* diag, elemento_t* elemp) {
	if (diag->N < MAXDIAGELEM)
		diag->elementi[diag->N++] = elemp;
}

void diag_remove_last(diagonale_t* diag) {
	if (diag->N > 0)
		diag->N--;
}

float diag_val(diagonale_t* diag, int ultima) {
	int i;
	float r = 0;

	for (i = 0; i < diag->N; i++)
		r += diag->elementi[i]->valore;

	if (ultima && diag->elementi[diag->N - 1]->valore >= 8)
		r *= 1.5;

	return r;
}

int diag_diff(diagonale_t* diag) {
	int i, diff = 0;
	for (i = 0; i < diag->N; i++)
		diff += diag->elementi[i]->difficoltà;
	return diff;
}

void diag_print(const FILE* fp, diagonale_t diag) {
	int i;
	for (i = 0; i < diag.N; i++)
		fprintf(fp, "%s ", diag.elementi[i]->nome);

	fprintf(fp, "\n");
}

int prog_diff(programma_t prog) {
	int i, r = 0;
	for (i = 0; i < NDIAGS; i++)
		r += diag_diff(&prog->diagonali[i]);
	return r;
}

programma_t prog_init() {
	int i;
	programma_t prog = (struct programma_s*)malloc(sizeof(struct programma_s));
	if (prog == NULL)
		exit(-1);

	for (i = 0; i < NDIAGS; i++)
		prog->diagonali[i].N = 0;

	return prog;
}

void prog_free(programma_t prog) {
	free(prog);
}

//float prog_make_r(int n_diag, int pos, elementi_l elementi, programma_t prog, int DD, int DP) {
//
//}

float prog_make(programma_t prog, int DD, int DP, elementi_l elementi) {
	int i, j, k, avanti = 0, indietro = 0;
	elem_sort(elementi);
	do {
		j = elem_easiest_of_type(elementi, 1, 0);
		if(j)
	}
}


void prog_print(const FILE* fp, programma_t prog) {
	int i;
	fprintf(fp, "Difficolta' totale: %d\n", prog_diff(prog));
	for (i = 0; i < NDIAGS; i++) {
		fprintf(fp, "Diagonale %d -> %f (%d)\n", i + 1, diag_val(&prog->diagonali[i], i == NDIAGS - 1), diag_diff(&prog->diagonali[i]));
		diag_print(fp, prog->diagonali[i]);
	}
}

float prog_val(programma_t prog) {
	int i;
	float r = 0;
	for (i = 0; i < NDIAGS; i++)
		r += diag_val(&prog->diagonali[i], i == NDIAGS - 1);
	return r;
}
