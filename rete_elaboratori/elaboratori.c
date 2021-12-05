#include "elaboratori.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct elab_s {
	int** mat_adj, V, E;
	char** tab_names;
};

int elab_get_index(elaboratori_t elabs, const char* node_name) {
	int i;
	for (i = 0; i < elabs->V; i++)
		if (strcmp(node_name, elabs->tab_names[i]) == 0)
			return i;
	return -1;
}

void elab_insert_node(elaboratori_t elabs, const char* node_name) {

}

elaboratori_t elab_init(const char* filename) {
	int i = 0;
	char nome1[MAXSTR], nome2[MAXSTR];
	elaboratori_t elabs = (struct elab_s*)malloc(sizeof(struct elab_s));
	FILE* infile = fopen(filename, "r");

	elabs->E = 20;
	elabs->tab_names = (char**)malloc(elabs->E * sizeof(char*));

	while (fscanf(infile, "%s %*s %s %*s %*d", nome1, nome2) == 2) {
		i++;
		if (i > elabs->E) {
			elabs->E *= 2;
			elabs->tab_names = realloc(elabs->tab_names, elabs->E * sizeof(char*));
		}
		elabs->tab_names[i - 1] = strdup(nome1);
	}
}
