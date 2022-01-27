#include <stdio.h>
#include <stdlib.h>

int** leggiFile(FILE* fp, int* NR, int* NC) {
	int** m, i, j;

	fscanf(fp, "%d %d", NR, NC);
	m = (int**)malloc(*NR * sizeof(int*));

	for (i = 0; i < *NR; i++) {
		m[i] = malloc(*NC * sizeof(int));
		for (j = 0; j < *NC; j++) {
			fscanf(fp, "%d", &m[i][j]);
		}
	}

	return m;
}

int verificaRegione(int** proposta, int** check, int** griglia, int r, int c, int NR, int NC) {
	int i, j, n, l;
	if (griglia[r][c] == 1 && proposta[r][c] == -1) {
		check[r][c] = 1;
		return 1;
	}
	if (griglia[r][c] != 1 && proposta[r][c] != -1) {
		n = proposta[r][c];
		for (i = 1; r + i < NR && proposta[r + i][c] == n; i++);
		l = i;

		if (c + l > NC)
			return 0;

		for (i = 0; i < l; i++) {
			for (j = 0; j < l; j++) {
				if (proposta[r + i][c + j] == n) {
					check[r + i][c + j] = 1;
				}
				else
					return 0;
			}
		}

		for (i = 0; i < NR; i++) {
			for (j = 0; j < NC; j++) {
				if (proposta[i][j] == n && !((i >= r && i < r + l) && (j >= c && j < c + l))) {
					return 0;
				}
			}
		}

		return 1;
	}
	return 0;
}

int verifica2(int** griglia, int** proposta, int NR, int NC) {
	int i, j, correct = 1, ** check = malloc(NR * sizeof(int*));

	for (i = 0; i < NR; i++) {
		check[i] = malloc(NC * sizeof(int));
		for (j = 0; j < NC; j++) {
			check[i][j] = 0;
		}
	}

	for (i = 0; i < NR && correct; i++) {
		for (j = 0; j < NC && correct; j++) {
			if (check[i][j] == 0) {
				if (verificaRegione(proposta, check, griglia, i, j, NR, NC) == 0) {
					correct = 0;
				}
			}
		}
	}

	for (i = 0; i < NR; i++) {
		free(check[i]);
	}

	free(check);

	return correct;
}

int verificaProposta(int** griglia, int NR, int NC) {
	FILE* fp = fopen("proposta.txt", "r");
	int i, j, correct = 1, ** proposta = malloc(NR * sizeof(int*)), ** check = malloc(NR * sizeof(int*));

	for (i = 0; i < NR; i++) {
		check[i] = malloc(NC * sizeof(int));
		proposta[i] = malloc(NC * sizeof(int));
		for (j = 0; j < NC; j++) {
			check[i][j] = 0;
			fscanf(fp, "%d", &proposta[i][j]);
		}
	}
	fclose(fp);

	for (i = 0; i < NR && correct; i++) {
		for (j = 0; j < NC && correct; j++) {
			if (check[i][j] == 0) {
				if (verificaRegione(proposta, check, griglia, i, j, NR, NC) == 0) {
					correct = 0;
				}
			}
		}
	}

	for (i = 0; i < NR; i++) {
		free(check[i]);
		free(proposta[i]);
	}

	free(check);
	free(proposta);

	return correct;
}

int contaRegioni(int** proposta, int NR, int NC) {
	int i, j, n = 0;
	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++) {
			if (proposta[i][j] > n)
				n = proposta[i][j];
		}
	}

	return n + 1;
}

void scriviProposta(int** m, int NR, int NC) {
	int i, j;
	FILE* fp = fopen("proposta.txt", "w");

	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++) {
			fprintf(fp, "%d ", m[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void disp(int** griglia, int r, int c, int** sol, int** best, int NR, int NC) {
	int i, j;

	if (c >= NC) {
		r++;
		c = 0;
	}

	if (r >= NR) {
		if (verifica2(griglia, sol, NR, NC)) {
			if (contaRegioni(sol, NR, NC) < contaRegioni(best, NR, NC)) {
				for (i = 0; i < NR; i++)
					for (j = 0; j < NC; j++)
						best[i][j] = sol[i][j];
			}
		}
	}
	else {
		if (griglia[r][c] == 0) {
			for (i = 0; i < NR * NC; i++) {
				sol[r][c] = i;
				disp(griglia, r, c + 1, sol, best, NR, NC);
			}
		}
		else {
			sol[r][c] = -1;
			disp(griglia, r, c + 1, sol, best, NR, NC);
		}
	}
}

int main() {
	FILE* fp = fopen("griglia.txt", "r");
	int NR, NC, ** griglia = leggiFile(fp, &NR, &NC), ** sol, ** best, i, j;
	fclose(fp);

	sol = malloc(NR * sizeof(int*));
	best = malloc(NR * sizeof(int*));

	for (i = 0; i < NR; i++) {
		sol[i] = malloc(NC * sizeof(int));
		best[i] = malloc(NC * sizeof(int));
		for (j = 0; j < NC; j++)
			best[i][j] = i * NC + j;
	}

	disp(griglia, 0, 0, sol, best, NR, NC);

	printf("La soluzione migliore ha %d regioni\n", contaRegioni(best, NR, NC));
	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++) {
			printf("%d ", best[i][j]);
		}
		printf("\n");
		free(best[i]);
		free(sol[i]);
		free(griglia[i]);
	}

	free(best);
	free(sol);
	free(griglia);

	return 0;
}