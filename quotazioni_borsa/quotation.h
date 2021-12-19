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
	int den;
	float num;
} quotation_t;

quotation_t qt_init(data_t date);
quotation_t qt_null();

quotation_t qt_merge(quotation_t* quotation1, quotation_t* quotation2);
void qt_insert_transaction(quotation_t* quotation, transaction_t transaction);
float qt_get_quotation(quotation_t* quotation);
QTKEY qt_key(quotation_t* quotation);