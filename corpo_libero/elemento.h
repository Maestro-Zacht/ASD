#pragma once

#include <stdio.h>
#define MAXSTR 100

typedef struct {
	char nome[MAXSTR];
	int tipologia, direzione_ingresso, direzione_uscita, precedenza, finale, difficoltà;
	float valore;
} elemento_t;

typedef struct {
	elemento_t* elementi;
	int N;
} elementi_l;

elementi_l elem_l_init(int N);
elementi_l elem_l_read(const FILE* fp);
void elem_l_free(elementi_l lista);
void elem_read(const FILE* fp, elemento_t* elem_p);