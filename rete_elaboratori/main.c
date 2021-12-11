#include <stdio.h>
#include <stdlib.h>
#include "elaboratori.h"

enum command { esci = 0, elenco, adiacenti, lista_adj };

int main(int argc, char** argv) {
	if (argc < 2)
		exit(-1);
	elaboratori_t elabs = elab_init(argv[1]);
	enum command scelta;
	char nome1[MAXSTR], nome2[MAXSTR], nome3[MAXSTR];

	do {
		printf("0) Esci\n1) Elenco di vertici e archi in ordine alfabetico\n2) Verifica se 3 vertici formano un sottografo completo\n3) Generazione lista delle adiacenze\n");
		scanf("%d", &scelta);
		getchar();

		switch (scelta)
		{
		case esci:
			break;
		case elenco:
			elab_print_edges(stdout, elabs);
			break;
		case adiacenti:
			printf("Inserire i nomi dei tre vertici separati da spazio\n");
			scanf("%s %s %s", nome1, nome2, nome3);
			if (elab_pair_adj_m(elabs, nome1, nome2, nome3))
				printf("I tre vertici sono adiacenti a coppie\n");
			else
				printf("I tre vertici non sono adiacenti a coppie\n");
			break;
		case lista_adj:
			elab_make_list_adj(elabs);
			break;
		default:
			printf("Comando non valido\n");
			break;
		}
	} while (scelta != esci);

	elab_free(elabs);

	return 0;
}