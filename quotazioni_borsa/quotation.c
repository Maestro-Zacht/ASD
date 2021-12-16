#include "quotation.h"
#include <stdlib.h>

quotation_t* qt_init(data_t date) {
	quotation_t* quotation = (quotation_t*)malloc(sizeof(quotation_t));

	quotation->date = date;
	quotation->N = 0;
	quotation->maxN = 20;
	quotation->transactions = (transaction_t*)malloc(quotation->maxN * sizeof(transaction_t));
	if (quotation->transactions == NULL)
		exit(-1);

	return quotation;
}

void qt_free(quotation_t* quotation) {
	free(quotation->transactions);
	free(quotation);
}

void qt_insert_transaction(quotation_t* quotation, transaction_t transaction) {
	if (datecmp(transaction.date, quotation->date) == 0) {
		if (quotation->N == quotation->maxN) {
			quotation->maxN += 20; // l'aumento di dimensione dipende dalla previsione del numero di dati, si potrebbe anche fare *= 2
			quotation->transactions = (transaction_t*)realloc(quotation->transactions, quotation->maxN * sizeof(transaction_t));
			if (quotation->transactions == NULL)
				exit(-1);
		}
		quotation->transactions[quotation->N++] = transaction;
	}
}

float qt_get_quotation(quotation_t* quotation) {
	int i, number = 0;
	float values = 0;
	for (i = 0; i < quotation->N; i++) {
		number += quotation->transactions[i].number;
		values += quotation->transactions[i].number * quotation->transactions[i].value;
	}
	return values / number;
}

QTKEY qt_key(quotation_t* quotation) {
	return quotation->date;
}
