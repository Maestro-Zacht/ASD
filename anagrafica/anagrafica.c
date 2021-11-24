#include "anagrafica.h"
#include <stdlib.h>
#include <string.h>

int datecmp(data a, data b) {
	if (a.anno > b.anno)
		return 3;
	else if (a.anno < b.anno)
		return -3;
	else if (a.mese > b.mese)
		return 2;
	else if (a.mese < b.mese)
		return -2;
	else if (a.giorno > b.giorno)
		return 1;
	else if (a.giorno < b.giorno)
		return -1;
	else
		return 0;
}

LISTA new_lista() {
	LISTA r;
	r.head = NULL;
	r.NULLITEM.birthday.anno = 0;
	r.NULLITEM.birthday.mese = 0;
	r.NULLITEM.birthday.giorno = 0;
	r.NULLITEM.cap = 0;
	r.NULLITEM.citta = NULL;
	r.NULLITEM.codice = -1;
	r.NULLITEM.cognome = NULL;
	r.NULLITEM.nome = NULL;
	r.NULLITEM.via = NULL;
	return r;
}

Item* prendi_item(const FILE* infile) {
	char nome[MAXSTR], cognome[MAXSTR], via[MAXSTR], citta[MAXSTR];
	Item* r = (Item*)malloc(sizeof(Item));
	fscanf(infile, "A%d %s %s %d/%d/%d %s %s %d\n", &r->codice, nome, cognome, &r->birthday.giorno, &r->birthday.mese, &r->birthday.anno, via, citta, &r->cap);

	if (r->codice < 0 || r->codice > 9999)
	{
		free(r);
		return NULL;
	}

	r->nome = strdup(nome);
	r->cognome = strdup(cognome);
	r->via = strdup(via);
	r->citta = strdup(citta);

	return r;
}

void stampa_item(const FILE* outfile, Item* r) {
	fprintf(outfile, "A%04d %s %s %02d/%02d/%04d %s %s %d\n", r->codice, r->nome, r->cognome, r->birthday.giorno, r->birthday.mese, r->birthday.anno, r->via, r->citta, r->cap);
}

void stampa_lista(const FILE* outfile, LISTA l) {
	anagrafica* i;
	for (i = l.head; i != NULL; i = i->next)
		stampa_item(outfile, i->dati);
}

void inserimento_ordinato(LISTA* l, Item* item) {
	anagrafica* new_item = (anagrafica*)malloc(sizeof(anagrafica)), * i;
	new_item->dati = item;

	if (l->head == NULL || datecmp(l->head->dati->birthday, item->birthday) > 0) {
		new_item->next = l->head;
		l->head = new_item;
		return;
	}
	for (i = l->head; i->next != NULL; i = i->next) {
		if (datecmp(i->next->dati->birthday, item->birthday) > 0) {
			new_item->next = i->next;
			i->next = new_item;
			return;
		}
	}

	i->next = new_item;
	new_item->next = NULL;
}

anagrafica* _ricerca_codice(LISTA l, int codice) {
	anagrafica* i, * p = NULL;
	for (i = l.head; i != NULL; p = i, i = i->next)
		if (codice == i->dati->codice)
			return p;

	return p;
}


Item* ricerca_codice(LISTA l, int codice) {
	anagrafica* r = _ricerca_codice(l, codice);
	if (r == NULL)
		return l.head;
	else if (r->next != NULL)
		return r->next->dati;
	else
		return &l.NULLITEM;
}

Item* estrazione_codice(LISTA *l, int codice) {
	anagrafica* item = _ricerca_codice(*l, codice), *tmp;
	Item* r;

	if (item == NULL) {
		r = item->dati;
		l->head = item->next;
		free(item);
	}
	else if (item->next == NULL)
		r = NULL;
	else {
		tmp = item->next;
		r = tmp->dati;
		item->next = tmp->next;
		free(tmp);
	}
	return r;
}

Item* estrazione_data(LISTA *l, data inizio, data fine) {
	anagrafica* i, * p = NULL;
	Item* r;

	if (datecmp(inizio, l->head->dati->birthday) <= 0 && datecmp(l->head->dati->birthday, fine) <= 0) {
		r = l->head->dati;
		i = l->head;
		l->head = i->next;
		free(i);
		return r;
	}

	for (i = l->head; i != NULL && datecmp(i->dati->birthday, fine) <= 0; p = i, i = i->next) {
		if (datecmp(inizio, i->dati->birthday) <= 0 && datecmp(i->dati->birthday, fine) <= 0) {
			r = i->dati;
			p->next = i->next;
			free(i);
			return r;
		}
	}
	return &l->NULLITEM;
}

void clear_item(Item* item) {
	free(item->nome);
	free(item->cognome);
	free(item->via);
	free(item->citta);
	free(item);
}

void clear_lista(LISTA l) {
	anagrafica* i = l.head, * next;
	while (i != NULL) {
		next = i->next;
		clear_item(i->dati);
		free(i);
		i = next;
	}
}