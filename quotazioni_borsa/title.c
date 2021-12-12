#include "title.h"
#include "quotations.h"
#include <stdlib.h>
#include <string.h>

struct title_s {
	char* cod;
	quotations_l quotations;
};

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
			*qtp = qt_init(transaction.date);
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

TTLKEY title_key(title_t title) {
	return title->cod;
}
