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
	int i, flag = 0;
	float r = 0;

	for (i = 0; i < diag->N; i++) {
		r += diag->elementi[i]->valore;
		if (diag->elementi[i]->valore >= 8)
			flag = 1;
	}

	if (ultima && flag)
		r *= 1.5;

	return r;
}

void diag_print(const FILE* fp, diagonale_t diag) {
	int i;
	for (i = 0; i < diag.N; i++)
		fprintf(fp, "%s ", diag.elementi[i]->nome);

	fprintf(fp, "\n");
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

float prog_make_r(int n_diag, int pos, int direzione_prec, elementi_l elementi, programma_t prog, diagonale_t tmp_diag, float best_val_prog, float val_prog, float val_diag, int DD, int DP) {
	int i;
	float tmp_val, new_diag_val;
	
	if (n_diag < NDIAGS) {
		if (pos < MAXDIAGELEM) {
			if (pos == 0)
				tmp_diag.N = 0;
			for (i = 0; i < elementi.N; i++) {
				if (
					elementi.elementi[i].direzione_ingresso == direzione_prec &&
					((pos == 0 && elementi.elementi[i].precedenza == 0) || (pos > 0 && elementi.elementi[i].precedenza == 1)) && 
					elementi.elementi[i].valore + val_diag < DD && 
					elementi.elementi[i].valore + val_prog < DP
				) {
					diag_add_elem(&tmp_diag, &elementi.elementi[i]);
					new_diag_val = diag_val(&tmp_diag, n_diag == NDIAGS - 1);
					tmp_val = new_diag_val - val_diag;
					if (elementi.elementi[i].finale)
						tmp_val = prog_make_r(n_diag + 1, 0, 1, elementi, prog, tmp_diag, best_val_prog, val_prog + tmp_val, 0, DD, DP);
					else
						tmp_val = prog_make_r(n_diag, pos + 1, elementi.elementi[i].direzione_uscita, elementi, prog, tmp_diag, best_val_prog, val_prog + tmp_val, new_diag_val, DD, DP);

					if (tmp_val > best_val_prog) {
						best_val_prog = tmp_val;
						if (pos == MAXDIAGELEM - 1 || elementi.elementi[i].finale)
							prog->diagonali[n_diag] = tmp_diag;
					}
					diag_remove_last(&tmp_diag);
				}
			}
		}
		else
			best_val_prog = prog_make_r(n_diag + 1, 0, 1, elementi, prog, tmp_diag, best_val_prog, val_prog, 0, DD, DP);
	}
	else if (!prog_check(prog))
		best_val_prog = -1;

	return best_val_prog;
}

float prog_make(programma_t prog, int DD, int DP, elementi_l elementi) {
	diagonale_t tmp;
	return prog_make_r(0, 0, 1, elementi, prog, tmp, 0, 0, 0, DD, DP);
}

void prog_print(const FILE* fp, programma_t prog) {
	int i;
	for (i = 0; i < NDIAGS; i++) {
		fprintf(fp, "Diagonale %d -> %f\n", i + 1, diag_val(&prog->diagonali[i], i == NDIAGS - 1));
		diag_print(fp, prog->diagonali[i]);
	}
}
