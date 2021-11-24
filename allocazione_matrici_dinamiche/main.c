#include <stdio.h>
#include <stdlib.h>

int** malloc2d(int nr, int nc) {
	int** m, i;
	m = (int**)malloc(nr * sizeof(int*));

	if (m == NULL)
		exit(1);

	for (i = 0; i < nr; i++) {
		m[i] = (int*)malloc(nc * sizeof(int));
		if (m[i] == NULL)
			exit(1);
	}

	return m;
}

void free2d(int** m, int nr) {
	int i;
	for (i = 0; i < nr; i++)
		free(m[i]);

	free(m);
}

int** leggiFile(const char* fn, int* nr, int* nc) {
	int** m, i, j;
	FILE* infile = fopen(fn, "r");

	fscanf(infile, "%d %d", nr, nc);
	m = malloc2d(*nr, *nc);

	for (i = 0; i < *nr; i++)
		for (j = 0; j < *nc; j++)
			fscanf(infile, "%d", &m[i][j]);

	fclose(infile);

	return m;
}

void separa(int** mat, int nr, int nc, int** bi, int** ne) {
	int i, j, * bianche, * nere, indice_b = 0, indice_n = 0;

	nere = (int*)malloc(((nr * nc) / 2) * sizeof(int));
	bianche = (int*)malloc((nr * nc - ((nr * nc) / 2)) * sizeof(int)); // l'area meno le nere, nel caso il lato sia dispari
	

	for (i = 0; i < nr; i++) {
		for (j = 0; j < nc; j++) {
			if ((i + j) & 1) { // dispari = casella nera
				nere[indice_n] = mat[i][j];
				indice_n++;
			}
			else { // pari = casella bianca
				bianche[indice_b] = mat[i][j];
				indice_b++;
				
			}
		}
	}

	*bi = bianche;
	*ne = nere;
}

int main() {
	int nr, nc, ** scacchiera = leggiFile("mat.txt", &nr, &nc), * bianche, * nere, i;

	separa(scacchiera, nr, nc, &bianche, &nere);

	printf("Le caselle bianche contengono:\n\n");
	for (i = 0; i < (nr * nc - ((nr * nc) / 2)); i++)
		printf("%d\n", bianche[i]);

	printf("\nLe caselle nere contengono:\n\n");
	for (i = 0; i < (nr * nc) / 2; i++)
		printf("%d\n", nere[i]);

	return 0;
}