#include <stdio.h>
#include "personaggi.h"
#include "inventario.h"

enum comando { exit = 0, cambia_pg, add_pg, del_pg, add_equip, del_equip, info_stats, print_pgs, print_inv };

int main() {
	tabella_personaggi_t tabella_personaggi;
	inventario_t inventario_oggetti;
	personaggio_t pg_tmp;
	enum comando c;
	char scelta, new_oggetto[MAXSTR];
	int tmp, ret_val, i;
	nodo_pg* pg_selezionato = NULL;
	FILE* infile = fopen("pg.txt", "r");

	tabella_personaggi = pg_setup_from_file(infile);
	fclose(infile);

	infile = fopen("inventario.txt", "r");

	inventario_oggetti = inv_setup_from_file(infile);

	fclose(infile);

	do {
		if (pg_selezionato == NULL)
			printf("\nNon e' stato selezionato nessun personaggio\n\n");
		else
			printf("\n-------- %s --------\n\n", pg_selezionato->personaggio.nome);

		printf("0) Esci\n1) Seleziona un personaggio\n2) Aggiungi un personaggio\n3) Elimina un personaggio\n4) Aggiungi equipaggiamento\n5) Rimuovi equipaggiamento\n6) Statistiche del personaggio\n7) Elenco dei personaggi\n8) Elenco degli oggetti\n");
		scanf("%d", &c);
		getchar();

		switch (c)
		{
		case exit:
			break;
		case cambia_pg:
			do {
				printf("Inserire il codice del personaggio (PGXXXX dove X e' una cifra 0-9): ");
				ret_val = scanf("PG%d", &tmp);
				getchar();
			} while (ret_val != 1 || tmp < 0 || tmp > 9999);
			pg_selezionato = pg_search(tmp, &tabella_personaggi);
			if (pg_selezionato == NULL)
				printf("Il personaggio non esiste\n");
			break;
		case add_pg:
			printf("Inserire le informazioni del personaggio nel formato\nPGXXXX <nome> <classe> <hp> <mp> <atk> <def> <mag> <spr>\n");
			pg_tmp = pg_input(stdin, &tabella_personaggi);
			if (pg_tmp.nome == tabella_personaggi.NULLPG.nome)
				printf("Input non valido\n");
			else {
				do {
					printf("Vuoi selezionare il personaggio inserito? (y/n): ");
					scanf(" %c", &scelta);
				} while (scelta != 'y' && scelta != 'n' && scelta != 'Y' && scelta != 'N');

				if (scelta == 'y' || scelta == 'Y')
					pg_selezionato = pg_insert(pg_tmp, &tabella_personaggi);
				else
					pg_insert(pg_tmp, &tabella_personaggi);
			}
			break;
		case del_pg:
			if (pg_selezionato == NULL) {
				do {
					printf("Inserire il codice del personaggio (PGXXXX dove X e' una cifra 0-9): ");
					ret_val = scanf("PG%d", &tmp);
					getchar();
				} while (ret_val != 1 || tmp < 0 || tmp > 9999);
				pg_selezionato = pg_search(tmp, &tabella_personaggi);
			}

			if (pg_selezionato == NULL)
				printf("Il personaggio non esiste\n");
			else {
				do {
					printf("Confermare di voler eliminare il personaggio PG%04d %s? (y/n): ", pg_selezionato->personaggio.codice, pg_selezionato->personaggio.nome);
					scanf("%c", &scelta);
					getchar();
				} while (scelta != 'y' && scelta != 'n' && scelta != 'Y' && scelta != 'N');

				if (scelta == 'y' || scelta == 'Y') {
					pg_delete(pg_selezionato->personaggio.codice, &tabella_personaggi);
					printf("Personaggio eliminato\n");
					pg_selezionato = NULL;
				}
				else
					printf("Eliminazione annullata\n");
			}
			break;
		case add_equip:
			if (pg_selezionato == NULL) {
				do {
					printf("Inserire il codice del personaggio (PGXXXX dove X e' una cifra 0-9): ");
					ret_val = scanf("PG%d", &tmp);
					getchar();
				} while (ret_val != 1 || tmp < 0 || tmp > 9999);
				pg_selezionato = pg_search(tmp, &tabella_personaggi);
			}

			if (pg_selezionato == NULL)
				printf("Il personaggio non esiste\n");
			else {
				printf("\nInserire il nome dell'oggetto da equipaggiare: ");
				scanf("%s", new_oggetto);

				pg_add_equip(pg_selezionato, new_oggetto, inventario_oggetti);
			}
			break;
		case del_equip:
			if (pg_selezionato == NULL) {
				do {
					printf("Inserire il codice del personaggio (PGXXXX dove X e' una cifra 0-9): ");
					ret_val = scanf("PG%d", &tmp);
					getchar();
				} while (ret_val != 1 || tmp < 0 || tmp > 9999);
				pg_selezionato = pg_search(tmp, &tabella_personaggi);
			}

			if (pg_selezionato == NULL)
				printf("Il personaggio non esiste\n");
			else {
				printf("\nInserire il nome dell'oggetto da rimuovere: ");
				scanf("%s", new_oggetto);

				pg_remove_equip(pg_selezionato, new_oggetto);
			}
			break;
		case info_stats:
			if (pg_selezionato == NULL) {
				do {
					printf("Inserire il codice del personaggio (PGXXXX dove X e' una cifra 0-9): ");
					ret_val = scanf("PG%d", &tmp);
					getchar();
				} while (ret_val != 1 || tmp < 0 || tmp > 9999);
				pg_selezionato = pg_search(tmp, &tabella_personaggi);
			}

			if (pg_selezionato == NULL)
				printf("Il personaggio non esiste\n");
			else
				pg_details(pg_selezionato);
			break;
		case print_pgs:
			pg_print_list(tabella_personaggi);
			break;
		case print_inv:
			printf("Inventario:\n");
			for (i = 0; i < inventario_oggetti.n_inv; i++)
				print_item(stdout, inventario_oggetti.vettInv[i]);
			break;
		default:
			printf("Comando non valido\n");
			break;
		}
	} while (c != exit);

	free_inv(&inventario_oggetti);
	pg_free_list(&tabella_personaggi);

	return 0;
}