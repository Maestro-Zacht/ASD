#include "quotations.h"
#include <stdlib.h>

typedef struct bst_node* link;

struct bst_node {
	quotation_t quotation;
	link p, l, r;
	int N;
};

struct quotations_s {
	link root, z;
};

link new_node(quotation_t quotation, link p, link r, link l, int N) {
	link n = (link)malloc(sizeof(struct bst_node));
	n->l = l;
	n->r = r;
	n->p = p;
	n->N = N;
	n->quotation = quotation;
	return n;
}

quotations_l qts_init() {
	quotations_l qts = (quotations_l)malloc(sizeof(struct quotations_s));
	qts->z = new_node(qt_null(), NULL, NULL, NULL, 0);
	qts->root = qts->z;
	return qts;
}

void qts_free_r(link l, link z) {
	if (l != z) {
		qts_free_r(l->r, z);
		qts_free_r(l->l, z);
		free(l);
	}
}

void qts_free(quotations_l quotations) {
	if (quotations != NULL) {
		qts_free_r(quotations->root, quotations->z);
		free(quotations->z);
		free(quotations);
	}
}

link leaf_insert_r(link l, quotation_t* quotation, link z) {
	if (l == z)
		return new_node(*quotation, z, z, z, 1);

	if (datecmp(qt_key(quotation), qt_key(&l->quotation)) < 0) {
		l->l = leaf_insert_r(l->l, quotation, z);
		l->l->p = l;
	}
	else {
		l->r = leaf_insert_r(l->r, quotation, z);
		l->r->p = l;
	}

	l->N++;
	return l;
}

void qts_insert_quotation(quotations_l quotations, quotation_t* quotation) {
	quotation_t* qtp = qts_search_quotation(quotations, qt_key(quotation));
	if (qtp == NULL)
		quotations->root = leaf_insert_r(quotations->root, quotation, quotations->z);
	else
		*qtp = qt_merge(quotation, qtp);
}

quotation_t* search_r(link l, QTKEY key, link z) {
	int cmp;
	if (l == z)
		return NULL;
	cmp = datecmp(key, qt_key(&l->quotation));
	if (cmp == 0)
		return &l->quotation;
	if (cmp < 0)
		return search_r(l->l, key, z);
	return search_r(l->r, key, z);
}

quotation_t* qts_search_quotation(quotations_l quotations, QTKEY key) {
	return search_r(quotations->root, key, quotations->z);
}

QTKEY key_min_r(link l, link z) {
	if (l->l == z)
		return qt_key(&l->quotation);
	return key_min_r(l->l, z);
}

void quotation_min_max_r(link l, QTKEY key_min, QTKEY key_max, float* min, float* max, link z) {
	float quotation;
	if (l != z) {
		if (date_in_range(key_min, key_max, qt_key(&l->quotation))) {
			quotation = qt_get_quotation(&l->quotation);
			if (quotation > *max)
				*max = quotation;
			if (quotation < *min)
				*min = quotation;
			quotation_min_max_r(l->l, key_min, key_max, min, max, z);
			quotation_min_max_r(l->r, key_min, key_max, min, max, z);
		}
		else if(datecmp(qt_key(&l->quotation), key_max) > 0)
			quotation_min_max_r(l->l, key_min, key_max, min, max, z);
		else
			quotation_min_max_r(l->r, key_min, key_max, min, max, z);
	}
}

void qts_quotation_min_max(quotations_l quotations, QTKEY key_min, QTKEY key_max, float* min, float* max) {
	quotation_min_max_r(quotations->root, key_min, key_max, min, max, quotations->z);
}

QTKEY qts_key_min(quotations_l quotations) {
	return key_min_r(quotations->root, quotations->z);
}

QTKEY key_max_r(link l, link z) {
	if (l->r == z)
		return qt_key(&l->quotation);
	return key_max_r(l->r, z);
}

QTKEY qts_key_max(quotations_l quotations) {
	return key_max_r(quotations->root, quotations->z);
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

void height_min_max(link l, link z, int pos, int* min, int* max) {
	if (l->l != z && l->r != z) {
		height_min_max(l->l, z, pos + 1, min, max);
		height_min_max(l->r, z, pos + 1, min, max);
	}
	else if (l->l == z && l->r == z) {
		if (pos > *max)
			*max = pos;
	}
	else if (l->l == z) {
		if (pos < *min)
			*min = pos;
		height_min_max(l->r, z, pos + 1, min, max);
	}
	else {
		if (pos < *min)
			*min = pos;
		height_min_max(l->l, z, pos + 1, min, max);
	}
}

void qts_balance(quotations_l quotations, int soglia) {	
	int min = quotations->root->N, max = -1;
	height_min_max(quotations->root, quotations->z, 0, &min, &max);
	if (min == 0 || max / min > soglia)
		quotations->root = balance_r(quotations->root, quotations->z);
}
