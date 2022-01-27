#include "scaffale.h"
#include <stdlib.h>
#include <string.h>

typedef struct item_s {
	char* key, **pacchi;
	int N;
} scaffale_item;

typedef struct bst_node* link;

struct bst_node {
	scaffale_item data;
	link p, l, r;
	int N;
};

struct scaffali_s {
	link root, z;
	int K;
};

link new_node(scaffale_item data, link p, link r, link l, int N) {
	link n = (link)malloc(sizeof(struct bst_node));
	n->l = l;
	n->r = r;
	n->p = p;
	n->N = N;
	n->data = data;
	return n;
}

scaffale_item scaffale_item_null() {
	scaffale_item nul;
	nul.N = 0;
	nul.key = nul.pacchi = NULL;
	return nul;
}

Scaffali SCAFFALIinit(int K) {
	Scaffali s = (struct scaffali_s*)malloc(sizeof(struct scaffali_s));
	s->K = K;
	s->z = new_node(scaffale_item_null(), NULL, NULL, NULL, 0);
	s->root = s->z;
	return s;
}

link leaf_insert_r(link l, scaffale_item data, link z) {
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

void SCAFFALIinsert(Scaffali s, char* key) {
	scaffale_item data;
	//data.corridoio = corridoio;
	data.key = strdup(key);
	data.N = 0;
	data.pacchi = (char**)malloc(s->K * sizeof(char*));
	leaf_insert_r(s->root, data, s->z);
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

void SCAFFALIbalance(Scaffali s) {
	s->root = balance_r(s->root, s->z);
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

int SCAFFALIisSlotFree(Scaffali s, char* key, int slot) {
	link node = search_r(s->root, key, s->z);

	if (node != NULL)
		return node->data.pacchi[slot] == NULL;

	return 0;
}

void SCAFFALIinsertPacco(Scaffali s, char* key, char* pacco, int slot) {
	link node;
	if (SCAFFALIisSlotFree(s, key, slot)) {
		node = search_r(s->root, key, s->z);
		node->data.pacchi[slot] = strdup(pacco);
	}
}

void SCAFFALIremovePacco(Scaffali s, char* key, int slot) {
	link node = search_r(s->root, key, s->z);

	if (node != NULL) {
		free(node->data.pacchi[slot]);
		node->data.pacchi[slot] = NULL;
	}
}
