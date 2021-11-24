#include "anagrafica.h"
#include <stdio.h>
#include <stdlib.h>

enum comando { esci = 0, get_tastiera, get_file, find_cod, estr_cod, estr_date, stampa };

int main() {
	LISTA l = new_lista();
	Item* new_one;
	FILE* file;
	data a, b;
	int codice;
	char filename[50];
	enum comando c;

	do {
		printf("\nInserire il numero corrispondente al comando:\n0) Esci\n1) Inserimento da tastiera di un nuovo elemento\n2) Inserimento da file di nuovi elementi\n3) Ricerca di un elemento per codice\n4) Estrazione e stampa di un elemento dato il codice\n5) Estrazione e stampa degli elementi compresi in un intervallo di date\n6) Scrittura della lista su file\n\n");
		scanf("%d", &c);
		getchar();

		switch (c)
		{
		case get_tastiera:
			printf("Inserire il nuovo elemento nel formato\n<codice> <nome> <cognome> <data_di_nascita> <via> <citta'> <cap>\n");
			new_one = prendi_item(stdin);
			if (new_one->codice == l.NULLITEM.codice)
				printf("Elemento non valido");
			else
				inserimento_ordinato(&l, new_one);
			break;
		case get_file:
			printf("Inserire il nome del file: ");
			scanf("%s", filename);

			file = fopen(filename, "r");
			if (file == NULL || feof(file)) {
				printf("File non valido\n");
				exit(-1);
			}

			do
			{
				new_one = prendi_item(file);
				if (new_one != NULL)
					inserimento_ordinato(&l, new_one);
			} while (!feof(file) && new_one != NULL);

			fclose(file);
			break;
		case find_cod:
			do
			{
				printf("Inserire il codice: ");
				scanf("A%d", &codice);
				getchar();
			} while (codice < 0 || codice>9999);

			new_one = ricerca_codice(l, codice);
			if (new_one->codice != l.NULLITEM.codice)
				stampa_item(stdout, new_one);
			else
				printf("Elemento non trovato\n");	
			break;
		case estr_cod:
			do
			{
				printf("Inserire il codice: ");
				scanf("A%d", &codice);
			} while (codice < 0 || codice>9999);

			new_one = estrazione_codice(&l, codice);
			if (new_one->codice != l.NULLITEM.codice)
				stampa_item(stdout, new_one);
			else
				printf("Elemento non presente\n");
			clear_item(new_one);
			break;
		case estr_date:
			printf("Inserire le date separate da spazio nel formato\ndd/mm/aaaa\n");
			scanf("%d/%d/%d %d/%d/%d", &a.giorno, &a.mese, &a.anno, &b.giorno, &b.mese, &b.anno);

			do
			{
				if (datecmp(a, b) < 0)
					new_one = estrazione_data(&l, a, b);
				else
					new_one = estrazione_data(&l, b, a);

				if (new_one->codice != l.NULLITEM.codice) {
					stampa_item(stdout, new_one);
					clear_item(new_one);
				}


			} while (new_one->codice != l.NULLITEM.codice);


			break;
		case stampa:
			printf("Inserire il nome del file: ");
			scanf("%s", filename);

			file = fopen(filename, "w");
			stampa_lista(file, l);
			fclose(file);

			break;
		case esci:
			break;
		default:
			printf("Comando non valido\n");
			break;
		}
	} while (c != esci);

	clear_lista(l);

	return 0;
}