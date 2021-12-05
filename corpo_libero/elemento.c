#include "elemento.h"

#include <stdlib.h>

elementi_l elem_l_init(int N) {
	elementi_l lista;
	if (N < 1)
		exit(-1);
	lista.N = N;
	lista.elementi = (elemento_t*)malloc(N * sizeof(elemento_t));

	return lista;
}

elementi_l elem_l_read(const FILE* fp) {
	int N, i;
	elementi_l lista;

	if (fscanf(fp, "%d", &N) != 1)
		exit(-1);

	lista = elem_l_init(N);

	for (i = 0; i < N; i++)
		elem_read(fp, &lista.elementi[i]);

	return lista;
}

void elem_l_free(elementi_l lista) {
	free(lista.elementi);
}


void elem_read(const FILE* fp, elemento_t* elem_p) {
	if (fscanf(fp, "%s %d %d %d %d %d %f %d", &elem_p->nome, &elem_p->tipologia, &elem_p->direzione_ingresso, &elem_p->direzione_uscita, &elem_p->precedenza, &elem_p->finale, &elem_p->valore, &elem_p->difficoltà) != 8)
		exit(-1);
}
