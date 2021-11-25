#include <stdio.h>
#include <stdlib.h>

#define FILENAME "att.txt"

typedef struct {
	int s, f;
} att;

typedef struct {
	int* v;
	int durata;
	int len;
} atts;

int check_intersection(att* elenco, att a, int* nodi, int N) {
	int i;

	for (i = 0; i < N; i++)
		if (elenco[nodi[i]].s < a.f && a.s < elenco[nodi[i]].f)
			return 1;

	return 0;
}


void next_att(int start, atts* current, atts* best, att* v, int N) {
	int i, j;

	if (current->durata > best->durata) {
		for (j = 0; j < current->len; j++)
			best->v[j] = current->v[j];
		best->durata = current->durata;
		best->len = current->len;
	}

	for (i = start; i < N; i++) {
		if (!check_intersection(v, v[i], current->v, current->len)) {
			current->v[current->len++] = i;
			current->durata += v[i].f - v[i].s;
			next_att(i + 1, current, best, v, N);
			current->len--;
			current->durata -= v[i].f - v[i].s;
		}
	}
}


void attSel(int N, att* v) {
	int i;
	atts current, best;
	current.durata = best.durata = current.len = best.len = 0;

	current.v = (int*)malloc(N * sizeof(int));
	best.v = (int*)malloc(N * sizeof(int));
	if (current.v == NULL || best.v == NULL)
		exit(-1);

	next_att(0, &current, &best, v, N);

	printf("Durata: %d\n", best.durata);

	for (i = 0; i < best.len; i++)
		printf("(%d, %d) ", v[best.v[i]].s, v[best.v[i]].f);
	printf("\n");

	free(current.v);
	free(best.v);
}

int main() {
	int i, N;
	att* v;
	FILE* infile = fopen(FILENAME, "r");

	fscanf(infile, "%d", &N);
	v = (att*)malloc(N * sizeof(att));
	if (v == NULL)
		exit(-1);

	for (i = 0; i < N; i++)
		fscanf(infile, "%d %d", &v[i].s, &v[i].f);

	fclose(infile);

	attSel(N, v);

	free(v);

	return 0;
}