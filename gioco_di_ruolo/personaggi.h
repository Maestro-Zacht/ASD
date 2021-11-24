#pragma once

#include "inventario.h"
#define MAXEQUIP 8

typedef struct {
	//int in_uso;
	oggetto_t* vett_equipaggiamento[MAXEQUIP];
} equipaggiamento_t;

typedef struct {
	int codice;
	char* nome, * classe;
	stat_t statistiche;
	equipaggiamento_t equipaggiamento;
} personaggio_t;

typedef struct nodo_pg {
	personaggio_t personaggio;
	struct nodo_pg* next;
} nodo_pg;

typedef struct {
	nodo_pg* head;
	//int n_pg;
	personaggio_t NULLPG;
} tabella_personaggi_t;

tabella_personaggi_t pg_setup_from_file(const FILE* infile);
nodo_pg* pg_insert(personaggio_t dati, tabella_personaggi_t* tb_pg);
personaggio_t pg_input(const FILE* infile, tabella_personaggi_t* pg_tb);
void pg_delete(int codice, tabella_personaggi_t* tb_pg);
nodo_pg* pg_search(int codice, tabella_personaggi_t* tb_pg);
void pg_add_equip(nodo_pg* personaggio, const char* item_nome, inventario_t inventario);
void pg_remove_equip(nodo_pg* personaggio, const char* item_nome);
void pg_details(nodo_pg* p);
void pg_print_list(tabella_personaggi_t tb_pg);
void pg_free_list(tabella_personaggi_t* tb_pg);