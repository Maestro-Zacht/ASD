#include <stdio.h>
#include <string.h>

#define MAXR 1000
#define MAXSTR 31

typedef enum comando { r_stampa = 1, r_ordina_data, r_ordina_cod, r_ordina_part, r_ordina_arr, r_ricerca, r_exit } comando;

enum ordinamento { o_unsorted, o_data, o_cod, o_part, o_arr };


struct ora {
	int ore, minuti, secondi;
};

struct data {
	int anno, mese, giorno;
};

typedef struct corsa {
	char codice[MAXSTR], partenza[MAXSTR], destinazione[MAXSTR];
	int ritardo;
	struct data data;
	struct ora ora_fine, ora_inizio;
} corsa;

int leggiFile(const char* fn, corsa* dati) {
	int righe, i, retVal;

	FILE* infile = fopen(fn, "r");

	fscanf(infile, "%d", &righe);

	for (i = 0; i < righe; i++) {
		corsa* elem = dati + i;
		retVal = fscanf(infile, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", &elem->codice, &elem->partenza, &elem->destinazione, &elem->data.anno, &elem->data.mese, &elem->data.giorno, &elem->ora_inizio.ore, &elem->ora_inizio.minuti, &elem->ora_inizio.secondi, &elem->ora_fine.ore, &elem->ora_fine.minuti, &elem->ora_fine.secondi, &elem->ritardo);
		if (retVal != 13)
			printf("Errore!\n");
	}


	fclose(infile);

	return righe;
}

void stampaCorsa(const corsa* c, FILE const* stream) {
	fprintf(stream, "codice: %s\npartenza: %s\narrivo: %s\ndata: %04d/%02d/%02d\nora di partenza: %02d:%02d:%02d\nora di arrivo: %02d:%02d:%02d\nritardo (min): %d\n\n", c->codice, c->partenza, c->destinazione, c->data.anno, c->data.mese, c->data.giorno, c->ora_inizio.ore, c->ora_inizio.minuti, c->ora_inizio.secondi, c->ora_fine.ore, c->ora_fine.minuti, c->ora_fine.secondi, c->ritardo);
}

int datecmp(struct data a, struct data b) {
	if (a.anno > b.anno)
		return 3;
	else if (a.anno < b.anno)
		return -3;
	else if (a.mese > b.mese)
		return 2;
	else if (a.mese < b.mese)
		return -2;
	else if (a.giorno > b.giorno)
		return 1;
	else if (a.giorno < b.giorno)
		return -1;
	else
		return 0;
}


int oracmp(struct ora a, struct ora b) {
	if (a.ore > b.ore)
		return 3;
	else if (a.ore < b.ore)
		return -3;
	else if (a.minuti > b.minuti)
		return 2;
	else if (a.minuti < b.minuti)
		return -2;
	else if (a.secondi > b.secondi)
		return 1;
	else if (a.secondi < b.secondi)
		return -1;
	else
		return 0;
}

int dateoracmp(corsa a, corsa b) {
	int r = datecmp(a.data, b.data);

	if (r == 0)
		r = oracmp(a.ora_inizio, b.ora_inizio);

	if (r == 0)
		r = oracmp(a.ora_fine, b.ora_fine);

	return r;
}



void bubbleSort_date(corsa* A, int N) {
	int i, j, continua = 1, current_swap = -1, last_swap = N - 1;
	corsa tmp;
	for (i = 0; i < N - 1 && continua; i++) {
		continua = 0;
		for (j = 0; j < last_swap; j++) {
			if (dateoracmp(A[j], A[j + 1]) > 0) {
				continua = 1;
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
				current_swap = j;
			}
		}
		last_swap = current_swap;
	}
}

void bubbleSort_arrivo(corsa* A, int N) {
	int i, j, continua = 1, current_swap = -1, last_swap = N - 1;
	corsa tmp;
	for (i = 0; i < N - 1 && continua; i++) {
		continua = 0;
		for (j = 0; j < last_swap; j++) {
			if (strcmp(A[j].destinazione, A[j+1].destinazione) > 0) {
				continua = 1;
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
				current_swap = j;
			}
		}
		last_swap = current_swap;
	}
}

void bubbleSort_partenza(corsa* A, int N) {
	int i, j, continua = 1, current_swap = -1, last_swap = N - 1;
	corsa tmp;
	for (i = 0; i < N - 1 && continua; i++) {
		continua = 0;
		for (j = 0; j < last_swap; j++) {
			if (strcmp(A[j].partenza, A[j + 1].partenza) > 0) {
				continua = 1;
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
				current_swap = j;
			}
		}
		last_swap = current_swap;
	}
}

void bubbleSort_tratta(corsa* A, int N) {
	int i, j, continua = 1, current_swap = -1, last_swap = N - 1;
	corsa tmp;
	for (i = 0; i < N - 1 && continua; i++) {
		continua = 0;
		for (j = 0; j < last_swap; j++) {
			if (strcmp(A[j].codice, A[j + 1].codice) > 0) {
				continua = 1;
				tmp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = tmp;
				current_swap = j;
			}
		}
		last_swap = current_swap;
	}
}


int ricerca_lineare(corsa* dati, int N, const char* src, int ris[]) {
	int len = strlen(src), i, j = 0;

	for (i = 0; i < N; i++) {
		if (strncmp(dati[i].partenza, src, len) == 0) {
			ris[j] = i;
			j++;
		}
	}

	return j;
}


int ricerca_dicotomica(corsa* dati, int N, const char* src, int ris[]) {
	int len = strlen(src), lower = 0, upper = N - 1, med, i, j = 0;

	if (strncmp(dati[lower].partenza, src, len) > 0 || strncmp(dati[upper].partenza, src, len) < 0)
		return 0;

	while (lower <= upper)
	{
		med = (lower + upper) / 2;
		if (strncmp(dati[med].partenza, src, len) == 0) {
			ris[j] = med;
			j++;

			for (i = med - 1; i < N && strncmp(dati[i].partenza, src, len) == 0; i--) {
				ris[j] = i;
				j++;
			}

			for (i = med + 1; i < N && strncmp(dati[i].partenza, src, len) == 0; i++) {
				ris[j] = i;
				j++;
			}
			
			lower = upper + 1;
		}
		else if (strncmp(dati[med].partenza, src, len) < 0)
			lower = med + 1;
		else
			upper = med - 1;
	}

	return j;
}


int main() {
	corsa dati[MAXR];
	int righe = leggiFile("log.txt", dati), i, s, ris[MAXR], n;
	comando scelta;
	enum ordinamento ord = o_unsorted;
	char src[MAXSTR];


	do
	{
		printf("Inserire uno dei seguenti numeri per eseguire l\'operazione relativa\n1) stampa tutti i dati\n2) ordinamento per data\n3) ordinamento per codice\n4) ordinamento per stazione di partenza\n5) ordinamento per stazione di arrivo\n6) ricerca per stazione di partenza\n7) esci\n\n");
		scanf("%d", &scelta);

		switch (scelta)
		{
		case r_stampa:
		{


			do
			{
				printf("Inserire 1 per stampare a schermo, 2 per stampare su file\n");
				scanf("%d", &s);
			} while (s < 1 || s > 2);

			if(s == 1)
				for (i = 0; i < righe; i++)
					stampaCorsa(dati + i, stdout);
			else
			{
				FILE* outfile;
				char fn[100];

				printf("Inserire il nome del file\n");
				scanf("%s", fn);

				outfile = fopen(fn, "w");


				for (i = 0; i < righe; i++)
					stampaCorsa(dati + i, outfile);

				fclose(outfile);
			}

			getc(stdin); // problemi di input, \n fa andare in loop
			break;
		}

		case r_ordina_data:
			bubbleSort_date(dati, righe);
			ord = o_data;

			for (i = 0; i < righe; i++)
				stampaCorsa(dati + i, stdout);
			break;

		case r_ordina_cod:
			bubbleSort_tratta(dati, righe);
			ord = o_cod;

			for (i = 0; i < righe; i++)
				stampaCorsa(dati + i, stdout);
			break;

		case r_ordina_part:
			bubbleSort_partenza(dati, righe);
			ord = o_part;

			for (i = 0; i < righe; i++)
				stampaCorsa(dati + i, stdout);
			break;

		case r_ordina_arr:
			bubbleSort_arrivo(dati, righe);
			ord = o_arr;

			for (i = 0; i < righe; i++)
				stampaCorsa(dati + i, stdout);
			break;

		case r_ricerca:
		{
			

			printf("inserire il testo da cercare\n");
			scanf("%s", src);

			if (ord == o_part) {
				n = ricerca_dicotomica(dati, righe, src, ris);
			}
			else 
				n = ricerca_lineare(dati, righe, src, ris);

			for (i = 0; i < n; i++)
				stampaCorsa(dati + ris[i], stdout);

			break;
		}
		case r_exit:
			break;
		default:
			printf("Scelta non valida\n");
			break;
		}
	} while (scelta != r_exit);

	return 0;
}