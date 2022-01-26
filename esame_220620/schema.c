#include "schema.h"
#include <stdlib.h>
#include <string.h>

typedef struct collocazione_s {
	int r, c, orizzontale, lunghezza;
} collocazione;

struct schema {
	int R, C, N;
	collocazione* collocazioni;
};

typedef struct soluzione_s {
	char** m;
} soluzione;

typedef struct node_s* link;

struct node_s {
	char* parola;
	int usata;
	link next;
};

struct parole {
	link* lista;
};

link add_parola(link l, char* parola) {
	link new_node = (struct node_s*)malloc(sizeof(struct node_s));
	new_node->next = l;
	new_node->usata = 0;
	new_node->parola = strdup(parola);
	return new_node;
}

Parole PAROLEload(FILE* infile) {
	int N, i, len;
	char parola[MAXSTR];
	Parole p = (struct parole*)malloc(sizeof(struct parole));

	p->lista = (link*)malloc((MAXSTR - 1) * sizeof(link));
	for (i = 0; i < MAXSTR - 1; i++)
		p->lista[i] = NULL;

	fscanf(infile, "%d", &N);

	for (i = 0; i < N; i++) {
		fscanf(infile, "&s", parola);
		len = strlen(parola);
		p->lista[len - 1] = add_parola(p->lista[len - 1], parola);
	}

	return p;
}

int PAROLEcheck(Parole p, char* parola) {
	int len = strlen(parola);
	link i;
	for (i = p->lista[len - 1]; i != NULL; i = i->next)
		if (strcmp(parola, i->parola) == 0)
			return 1;
	return 0;
}

void PAROLEfree(Parole p) {
	link l, next;
	int i;
	for (i = 0; i < MAXSTR - 1; i++) {
		for (l = p->lista[i]; l != NULL; l = next) {
			next = l->next;
			free(l->parola);
			free(l);
		}
	}
	free(p->lista);
	free(p);
}


Schema SCHEMAload(FILE* infile) {
	int i;
	char orizzontale;
	Schema s = (struct schema*)malloc(sizeof(struct schema));

	fscanf(infile, "%d %d %d", &s->R, &s->C, &s->N);

	s->collocazioni = (collocazione*)malloc(s->N * sizeof(collocazione));

	for (i = 0; i < s->N; i++) {
		fscanf(infile, "%d %d %d %c", &s->collocazioni[i].lunghezza, &s->collocazioni[i].r, &s->collocazioni[i].c, &orizzontale);
		if (orizzontale == 'O')
			s->collocazioni[i].orizzontale = 1;
		else
			s->collocazioni[i].orizzontale = 0;
	}

	return s;
}

void SCHEMAfree(Schema s) {
	free(s->collocazioni);
	free(s);
}

int verificaSchema(Schema s, Parole p, char** m) {
	int i, r, c, orizz, len, j;
	char parola[MAXSTR], lettera;

	for (i = 0; i < s->N; i++) {
		r = s->collocazioni[i].r;
		c = s->collocazioni[i].c;
		orizz = s->collocazioni[i].orizzontale;
		len = s->collocazioni[i].lunghezza;
		for (j = 0; j < len; j++) {
			lettera = (orizz) ? m[r][c + j] : m[r + j][c];
			if (lettera < 65 || lettera > 90)
				return 0;

			parola[j] = lettera;
		}
		parola[len] = '\0';
		if (!PAROLEcheck(p, parola))
			return 0;
	}

	return 1;
}

int checkSol(Schema s, Parole p, soluzione sol) {
	return verificaSchema(s, p, sol.m);
}

void solve_r(Schema s, Parole p, soluzione sol, int pos, int* trovato) {
	int r, c, orizz, len, j;
	link i;

	if (pos >= s->N)
		*trovato = checkSol(s, p, sol);
	else {
		r = s->collocazioni[pos].r;
		c = s->collocazioni[pos].c;
		orizz = s->collocazioni[pos].orizzontale;
		len = s->collocazioni[pos].lunghezza;
		for (i = p->lista[len - 1]; i != NULL && !(*trovato); i = i->next) {
			if (!i->usata) {
				i->usata = 1;
				for (j = 0; j < len; j++) {
					if (orizz)
						sol.m[r][c + j] = i->parola[j];
					else
						sol.m[r + j][c] = i->parola[j];
				}
				solve_r(s, p, sol, pos + 1, trovato);
				i->usata = 0;
			}
		}
	}
}

void solve(Schema s, Parole p) {
	soluzione sol;
	int i, j, trovato = 0;

	sol.m = (char**)malloc(s->R * sizeof(char*));
	for (i = 0; i < s->R; i++) {
		sol.m[i] = (char*)malloc(s->C * sizeof(char));
		for (j = 0; j < s->C; j++)
			sol.m[i][j] = '\0';
	}

	solve_r(s, p, sol, 0, &trovato);

	if (trovato) {
		printf("Soluzione trovata:\n");
		for (i = 0; i < s->R; i++) {
			for (j = 0; j < s->C; j++) {
				if (sol.m[i][j] == '\0')
					printf("  ");
				else
					printf("%c ", sol.m[i][j]);
			}
			printf("\n");
		}
	}
	else
		printf("Soluzione non trovata\n");

	for (i = 0; i < s->R; i++)
		free(sol.m[i]);
	free(sol.m);
}
