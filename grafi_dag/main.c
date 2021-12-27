#include <stdio.h>
#include "grafo.h"
#include "edge_lists.h"

#define MAXFNLEN 51

int main() {
	char filename[MAXFNLEN];
	FILE* infile;
	graph_t grafo, dag;
	set_edgelist_t archi_rimuovere;
	edgelist_t archi_min;

	printf("Inserire il nome del file (max %d caratteri): ", MAXFNLEN - 1);
	scanf("%s", filename);

	infile = fopen(filename, "r");
	grafo = graph_init(infile);
	fclose(infile);

	archi_rimuovere = graph_convert_to_dag(grafo);

	printf("\nInsieme di archi da rimuovere per ottenere un DAG:\n");
	graph_print_edges_list(grafo, archi_rimuovere, stdout);

	archi_min = set_elist_best_elist(archi_rimuovere);

	if (archi_min != NULL) {
		printf("Costruzione del DAG rimuovendo gli archi dall'insieme a peso massimo:\n");
		graph_print_edges(grafo, archi_min, stdout);
	}

	dag = graph_remove_edges(grafo, archi_min);

	printf("\nDAG:\n");
	graph_print(dag, stdout);

	printf("\nCalcolo delle distanze massime da ogni nodo sorgente\n");
	dag_print_max_dist(dag, stdout);

	graph_free(grafo);
	graph_free(dag);
	set_elist_free(archi_rimuovere);

	return 0;
}