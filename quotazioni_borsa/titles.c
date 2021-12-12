#include "titles.h"
#include <string.h>

typedef struct node_s {
	title_t title;
	struct node_s* next;
} node_t;

struct titles_s {
	node_t* head;
};

void titles_insert(titles_l titles, title_t title) {
	node_t* i, * p;

	if (strcmp(title_key(title), title_key(titles->head->title)) < 0) {
		//here
	}

	for (p = NULL, i = titles->head; i != NULL; i = i->next) {

	}
}

titles_l titles_read(const FILE* fp) {
}
