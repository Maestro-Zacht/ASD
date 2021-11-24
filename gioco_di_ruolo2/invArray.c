#include "invArray.h"

struct invArray_s {
	int n_inv;
	inv_t* vett_inv;
};

invArray_t invArray_init() {
	invArray_t arr = (invArray_t)malloc(sizeof(struct invArray_s));
	if (arr == NULL)
		exit(-1);
	arr->n_inv = 0;
	arr->vett_inv = NULL;
	return arr;
}

void invArray_free(invArray_t invArray) {
	if (invArray != NULL) {
		if (invArray->vett_inv != NULL)
			free(invArray->vett_inv);
		free(invArray);
	}
}

void invArray_read(FILE* fp, invArray_t invArray) {
	int i;
	if (fp == NULL || fscanf(fp, "%d", &invArray->n_inv) != 1)
		exit(-1);
	invArray->vett_inv = (inv_t*)malloc(invArray->n_inv * sizeof(inv_t));
	if (invArray->vett_inv == NULL)
		exit(-1);

	for (i = 0; i < invArray->n_inv; i++)
		inv_read(fp, &invArray->vett_inv[i]);
}

void invArray_print(FILE* fp, invArray_t invArray) {
	int i;
	for (i = 0; i < invArray->n_inv; i++)
		inv_print(fp, &invArray->vett_inv[i]);
}

void invArray_printByIndex(FILE* fp, invArray_t invArray, int index) {
	if (index < 0 || index >= invArray->n_inv)
		fprintf(fp, "Non c'e' nessun elemento alla posizione %d\n", index + 1);
	else
		inv_print(fp, &invArray->vett_inv[index]);
}

inv_t* invArray_getByIndex(invArray_t invArray, int index) {
	if (index < 0 || index >= invArray->n_inv)
		return NULL;
	return &invArray->vett_inv[index];
}

int invArray_searchByName(invArray_t invArray, char* name) {
	int i;
	for (i = 0; i < invArray->n_inv; i++)
		if (strcmp(name, invArray->vett_inv[i].nome) == 0)
			return i;
	return -1;
}
