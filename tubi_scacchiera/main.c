#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char colors[2]; // x y se non ruotata, y x se ruotata
	int values[2];	// x y se non ruotata, y x se ruotata
} tessera_t;

typedef struct {
	tessera_t* tessere;
	int N;
} tessere_t;

struct tessera_piazzata {
	int id_tessera;
	int rotazione;
	int predefinita;
};

typedef struct {
	struct tessera_piazzata **tessere;
	int R, C;
} scacchiera_t;

void print_scacchiera(scacchiera_t* scacchiera, tessere_t* tessere) {
	int i, j;
	for (i = 0; i < scacchiera->R; i++) {
		printf("Riga %d:\t\t", i + 1);
		for (j = 0; j < scacchiera->C; j++)
			printf("%c(%d) ", tessere->tessere[scacchiera->tessere[i][j].id_tessera].colors[0 + scacchiera->tessere[i][j].rotazione], tessere->tessere[scacchiera->tessere[i][j].id_tessera].values[0 + scacchiera->tessere[i][j].rotazione]);
		printf("\n");
	}

	for (j = 0; j < scacchiera->C; j++) {
		printf("Colonna %d:\t", j + 1);
		for (i = 0; i < scacchiera->R; i++)
			printf("%c(%d) ", tessere->tessere[scacchiera->tessere[i][j].id_tessera].colors[(1 + scacchiera->tessere[i][j].rotazione) & 1], tessere->tessere[scacchiera->tessere[i][j].id_tessera].values[(1 + scacchiera->tessere[i][j].rotazione) & 1]);
		printf("\n");
	}
}

int valore_scacchiera(scacchiera_t* scacchiera, tessere_t* tessere) {
	int i, j, same_color, value = 0, value_parz;
	for (i = 0; i < scacchiera->R; i++) {
		same_color = 1;
		value_parz = tessere->tessere[scacchiera->tessere[i][0].id_tessera].values[0 + scacchiera->tessere[i][0].rotazione];
		for (j = 1; j < scacchiera->C && same_color; j++) {
			if (tessere->tessere[scacchiera->tessere[i][j].id_tessera].colors[0 + scacchiera->tessere[i][j].rotazione] == tessere->tessere[scacchiera->tessere[i][j - 1].id_tessera].colors[0 + scacchiera->tessere[i][j - 1].rotazione])
				value_parz += tessere->tessere[scacchiera->tessere[i][j].id_tessera].values[0 + scacchiera->tessere[i][j].rotazione];
			else
				same_color = 0;
		}
		if (same_color)
			value += value_parz;
	}

	for (j = 0; j < scacchiera->C; j++) {
		same_color = 1;
		value_parz = tessere->tessere[scacchiera->tessere[0][j].id_tessera].values[(1 + scacchiera->tessere[0][j].rotazione) & 1];
		for (i = 1; i < scacchiera->R && same_color; i++) {
			if (tessere->tessere[scacchiera->tessere[i][j].id_tessera].colors[(1 + scacchiera->tessere[i][j].rotazione) & 1] == tessere->tessere[scacchiera->tessere[i - 1][j].id_tessera].colors[(1 + scacchiera->tessere[i - 1][j].rotazione) & 1])
				value_parz += tessere->tessere[scacchiera->tessere[i][j].id_tessera].values[(1 + scacchiera->tessere[i][j].rotazione) & 1];
			else
				same_color = 0;
		}
		if (same_color)
			value += value_parz;
	}

	return value;
}

void copia_scacchiera(scacchiera_t* target, scacchiera_t* from) {
	int i, j;

	target->R = from->R;
	target->C = from->C;

	for (i = 0; i < from->R; i++)
		for (j = 0; j < from->C; j++)
			target->tessere[i][j] = from->tessere[i][j];
}


int disponi(int riga, int colonna, tessere_t* tessere, scacchiera_t* scacchiera, int* mark, int max_val, scacchiera_t* best) {
	int i, tmp_val;
	if (colonna >= scacchiera->C) {
		riga++;
		colonna = 0;
	}

	if (riga >= scacchiera->R) {
		tmp_val = valore_scacchiera(scacchiera, tessere);
		if (tmp_val > max_val)
			copia_scacchiera(best, scacchiera);
		return tmp_val;
	}

	if (scacchiera->tessere[riga][colonna].predefinita)
		return disponi(riga, colonna + 1, tessere, scacchiera, mark, max_val, best);

	for (i = 0; i < tessere->N; i++) {
		if (mark[i] == 0) {
			mark[i] = 1;
			scacchiera->tessere[riga][colonna].id_tessera = i;
			scacchiera->tessere[riga][colonna].rotazione = 0;
			tmp_val = disponi(riga, colonna + 1, tessere, scacchiera, mark, max_val, best);
			if (tmp_val > max_val)
				max_val = tmp_val;

			scacchiera->tessere[riga][colonna].rotazione = 1;
			tmp_val = disponi(riga, colonna + 1, tessere, scacchiera, mark, max_val, best);
			if (tmp_val > max_val)
				max_val = tmp_val;

			mark[i] = 0;
		}
	}

	return max_val;
}

void fill_scacchiera(scacchiera_t scacchiera, tessere_t tessere) {
	int valore, * mark = (int*)calloc(tessere.N, sizeof(int)), i, j;
	scacchiera_t migliore;
	migliore.R = scacchiera.R;
	migliore.C = scacchiera.C;
	migliore.tessere = (struct tessera_piazzata**)malloc(migliore.R * sizeof(struct tessera_piazzata*));
	if (migliore.tessere == NULL)
		exit(-1);

	for (i = 0; i < migliore.R; i++) {
		migliore.tessere[i] = (struct tessera_piazzata*)malloc(migliore.C * sizeof(struct tessera_piazzata));
		if (migliore.tessere[i] == NULL)
			exit(-1);
	}

	for (i = 0; i < scacchiera.R; i++)
		for (j = 0; j < scacchiera.C; j++)
			if (scacchiera.tessere[i][j].predefinita)
				mark[scacchiera.tessere[i][j].id_tessera] = 1;

	valore = disponi(0, 0, &tessere, &scacchiera, mark, 0, &migliore);

	printf("Valore della scacchiera: %d\n", valore);
	
	print_scacchiera(&migliore, &tessere);
}

int main() {
	tessere_t tessere;
	scacchiera_t scacchiera;
	int i, j, id_tessera, rotazione;
	char inp[20];
	FILE* infile = fopen("tiles.txt", "r");

	fscanf(infile, "%d\n", &tessere.N);
	tessere.tessere = (tessera_t*)malloc(tessere.N * sizeof(tessera_t));
	if (tessere.tessere == NULL)
		exit(-1);

	for (i = 0; i < tessere.N; i++)
		fscanf(infile, "%c %d %c %d\n", &tessere.tessere[i].colors[0], &tessere.tessere[i].values[0], &tessere.tessere[i].colors[1], &tessere.tessere[i].values[1]);

	fclose(infile);

	infile = fopen("board.txt", "r");

	fscanf(infile, "%d %d", &scacchiera.R, &scacchiera.C);
	scacchiera.tessere = (struct tessera_piazzata**)malloc(scacchiera.R * sizeof(struct tessera_piazzata*));
	if (scacchiera.tessere == NULL)
		exit(-1);

	for (i = 0; i < scacchiera.R; i++) {
		scacchiera.tessere[i] = (struct tessera_piazzata*)malloc(scacchiera.C * sizeof(struct tessera_piazzata));
		if (scacchiera.tessere[i] == NULL)
			exit(-1);

		for (j = 0; j < scacchiera.C; j++) {
			fscanf(infile, "%d/%d", &scacchiera.tessere[i][j].id_tessera, &scacchiera.tessere[i][j].rotazione);
			if (scacchiera.tessere[i][j].id_tessera == -1)
				scacchiera.tessere[i][j].predefinita = 0;
			else
				scacchiera.tessere[i][j].predefinita = 1;
		}
	}

	fclose(infile);

	fill_scacchiera(scacchiera, tessere);

	return 0;
}