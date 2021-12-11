#include "personaggi.h"
#include <stdlib.h>
#include <string.h>



void pg_free(nodo_pg* nodo) {
	free(nodo->personaggio.nome);
	free(nodo->personaggio.classe);
	free(nodo);
}

nodo_pg* pg_insert(personaggio_t dati, tabella_personaggi_t* tb_pg) {
	nodo_pg* nodo = (nodo_pg*)malloc(sizeof(nodo_pg));
	nodo->personaggio = dati;
	nodo->next = NULL;
	if (tb_pg->tail == NULL)
		tb_pg->tail = tb_pg->head = nodo;
	else {
		tb_pg->tail->next = nodo;
		tb_pg->tail = nodo;
	}
	return nodo;
}

nodo_pg* _pg_search(int codice, tabella_personaggi_t* tb_pg) {
	nodo_pg* i, *p;
	for (i = tb_pg->head, p = NULL; i != NULL; p = i, i = i->next)
		if (codice == i->personaggio.codice)
			return p;
	return p;
}

void pg_delete(int codice, tabella_personaggi_t* tb_pg) {
	nodo_pg* prev = _pg_search(codice, tb_pg), *tmp;
	if (prev == NULL) {
		if (tb_pg->head != NULL) {
			if (tb_pg->head == tb_pg->tail) {
				free(tb_pg->head);
				tb_pg->head = tb_pg->tail = NULL;
			}
			else {
				tmp = tb_pg->head->next;
				pg_free(tb_pg->head);
				tb_pg->head = tmp;
			}
		}			
	}
	else if (prev->next != NULL) {
		if (tb_pg->tail == prev->next)
			tb_pg->tail = prev;
		tmp = prev->next->next;
		pg_free(prev->next);
		prev->next = tmp;
	}
}

nodo_pg* pg_search(int codice, tabella_personaggi_t* tb_pg) {
	nodo_pg* prev = _pg_search(codice, tb_pg);
	if (prev == NULL)
		return tb_pg->head;
	return prev->next;
}

void pg_add_equip(nodo_pg* personaggio, const char* item_nome, inventario_t inventario) {
	oggetto_t* equip = find_item(item_nome, inventario);
	int i, already_equip = 0;
	if (equip == &inventario.NULLOBJ)
		printf("Equipaggiamento non presente\n");
	else if (personaggio == NULL)
		printf("Personaggio non presente\n");
	else {
		for (i = 0; i < MAXEQUIP && !already_equip; i++)
			if (personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i] == equip)
				already_equip = 1;

		if (already_equip)
			printf("Oggetto gia' equipaggiato\n");
		else {
			for (i = 0; i < MAXEQUIP; i++) {
				if (personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i] == NULL) {
					// personaggio->personaggio.equipaggiamento.in_uso = 1;
					personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i] = equip;
					printf("Equipaggiamento aggiunto\n");
					return;
				}
			}
			printf("Non e' possibile aggiungere altro equipaggiamento\n");
		}
	}
}

void pg_remove_equip(nodo_pg* personaggio, const char* item_nome) {
	int i;
	if (personaggio == NULL)
		printf("Personaggio non presente\n");
	else {
		for (i = 0; i < MAXEQUIP; i++) {
			if (personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i] != NULL && strcmp(item_nome, personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i]->nome) == 0) {
				personaggio->personaggio.equipaggiamento.vett_equipaggiamento[i] = NULL;
				printf("Equipaggiamento rimosso\n");
				return;
			}
		}
		printf("Equipaggiamento non presente\n");
	}
}

void pg_details(nodo_pg* p) {
	stat_t statistiche_equip;
	int i, is_equip = 0;
	if (p == NULL)
		printf("Personaggio non presente\n");
	else {
		printf("PG%d\n%s (%s)\n\nEquipaggiamento:\n", p->personaggio.codice, p->personaggio.nome, p->personaggio.classe);

		statistiche_equip = p->personaggio.statistiche;
		for (i = 0; i < MAXEQUIP; i++) {
			if (p->personaggio.equipaggiamento.vett_equipaggiamento[i] != NULL) {
				print_item(stdout, *p->personaggio.equipaggiamento.vett_equipaggiamento[i]);
				statistiche_equip.atk += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.atk;
				statistiche_equip.def += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.def;
				statistiche_equip.hp += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.hp;
				statistiche_equip.mp += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.mp;
				statistiche_equip.mag += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.mag;
				statistiche_equip.spr += p->personaggio.equipaggiamento.vett_equipaggiamento[i]->statistiche.spr;
				is_equip = 1;
			}
		}
		if (!is_equip)
			printf("Nessun equipaggiamento presente\n");

		printf("\nStatistiche base:\n");
		print_stats(stdout, p->personaggio.statistiche);
		
		if (is_equip) {
			if (statistiche_equip.atk < 0)
				statistiche_equip.atk = 0;

			if (statistiche_equip.def < 0)
				statistiche_equip.def = 0;

			if (statistiche_equip.hp < 0)
				statistiche_equip.hp = 0;

			if (statistiche_equip.mp < 0)
				statistiche_equip.mp = 0;

			if (statistiche_equip.mag < 0)
				statistiche_equip.mag = 0;

			if (statistiche_equip.spr < 0)
				statistiche_equip.spr = 0;

			printf("\nStatistiche con equipaggiamento:\n");
			print_stats(stdout, statistiche_equip);
		}
	}
}

int pg_check_codice(int new_pg_codice, tabella_personaggi_t* pg_tb) {
	nodo_pg* i;
	for (i = pg_tb->head; i != NULL; i = i->next)
		if (new_pg_codice == i->personaggio.codice)
			return 0;
	return 1;
}

personaggio_t pg_input(const FILE* infile, tabella_personaggi_t* pg_tb) {
	personaggio_t p;
	char nome[MAXSTR], classe[MAXSTR];
	int i, ret_val = fscanf(infile, " PG%d %s %s %d %d %d %d %d %d", &p.codice, nome, classe, &p.statistiche.hp, &p.statistiche.mp, &p.statistiche.atk, &p.statistiche.def, &p.statistiche.mag, &p.statistiche.spr);
	if (ret_val != 9 || p.codice < 0 || p.codice > 9999 || !pg_check_codice(p.codice, pg_tb))
		return pg_tb->NULLPG;

	p.nome = strdup(nome);
	p.classe = strdup(classe);

	//p.equipaggiamento.in_uso = 0;
	for (i = 0; i < MAXEQUIP; i++)
		p.equipaggiamento.vett_equipaggiamento[i] = NULL;

	return p;
}

tabella_personaggi_t pg_setup_from_file(const FILE* infile) {
	tabella_personaggi_t tb_pg;
	personaggio_t p;

	if (infile == NULL || feof(infile))
		exit(-1);

	tb_pg.head = tb_pg.tail = tb_pg.NULLPG.classe = tb_pg.NULLPG.nome = NULL;
	tb_pg.NULLPG.codice = tb_pg.NULLPG.statistiche.atk = tb_pg.NULLPG.statistiche.mag = tb_pg.NULLPG.statistiche.hp = tb_pg.NULLPG.statistiche.mp = tb_pg.NULLPG.statistiche.def = tb_pg.NULLPG.statistiche.spr = 0;

	do {
		p = pg_input(infile, &tb_pg);

		if (p.nome != tb_pg.NULLPG.nome)
			pg_insert(p, &tb_pg);

	} while (!feof(infile) && p.nome != tb_pg.NULLPG.nome);

	return tb_pg;
}

void pg_print_list(tabella_personaggi_t tb_pg) {
	nodo_pg* i;
	for (i = tb_pg.head; i != NULL; i = i->next)
		printf("PG%04d %s (%s)\n", i->personaggio.codice, i->personaggio.nome, i->personaggio.classe);
}

void pg_free_list(tabella_personaggi_t* tb_pg) {
	nodo_pg* i, * tmp;
	for (i = tb_pg->head; i != NULL; i = tmp) {
		tmp = i->next;
		free(i->personaggio.nome);
		free(i->personaggio.classe);
		free(i);
	}
	tb_pg->head = tb_pg->tail = NULL;
}