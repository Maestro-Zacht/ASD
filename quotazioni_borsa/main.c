#include "titles.h"
#include "title.h"
#include "datetime.h"
#include <stdio.h>

#define MAXFNLEN 51

enum comando { esci = 0, read_file, search_titolo, qt_search, daterange_search, min_max_search, balance };


int main() {
	FILE* infile;
	char filename[MAXFNLEN], titlename[MAXSTR];
	enum comando scelta;
	float soglia;
	data_t data1, data2;
	title_t titolo_sel = NULL;
	titles_l titles = titles_init();

	do {
		if (titolo_sel == NULL)
			printf("\n----- Nessun titolo selezionato -----\n\n");
		else
			printf("\n----- Titolo selezionato: %s -----\n\n", title_key(titolo_sel));
		printf("0) Esci\n1) Acquisizione di dati da file\n2) Selezione di un titolo\n3) Ricerca di quotazione del titolo\n4) Ricerca della quotazione minima e massima del titolo in un intervallo di date\n5) Ricerca della quotazione minima e massima del titolo globalmente\n6) Bilanciamento delle quotazioni\n");
		scanf("%d", &scelta);
		getchar();

		switch (scelta)
		{
		case esci:
			break;
		case read_file:
			printf("Inserire il nome del file (max %d caratteri): ", MAXFNLEN - 1);
			scanf("%s", filename);
			infile = fopen(filename, "r");

			titles_read(infile, titles);

			fclose(infile);
			break;
		case search_titolo:
			printf("Inserire il nome del titolo (max %d caratteri): ", MAXSTR - 1);
			scanf("%s", titlename);
			titolo_sel = titles_search(titles, titlename);
			if (titolo_sel == NULL)
				printf("Titolo non trovato\n");
			break;
		case qt_search:
			if (titolo_sel == NULL)
				printf("Nessun titolo selezionato\n");
			else {
				printf("Inserire la data nel formato aaaa/mm/gg hh:mm\n");
				data1 = date_read(stdin);
				title_print_quotation(stdout, titolo_sel, data1);
			}
			break;
		case daterange_search:
			if (titolo_sel == NULL)
				printf("Nessun titolo selezionato\n");
			else {
				printf("Inserire le 2 date nel formato aaaa/mm/gg hh:mm\n");
				data1 = date_read(stdin);
				data2 = date_read(stdin);
				if (datecmp(data1, data2) > 0)
					title_print_quotations_range(stdout, titolo_sel, data2, data1);
				else
					title_print_quotations_range(stdout, titolo_sel, data1, data2);
			}
			break;
		case min_max_search:
			title_print_quotations_min_max(stdout, titolo_sel);
			break;
		case balance:
			printf("Inserire la soglia: ");
			scanf("%f", &soglia);
			title_balance(titolo_sel, soglia);
			break;
		default:
			printf("Scelta non valida\n");
			break;
		}
	} while (scelta != esci);


	titles_free(titles);

	return 0;
}