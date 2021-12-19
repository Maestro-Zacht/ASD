#include "quotation.h"
#include <stdlib.h>

quotation_t qt_init(data_t date) {
	quotation_t quotation;

	quotation.date = date;
	quotation.den = 0;
	quotation.num = 0;
	return quotation;
}

quotation_t qt_null() {
	quotation_t qtnull;
	qtnull.den = 0;
	qtnull.num = 0;
	qtnull.date.year = qtnull.date.month = qtnull.date.day = -1;
	return qtnull;
}

quotation_t qt_merge(quotation_t* quotation1, quotation_t* quotation2) {
	quotation_t r;
	if (datecmp(qt_key(quotation1), qt_key(quotation1)) == 0) {
		r.date = quotation1->date;
		r.num = quotation1->num + quotation2->num;
		r.den = quotation1->den + quotation2->den;
	}
	else
		r = qt_null();

	return r;
}

void qt_insert_transaction(quotation_t* quotation, transaction_t transaction) {
	if (datecmp(transaction.date, quotation->date) == 0) {
		quotation->num += transaction.number * transaction.value;
		quotation->den += transaction.number;
	}
}

float qt_get_quotation(quotation_t* quotation) {
	return (quotation->den != 0) ? quotation->num / quotation->den : 0;
}

QTKEY qt_key(quotation_t* quotation) {
	return quotation->date;
}
