#pragma once

#include "datetime.h"

typedef data_t QTKEY;

typedef struct {
	data_t date;
	float value;
	int number;
} transaction_t;

typedef struct {
	data_t date;
	transaction_t* transactions;
	int N, maxN;
} quotation_t;

quotation_t* qt_init(data_t date);
void qt_free(quotation_t* quotation);

void qt_insert_transaction(quotation_t* quotation, transaction_t transaction);
float qt_get_quotation(quotation_t* quotation);
QTKEY qt_key(quotation_t* quotation);