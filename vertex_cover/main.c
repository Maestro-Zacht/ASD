#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int u, v;
} arco;

typedef struct {
	arco* archi;
	int E, N;
} grafo;


grafo leggiFile(const FILE* infile) {
	grafo r;
	int i;

	fscanf(infile, "%d %d", &r.N, &r.E);

	r.archi = (arco*)malloc(r.E * sizeof(arco));

	for (i = 0; i < r.E; i++)
		fscanf(infile, "%d %d", &r.archi[i].u, &r.archi[i].v);

	return r;
}


int check(grafo G, int* soluzione) {
	int i;
	for (i = 0; i < G.E; i++)
		if (soluzione[G.archi[i].u] == 0 && soluzione[G.archi[i].v] == 0)
			return 0;
	return 1;
}


int powerset(int pos, int* sol, int cnt, grafo G) {
	int j;
	if (pos >= G.N) {
		if (check(G, sol)) {
			printf("{ ");
			for (j = 0; j < G.N; j++)
				if (sol[j] != 0)
					printf("%d, ", j);
			printf("}\n");

			return cnt + 1;
		}
		return cnt;
	}
	sol[pos] = 0;
	cnt = powerset(pos + 1, sol, cnt, G);
	sol[pos] = 1;
	cnt = powerset(pos + 1, sol, cnt, G);
	return cnt;
}

int vertex_cover(grafo G) {
	int r, * sol = (int*)calloc(G.N, sizeof(int));
	r = powerset(0, sol, 0, G);
	free(sol);
	return r;
}


int main() {
	grafo G;
	int covers;
	FILE* infile = fopen("grafo.txt", "r");

	G = leggiFile(infile);
	fclose(infile);

	covers = vertex_cover(G);

	printf("Sono state trovate %d coperture\n", covers);

	free(G.archi);

	return 0;
}