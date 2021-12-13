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

int prog_check(programma_t prog) {
	int i, j, avanti = 0, indietro = 0, due_elementi = 0;
	for (i = 0; i < NDIAGS; i++) {
		if (prog->diagonali[i].N < 1)
			return 0;

		if (prog->diagonali[i].N > 1)
			due_elementi = 1;

		for (j = 0; j < prog->diagonali[i].N; j++) {
			if (prog->diagonali[i].elementi[j]->tipologia == 2)
				avanti = 1;
			else if (prog->diagonali[i].elementi[j]->tipologia == 1)
				indietro = 1;
		}
	}

	return avanti && indietro && due_elementi;
}

float prog_make_r(int n_diag, int pos, int direzione_prec, elementi_l elementi, programma_t prog, programma_t tmp_prog, float best_val_prog, int DD, int DP) {
	int i, j;
	float tmp_val, val_prog;
	
	if (n_diag < NDIAGS) {
		if (pos < MAXDIAGELEM) {
			for (i = 0; i < elementi.N; i++) {
				diag_add_elem(&tmp_prog->diagonali[n_diag], &elementi.elementi[i]);
				if (
					elementi.elementi[i].direzione_ingresso == direzione_prec &&
					(pos > 0 || (pos == 0 && elementi.elementi[i].precedenza == 0)) &&
					diag_diff(&tmp_prog->diagonali[n_diag]) <= DD &&
					prog_diff(tmp_prog) <= DP
				) {
					if (!elementi.elementi[i].finale) {
						tmp_val = prog_make_r(n_diag, pos + 1, elementi.elementi[i].direzione_uscita, elementi, prog, tmp_prog, best_val_prog, DD, DP);

						if (tmp_val > best_val_prog)
							best_val_prog = tmp_val;
					}

					tmp_val = prog_make_r(n_diag + 1, 0, 1, elementi, prog, tmp_prog, best_val_prog, DD, DP);
					if (tmp_val > best_val_prog) {
						best_val_prog = tmp_val;
					}
				}
				diag_remove_last(&tmp_prog->diagonali[n_diag]);
			}
		}
		else
			best_val_prog = prog_make_r(n_diag + 1, 0, 1, elementi, prog, tmp_prog, best_val_prog, DD, DP);
	}
	else {
		val_prog = prog_val(tmp_prog);
		if (!prog_check(tmp_prog))
			best_val_prog = -1;
		else if(val_prog > best_val_prog) {
			best_val_prog = val_prog;
			for (i = 0; i < NDIAGS; i++) {
				prog->diagonali[i].N = tmp_prog->diagonali[i].N;
				for (j = 0; j < tmp_prog->diagonali[i].N; j++) {
					prog->diagonali[i].elementi[j] = tmp_prog->diagonali[i].elementi[j];
				}
			}
		}
	}
	return best_val_prog;
}

float prog_make(programma_t prog, int DD, int DP, elementi_l elementi) {
	programma_t tmp = prog_init();
	float r = prog_make_r(0, 0, 1, elementi, prog, tmp, 0, DD, DP);
	prog_free(tmp);
	return r;
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
