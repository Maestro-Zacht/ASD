#pragma once

#include <stdio.h>

#define MAXSTR 51

typedef struct {
	int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct {
	char* nome, * tipologia;
	stat_t statistiche;
} oggetto_t;

typedef struct {
	oggetto_t* vettInv;
	oggetto_t NULLOBJ;
	int n_inv;
} inventario_t;

inventario_t inv_setup_from_file(const FILE* infile);
oggetto_t* find_item(const char* name, inventario_t inventario);
void print_item(const FILE* fp, oggetto_t item);
void print_stats(const FILE* fp, stat_t stat);
void free_inv(inventario_t* inventario);