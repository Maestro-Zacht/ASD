#pragma once
#include <stdio.h>
#define MAXSTR 51

typedef struct {
	int giorno, mese, anno;
} data;

typedef struct {
	int codice, cap;
	char* nome, * cognome, * via, * citta;
	data birthday;
} Item;

typedef struct anagrafica {
	Item* dati;
	struct anagrafica* next;
} anagrafica;

typedef struct {
	anagrafica* head;
	Item NULLITEM;
} LISTA;

int datecmp(data a, data b);
LISTA new_lista(void);
Item* prendi_item(const FILE*);
void stampa_item(const FILE*, Item*);
void stampa_lista(const FILE*, LISTA);
void inserimento_ordinato(LISTA*, Item*);
Item* ricerca_codice(LISTA, int);
Item* estrazione_codice(LISTA*, int);
Item* estrazione_data(LISTA*, data, data);
void clear_item(Item*);
void clear_lista(LISTA);