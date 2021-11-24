#include "pgList.h"

typedef struct node_s {
	pg_t personaggio;
	struct node_s* next;
} node_t;

struct pgList_s {
	node_t* head;
};

node_t* _search_pg_prev(pgList_t pgList, const char* cod) {
	node_t* i, * p;
	for (i = pgList->head, p = NULL; i != NULL; p = i, i = i->next)
		if (strcmp(cod, i->personaggio.cod) == 0)
			return p;
	return p;
}

pgList_t pgList_init() {
	pgList_t lista = (pgList_t)malloc(sizeof(struct pgList_s));
	if (lista != NULL)
		lista->head = NULL;
	return lista;
}

void pgList_free(pgList_t pgList) {
	node_t* i, * tmp;
	for (i = pgList->head; i != NULL; i = tmp) {
		tmp = i->next;
		pg_clean(&i->personaggio);
		free(i);
	}
	pgList->head = NULL;
}

void pgList_read(FILE* fp, pgList_t pgList) {
	pg_t new_pg;
	if (fp != NULL && pgList != NULL)
		while (pg_read(fp, &new_pg))
			pgList_insert(pgList, new_pg);
}

void pgList_print(FILE* fp, pgList_t pgList, invArray_t invArray) {
	node_t* i;
	for (i = pgList->head; i != NULL; i = i->next)
		pg_print(fp, &i->personaggio, invArray);
}

void pgList_insert(pgList_t pgList, pg_t pg) {
	node_t* new_node;

	if (pgList_searchByCode(pgList, pg.cod) == NULL) {
		new_node = (node_t*)malloc(sizeof(node_t));
		new_node->personaggio = pg;
		new_node->next = pgList->head;
		pgList->head = new_node;
	}
}

void pgList_remove(pgList_t pgList, char* cod) {
	node_t* tmp, * prev = _search_pg_prev(pgList, cod);
	if (prev == NULL) {
		if (pgList->head != NULL) {
			tmp = pgList->head->next;
			pg_clean(&pgList->head->personaggio);
			free(pgList->head);
			pgList->head = tmp;
		}
	}
	else if (prev->next != NULL) {
		tmp = prev->next->next;
		pg_clean(&prev->next->personaggio);
		free(prev->next);
		prev->next = tmp;
	}
}

pg_t* pgList_searchByCode(pgList_t pgList, char* cod) {
	node_t* prev = _search_pg_prev(pgList, cod);
	if (prev == NULL) {
		if (pgList->head != NULL)
			return &pgList->head->personaggio;
		else
			return NULL;
	}
	else if (prev->next != NULL)
		return &prev->next->personaggio;
	else
		return NULL;
}
