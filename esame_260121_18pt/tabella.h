#pragma once

typedef struct table_s* st_table;

st_table st_init(int N);
st_table st_copy(st_table table);
void st_free(st_table table);

void st_sort(st_table table);

void st_insert(st_table table, const char* id);

const char* st_search_by_index(st_table table, int index);
int st_search_by_id(st_table table, const char* id);