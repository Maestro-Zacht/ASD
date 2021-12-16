#include "titles.h"
#include <stdlib.h>

typedef struct node_s {
	title_t title;
	struct node_s* next;
} node_t;

struct titles_s {
	node_t* head;
};

void titles_insert(titles_l titles, title_t title) {
	node_t* i, * p, * new_node = (node_t*)malloc(sizeof(node_t));
	new_node->title = title;

	if (title_cmp(title, titles->head->title) < 0) {
		new_node->next = titles->head;
		titles->head = new_node;
		return;
	}

	for (p = titles->head, i = titles->head->next; i != NULL && title_cmp(title, i->title) > 0; p = i, i = i->next);

	p->next = new_node;
	new_node->next = i;
}

titles_l titles_read(const FILE* fp) {
	int N, i;
	title_t titolo;
	titles_l titoli;

	if (fscanf(fp, "%d", &N) != 1)
		exit(-1);

	titoli = (struct titles_s*)malloc(sizeof(struct titles_s));
	titoli->head = NULL;

	for (i = 0; i < N; i++) {
		titolo = title_read(fp);
		titles_insert(titoli, titolo);
	}

	return titoli;
}

void titles_free(titles_l titles) {
	node_t* i, * next;
	for (i = titles->head; i != NULL; i = next) {
		next = i->next;
		title_free(i->title);
		free(i);
	}
	free(titles);
}

title_t titles_search(titles_l titles, TTLKEY title_cod) {
	node_t* i;

	for (i = titles->head; i != NULL && title_cmp_key(i->title, title_cod) < 0; i = i->next);
	
	if (i == NULL || title_cmp_key(i->title, title_cod) > 0)
		return NULL;
	return i->title;
}
