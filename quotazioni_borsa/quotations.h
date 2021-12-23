#pragma once

#include <stdio.h>
#include "quotation.h"

typedef struct quotations_s* quotations_l;

quotations_l qts_init();
void qts_free(quotations_l quotations);

void qts_insert_quotation(quotations_l quotations, quotation_t* quotation);
quotation_t* qts_search_quotation(quotations_l quotations, QTKEY key);

void qts_quotation_min_max(quotations_l quotations, QTKEY key_min, QTKEY key_max, float* min, float* max);
QTKEY qts_key_min(quotations_l quotations);
QTKEY qts_key_max(quotations_l quotations);

void qts_balance(quotations_l quotations, float soglia);