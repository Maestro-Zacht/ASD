#include "pacco.h"
#include <stdlib.h>
#include <string.h>

typedef struct bst_node* link;

struct bst_node {
	pacco_item data;
	link p, l, r;
	int N;
};

struct pacchi_s {
	link root, z;
};

link new_node(pacco_item data, link p, link r, link l, int N) {
	link n = (link)malloc(sizeof(struct bst_node));
	n->l = l;
	n->r = r;
	n->p = p;
	n->N = N;
	n->data = data;
	return n;
}

pacco_item pacco_item_null() {
	pacco_item nul;
	nul.corridoio = nul.slot = -1;
	return nul;
}

pacco_item pacco_item_load(FILE* fp) {
	pacco_item data;
	fscanf(fp, "%s %d %s %d", data.key, &data.corridoio, data.scaffale, &data.slot);
	return data;
}

int pacco_item_check_null(pacco_item data) {
	return data.corridoio == -1;
}

Pacchi PACCHIinit() {
	Pacchi p = (struct pacchi_s*)malloc(sizeof(struct pacchi_s));
	p->z = new_node(pacco_item_null(), NULL, NULL, NULL, 0);
	p->root = p->z;
	return p;
}

link leaf_insert_r(link l, pacco_item data, link z) {
	if (l == z)
		return new_node(data, z, z, z, 1);

	if (strcmp(data.key, l->data.key) < 0) {
		l->l = leaf_insert_r(l->l, data, z);
		l->l->p = l;
	}
	else {
		l->r = leaf_insert_r(l->r, data, z);
		l->r->p = l;
	}

	l->N++;
	return l;
}

void PACCHIinsert(Pacchi p, pacco_item data) {
	leaf_insert_r(p->root, data, p->z);
}

link search_r(link l, char* key, link z) {
	int cmp;
	if (l == z)
		return NULL;
	cmp = strcmp(key, l->data.key);
	if (cmp == 0)
		return l;
	if (cmp < 0)
		return search_r(l->l, key, z);
	return search_r(l->r, key, z);
}

pacco_item PACCHIsearch(Pacchi p, char* key) {
	link node = search_r(p->root, key, p->z);
	if (node != NULL)
		return node->data;
	return pacco_item_null();
}

void PACCHIextract(Pacchi p, char* key) {
	link node = search_r(p->root, key, p->z);
	if (node != NULL) {
		node->data.corridoio = node->data.slot = -1;
	}
}

void PACCHIposition(Pacchi p, char* key, int corridoio, char* scaffale, int slot) {
	link node = search_r(p->root, key, p->z);
	if (node != NULL) {
		node->data.corridoio = corridoio;
		strcpy(node->data.scaffale, scaffale);
		node->data.slot = slot;
	}
}

link rot_r(link l) {
	link help = l->l;
	l->l = help->r;
	l->l->p = l;
	help->r = l;
	help->p = l->p;
	l->p = help;
	help->N = l->N;
	l->N = 1;
	l->N += (l->l) ? l->l->N : 0;
	l->N += (l->r) ? l->r->N : 0;
	return help;
}

link rot_l(link l) {
	link help = l->r;
	l->r = help->l;
	l->r->p = l;
	help->l = l;
	help->p = l->p;
	l->p = help;
	help->N = l->N;
	l->N = 1;
	l->N += (l->l) ? l->l->N : 0;
	l->N += (l->r) ? l->r->N : 0;
	return help;
}

link partition_r(link l, int rank) {
	int t = l->l->N;
	if (t > rank) {
		l->l = partition_r(l->l, rank);
		l = rot_r(l);
	}
	else if (t < rank) {
		l->r = partition_r(l->r, rank - t - 1);
		l = rot_l(l);
	}
	return l;
}

link balance_r(link l, link z) {
	int med;
	if (l != z) {
		med = (l->N + 1) / 2 - 1;
		l = partition_r(l, med);
		l->l = balance_r(l->l, z);
		l->r = balance_r(l->r, z);
	}
	return l;
}

void PACCHIbalance(Pacchi p) {
	balance_r(p->root, p->z);
}


