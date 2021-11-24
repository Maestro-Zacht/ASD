#include <stdio.h>
#include <string.h>

#define MAXR 1000
#define MAXSTR 31

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
	struct ora fine, inizio;
} corsa;

typedef enum comando { r_date = 1, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine } comando;

int leggiFile(const char* fn, corsa* dati) {
	int righe, i, retVal;

	FILE* infile = fopen(fn, "r");

	fscanf(infile, "%d", &righe);

	for (i = 0; i < righe; i++) {
		corsa* elem = dati + i;
		retVal = fscanf(infile, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", &elem->codice, &elem->partenza, &elem->destinazione, &elem->data.anno, &elem->data.mese, &elem->data.giorno, &elem->inizio.ore, &elem->inizio.minuti, &elem->inizio.secondi, &elem->fine.ore, &elem->fine.minuti, &elem->fine.secondi, &elem->ritardo);
		if (retVal != 13)
			printf("Errore!\n");
	}


	fclose(infile);

	return righe;
}

void stampaCorsa(const corsa* c) {
	printf("codice: %s\npartenza: %s\narrivo: %s\ndata: %04d/%02d/%02d\nora di partenza: %02d:%02d:%02d\nora di arrivo: %02d:%02d:%02d\nritardo (min): %d\n\n", c->codice, c->partenza, c->destinazione, c->data.anno, c->data.mese, c->data.giorno, c->inizio.ore, c->inizio.minuti, c->inizio.secondi, c->fine.ore, c->fine.minuti, c->fine.secondi, c->ritardo);
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

comando getcmd(const char* cmd) {
	comando r;
	if (strcmp(cmd, "date") == 0)
		r = r_date;
	else if (strcmp(cmd, "partenza") == 0)
		r = r_partenza;
	else if (strcmp(cmd, "capolinea") == 0)
		r = r_capolinea;
	else if (strcmp(cmd, "ritardo") == 0)
		r = r_ritardo;
	else if (strcmp(cmd, "ritardo_tot") == 0)
		r = r_ritardo_tot;
	else if (strcmp(cmd, "fine") == 0)
		r = r_fine;
	else
		r = -1;

	return r;
}


int main() {
	corsa dati[MAXR];
	int righe = leggiFile("log.txt", dati), i, retVal;
	char input[3 * MAXSTR], cmd[MAXSTR];

	comando scelta;

	do
	{
		printf("1.  elencare tutte le corse partite in un certo intervallo di date\nFormato:\ndate <data di inizio: aaaa/mm/gg> <data di fine: aaaa/mm/gg>\n\n2.  elencare tutti le corse partite da una certa fermata\nFormato:\npartenza <nome fermata>\n\n3.  elencare tutti le corse che fanno capolinea in una certa fermata\nFormato:\ncapolinea <nome fermata>\n\n4.  elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date\nFormato:\nritardo <data di inizio: aaaa/mm/gg> <data di fine: aaaa/mm/gg>\n\n5.  elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\nFormato:\nritardo_tot <codice tratta>\n\n6.  Esci\nFormato:\nfine\n\n");
		fgets(input, 3 * MAXSTR, stdin);
		
		sscanf(input, "%s", cmd);

		scelta = getcmd(cmd);

		switch (scelta)
		{
		case r_date:
		{
			struct data inizio, fine;

			retVal = sscanf(input, "%*s %d/%d/%d %d/%d/%d", &inizio.anno, &inizio.mese, &inizio.giorno, &fine.anno, &fine.mese, &fine.giorno);
			if (retVal != 6)
				printf("Errore nella sintassi del comando\n");
			else if (datecmp(inizio, fine) > 0)
				printf("Intervallo di date non valido\n");
			else {
				printf("\n\n");
				for (i = 0; i < righe; i++)
					if (datecmp(inizio, dati[i].data) <= 0 && datecmp(fine, dati[i].data) >= 0)
						stampaCorsa(dati + i);
			}

			break;
		}
		case r_partenza:
		{
			char partenza[MAXSTR];

			retVal = sscanf(input, "%*s %s", partenza);
			if (retVal != 1)
				printf("Errore nella sintassi del comando\n");
			else
			{
				for (i = 0; i < righe; i++)
					if (strcmp(partenza, dati[i].partenza) == 0)
						stampaCorsa(dati + i);
			}

			break;
		}
		case r_capolinea:
		{
			char arrivo[MAXSTR];

			retVal = sscanf(input, "%*s %s", arrivo);
			if (retVal != 1)
				printf("Errore nella sintassi del comando\n");
			else {
				for (i = 0; i < righe; i++)
					if (strcmp(arrivo, dati[i].destinazione) == 0)
						stampaCorsa(dati + i);
			}

			break;
		}
		case r_ritardo:
		{
			struct data inizio, fine;

			retVal = sscanf(input, "%*s %d/%d/%d %d/%d/%d", &inizio.anno, &inizio.mese, &inizio.giorno, &fine.anno, &fine.mese, &fine.giorno);
			if (retVal != 6)
				printf("Errore nella sintassi del comando\n");
			else if (datecmp(inizio, fine) > 0)
				printf("Intervallo di date non valido\n");
			else {
				for (i = 0; i < righe; i++) {
					if (dati[i].ritardo > 0 && datecmp(inizio, dati[i].data) <= 0 && datecmp(fine, dati[i].data) >= 0)
						stampaCorsa(dati + i);
				}
			}

			break;
		}
		case r_ritardo_tot:
		{
			char tratta[MAXSTR];
			int ritardo_tot = 0;

			retVal = sscanf(input, "%*s %s", tratta);
			if (retVal != 1)
				printf("Errore nella sintassi del comando\n");
			else {
				for (i = 0; i < righe; i++)
					if (strcmp(tratta, dati[i].codice) == 0)
						ritardo_tot += dati[i].ritardo;

				printf("Il ritardo accumulato e' %d\n", ritardo_tot);
			}
			
			break;
		}
		case r_fine:
			break;
		default:
		{
			printf("Scelta non valida\n");
			break;
		}
		}

		
	} while (scelta != r_fine);

	return 0;
}