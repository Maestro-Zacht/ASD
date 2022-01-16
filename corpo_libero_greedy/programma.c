#include "programma.h"

#include <stdlib.h>

#define DBG 1

typedef struct {
	elemento_t elementi[MAXDIAGELEM];
	int N;
} diagonale_t;

struct programma_s {
	diagonale_t diagonali[NDIAGS];
};

void diag_add_elem(diagonale_t* diag, elemento_t* elemp) {
#if DBG
	if (elemp->conv <= 0) {
		printf("Convenienza nulla!!\n");
		exit(-1);
	}
#endif
	if (diag->N < MAXDIAGELEM)
		diag->elementi[diag->N++] = *elemp;
}


float diag_val(diagonale_t* diag, int ultima) {
	int i;
	float r = 0;

	for (i = 0; i < diag->N; i++)
		r += diag->elementi[i].valore;

	if (ultima && diag->elementi[diag->N - 1].valore >= 8)
		r *= 1.5;

	return r;
}

int diag_diff(diagonale_t* diag) {
	int i, diff = 0;
	for (i = 0; i < diag->N; i++)
		diff += diag->elementi[i].difficoltà;
	return diff;
}

void diag_print(const FILE* fp, diagonale_t diag) {
	int i;
	for (i = 0; i < diag.N; i++)
		fprintf(fp, "%s ", diag.elementi[i].nome);

	fprintf(fp, "\n");
}

int diag_check_acr(diagonale_t diag) {
	int i;
	for (i = 0; i < diag.N; i++)
		if (diag.elementi[i].tipologia == 1 || diag.elementi[i].tipologia == 2)
			return 1;
	return 0;
}

int prog_check_tipo(programma_t prog, int tipo) {
	int i, j;
	for (i = 0; i < NDIAGS; i++)
		for (j = 0; j < prog->diagonali[i].N; j++)
			if (prog->diagonali[i].elementi[j].tipologia == tipo)
				return 1;
	return 0;
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


void prog_make(programma_t prog, int DD, int DP, elementi_l elementi) {
	int inserito, i;
	elem_sort(elementi, 2, 1, -1, min(DD, DP / 3), 1, 0, 1);
	if (elementi.elementi[0].conv > 0) {
		diag_add_elem(&prog->diagonali[0], &elementi.elementi[0]);
	}
	else {
		elem_sort(elementi, -1, 1, -1, min(DD, DP / 3), 1, 0, 1);
		diag_add_elem(&prog->diagonali[0], &elementi.elementi[0]);
	}

	if (prog_check_tipo(prog, 2)) {
		elem_sort(elementi, 1, 1, -1, min(DD, DP / 3), 1, 0, 1);
		if (elementi.elementi[0].conv > 0) {
			diag_add_elem(&prog->diagonali[1], &elementi.elementi[0]);
		}
		else {
			elem_sort(elementi, -1, 1, -1, min(DD, DP / 3), 1, 0, 1);
			diag_add_elem(&prog->diagonali[1], &elementi.elementi[0]);
		}
	}
	else {
		elem_sort(elementi, -1, 1, -1, min(DD, DP / 3)/2, 1, 0, 1);
		diag_add_elem(&prog->diagonali[1], &elementi.elementi[0]);
	}

	elem_sort(elementi, 1, 1, -1, min(DD, DP / 3) / 2, 1, 0, 1);
	if (elementi.elementi[0].conv > 0)
		diag_add_elem(&prog->diagonali[2], &elementi.elementi[0]);
	else {
		elem_sort(elementi, 2, 1, -1, min(DD, DP / 3) / 2, 1, 0, 1);
		if (elementi.elementi[0].conv > 0)
			diag_add_elem(&prog->diagonali[2], &elementi.elementi[0]);
		else {
			elem_sort(elementi, -1, 1, -1, min(DD, DP / 3) / 2, 1, 0, 1);
			diag_add_elem(&prog->diagonali[2], &elementi.elementi[0]);
		}
	}

	inserito = 0;
	if (!prog_check_tipo(prog, 1)) {
		for (i = 0; i < NDIAGS && !inserito; i++) {
			elem_sort(elementi, 1, prog->diagonali[i].elementi[0].direzione_uscita, -1, min(DD - diag_diff(&prog->diagonali[i]), DP - prog_diff(prog)), 0, -1, 0);
			if (elementi.elementi[0].conv > 0) {
				diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
				inserito = 1;
			}
		}
		if (!inserito) {
			for (i = 0; i < NDIAGS && !inserito; i++) {
				elem_sort(elementi, 0, prog->diagonali[i].elementi[0].direzione_uscita, -1, min(DD - diag_diff(&prog->diagonali[i]), DP - prog_diff(prog)), 0, -1, 0);
				if (elementi.elementi[0].conv > 0) {
					diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
					inserito = 1;
				}
			}
			elem_sort(elementi, 1, prog->diagonali[i].elementi[0].direzione_uscita, -1, min(DD - diag_diff(&prog->diagonali[i]), DP - prog_diff(prog)), 0, -1, 0);
			if (elementi.elementi[0].conv > 0) {
				diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
			}
			else {
				printf("Non riuscito ad inserire indietro!");
				exit(-1);
			}
		}
	}

	if (!prog_check_tipo(prog, 2)) {
		for (i = 0; i < NDIAGS && !inserito; i++) {
			elem_sort(elementi, 2, prog->diagonali[i].elementi[0].direzione_uscita, -1, min(DD - diag_diff(&prog->diagonali[i]), DP - prog_diff(prog)), 0, -1, 0);
			if (elementi.elementi[0].conv > 0) {
				diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
				inserito = 1;
			}
		}
		if (!inserito) {
			printf("Non riuscito ad inserire avanti!");
			exit(-1);
		}
	}

	for (i = 0; i < NDIAGS; i++) {
		if (!diag_check_acr(prog->diagonali[i])) {
			elem_sort(elementi, 1, prog->diagonali[i].elementi[prog->diagonali[i].N - 1].direzione_uscita, -1, min(DP - prog_diff(prog), DD - diag_diff(&prog->diagonali[i])), 0, 0, 0);
			if (elementi.elementi[0].conv > 0)
				diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
			else {
				elem_sort(elementi, 2, prog->diagonali[i].elementi[prog->diagonali[i].N - 1].direzione_uscita, -1, min(DP - prog_diff(prog), DD - diag_diff(&prog->diagonali[i])), 0, 0, 0);
				if (elementi.elementi[0].conv > 0)
					diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
				else {
					printf("Non riuscito ad inserire acrobatico!");
					exit(-1);
				}
			}
		}
	}

	do {
		inserito = 0;
		for (i = 0; i < NDIAGS; i++) {
			elem_sort(elementi, -1, prog->diagonali[i].elementi[0].direzione_uscita, -1, min(DD - diag_diff(&prog->diagonali[i]), DP - prog_diff(prog)), 0, -1, 0);
			if (elementi.elementi[0].conv > 0) {
				diag_add_elem(&prog->diagonali[i], &elementi.elementi[0]);
				inserito = 1;
			}
		}
	} while (inserito);
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
