#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 256

typedef struct proposta {
	int num;
	char** canzoni;
} proposta;


int leggiFile(const char* fn, proposta** lista) {
	int N, i, j, ncanzoni;
	char titolo[MAXSTR];
	FILE* infile = fopen(fn, "r");

	fscanf(infile, "%d", &N);
	*lista = (proposta*)malloc(N * sizeof(proposta));

	for (i = 0; i < N; i++) {
		fscanf(infile, "%d", &ncanzoni);
		(*lista)[i].canzoni = (char**)malloc(ncanzoni * sizeof(char*));
		(*lista)[i].num = ncanzoni;

		for (j = 0; j < ncanzoni; j++) {
			fscanf(infile, "%s", titolo);
			(*lista)[i].canzoni[j] = strdup(titolo);
		}
	}

	fclose(infile);

	return N;
}


int n_playlist_real(int pos_amico, int* scelte, int n_amici, int cnt, proposta* lista_canzoni) {
	int i;
	if (pos_amico >= n_amici) {
		printf("Playlist creata:\n");
		for (i = 0; i < n_amici; i++)
			printf("%s\n", lista_canzoni[i].canzoni[scelte[i]]);
		printf("\n");
		return cnt + 1;
	}

	for (i = 0; i < lista_canzoni[pos_amico].num; i++) {
		scelte[pos_amico] = i;
		cnt = n_playlist_real(pos_amico + 1, scelte, n_amici, cnt, lista_canzoni);
	}
	return cnt;
}

int n_playlist(int n_amici, proposta* lista_canzoni) {
	int* scelte = (int*)malloc(n_amici * sizeof(int)), res;
	res = n_playlist_real(0, scelte, n_amici, 0, lista_canzoni);
	free(scelte);
	return res;
}


int main() {
	proposta* lista_canzoni;
	int i, j, n_amici, expected_n_playlist = 1;

	n_amici = leggiFile("brani.txt", &lista_canzoni);

	printf("\nSono state create %d playlist\n", n_playlist(n_amici, lista_canzoni));

	for (i = 0; i < n_amici; i++) {
		expected_n_playlist *= lista_canzoni[i].num;
		for (j = 0; j < lista_canzoni[i].num; j++)
			free(lista_canzoni[i].canzoni[j]);
		free(lista_canzoni[i].canzoni);
	}
	free(lista_canzoni);

	printf("Numero di playlist aspettato: %d\n", expected_n_playlist);

	return 0;
}