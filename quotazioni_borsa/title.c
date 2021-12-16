#include "title.h"
#include "quotations.h"
#include <stdlib.h>
#include <string.h>

struct title_s {
	char* cod;
	quotations_l quotations;
};

TTLKEY title_key(title_t title) {
	return title->cod;
}

title_t title_read(const FILE* fp) {
	char name[MAXSTR];
	int i, N;
	quotation_t* qtp;
	transaction_t transaction;
	title_t titolo = (struct title_s*)malloc(sizeof(struct title_s));
	if (titolo == NULL)
		exit(-1);

	if (fscanf(fp, "%s %d", name, &N) != 2)
		exit(-1);

	titolo->cod = strdup(name);

	for (i = 0; i < N; i++) {
		transaction.date = date_read(fp);
		qtp = qts_search_quotation(titolo->quotations, transaction.date);
		if (qtp == NULL) {
			qtp = (quotation_t*)malloc(sizeof(quotation_t));
			qtp = qt_init(transaction.date);
			qts_insert_quotation(titolo->quotations, qtp);
		}
		if (fscanf(fp, "%f %d", &transaction.value, &transaction.number) != 2)
			exit(-1);
		qt_insert_transaction(qtp, transaction);
	}
	return titolo;
}

void title_free(title_t title) {
	free(title->cod);
	qts_free(title->quotations);
	free(title);
}

int title_cmp(title_t a, title_t b) {
	return strcmp(title_key(a), title_key(b));
}

int title_cmp_key(title_t a, TTLKEY key) {
	return strcmp(title_key(a), key);
}

void title_print_quotation(const FILE* fp, title_t title, QTKEY key) {
	quotation_t* qtp = qts_search_quotation(title->quotations, key);
	fprintf(fp, "La quotazione del titolo %s al giorno ", title_key(title));
	date_print(fp, qtp->date);
	fprintf(fp, "e' stata di %.2f dollari\n", qt_get_quotation(qtp));
}

void title_print_quotations_range(const FILE* fp, title_t title, QTKEY key_min, QTKEY key_max) {
	float val_min, val_max;
	qts_quotation_min_max(title->quotations, key_min, key_max, &val_min, &val_max);
	fprintf(fp, "Il valore minimo della quotazione del titolo e' stato %.2f, mentre il massimo e' stato %.2f\n", val_min, val_max);
}

void title_print_quotations_min_max(const FILE* fp, title_t title) {
	float val_min, val_max;
	qts_quotation_min_max(title->quotations, qts_key_min(title->quotations), qts_key_max(title->quotations), &val_min, &val_max);
	fprintf(fp, "Il valore minimo della quotazione del titolo e' stato %.2f, mentre il massimo e' stato %.2f\n", val_min, val_max);
}
