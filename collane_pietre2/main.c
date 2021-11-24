#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TESTFILE "test_set.txt"

enum pietra { any = -1, zaffiro = 0, rubino, smeraldo, topazio };

typedef struct {
	int elenco[4];		// { nz, nr, ns, nt }
	int valori[4];		// { vz, vr, vs, vt }
	int max_rip;
} info_pietre;

typedef struct {
	enum pietra* pietre;
	int lunghezza;
	int valore;
} collana;

void copia_collana(collana* target, const collana* from) {
	target->lunghezza = from->lunghezza;
	target->valore = from->valore;
	target->pietre = (enum pietra*)malloc(target->lunghezza * sizeof(enum pietra));
	memcpy(target->pietre, from->pietre, target->lunghezza * sizeof(enum pietra));
}

int check_pietra(collana* c, enum pietra inserire, info_pietre* info) {
	enum pietra ultima_inserita;
	int i;

	if (c->lunghezza > 0)
		ultima_inserita = c->pietre[c->lunghezza - 1];
	else
		ultima_inserita = any;

	switch (ultima_inserita)
	{
	case any:
		return 1;
	case zaffiro:
		for (i = c->lunghezza - 2; i >= 0 && c->pietre[i] == zaffiro; i--);
		if (c->lunghezza - 1 - i < info->max_rip)
			return inserire == zaffiro || inserire == rubino;
		else
			return inserire == rubino;
	case rubino:
		return inserire == smeraldo || inserire == topazio;
	case topazio:
		return inserire == zaffiro || inserire == rubino;
	case smeraldo:
		for (i = c->lunghezza - 2; i >= 0 && c->pietre[i] == smeraldo; i--);
		if (c->lunghezza - 1 - i < info->max_rip)
			return inserire == smeraldo || inserire == topazio;
		else
			return inserire == topazio;
	}
}

int check_numbers_zs(collana* c) {
	int nz = 0, ns = 0, i;
	for (i = 0; i < c->lunghezza; i++) {
		switch (c->pietre[i])
		{
		case zaffiro:
			nz++;
			break;
		case smeraldo:
			ns++;
			break;
		}
	}
	return nz <= ns;
}

collana add_pietra(info_pietre* elenco, collana* collana_attuale) {
	int i;
	collana best, tmp;

	copia_collana(&best, collana_attuale);

	for (i = 0; i < 4; i++) {
		if (elenco->elenco[i] > 0 && check_pietra(collana_attuale, i, elenco)) {
			elenco->elenco[i]--;
			collana_attuale->pietre[collana_attuale->lunghezza++] = i;
			collana_attuale->valore += elenco->valori[i];

			tmp = add_pietra(elenco, collana_attuale);
			if (tmp.valore > best.valore && check_numbers_zs(&tmp)) {
				free(best.pietre);
				best.pietre = tmp.pietre;
				best.lunghezza = tmp.lunghezza;
				best.valore = tmp.valore;
			}
			else
				free(tmp.pietre);

			elenco->elenco[i]++;
			collana_attuale->lunghezza--;
			collana_attuale->valore -= elenco->valori[i];
		}
	}

	return best;
}

collana make_collana(info_pietre* elenco, collana* c) {
	int i, j, num_giri = elenco->elenco[0], rotazione = 0;
	collana r, result;

	for (i = 1; i < 4; i++)
		if (elenco->elenco[i] < num_giri)
			num_giri = elenco->elenco[i];

	for (i = 0; i < 4; i++)
		elenco->elenco[i] -= num_giri;

	r = add_pietra(elenco, c);

	if (r.pietre[0] == smeraldo || r.pietre[0] == topazio)
		rotazione = 1;

	result.lunghezza = num_giri * 4 + r.lunghezza;
	result.valore = r.valore + (elenco->valori[0] + elenco->valori[1] + elenco->valori[2] + elenco->valori[3]) * num_giri;
	result.pietre = (enum pietra*)malloc(result.lunghezza * sizeof(enum pietra));

	for (i = 0; i < num_giri; i++)
		for (j = 0; j < 4; j++)
			result.pietre[i * 4 + j] = (j + rotazione) % 4;

	for (i = 0; i < r.lunghezza; i++)
		result.pietre[num_giri * 4 + i] = r.pietre[i];

	free(r.pietre);
	return result;
}

int main() {
	int N, i, ret_val, j;
	info_pietre elenco;
	collana c, res;
	FILE* infile = fopen(TESTFILE, "r");

	c.pietre = malloc(1); // per poter fare free nel for

	ret_val = fscanf(infile, "%d", &N);
	if (ret_val != 1) {
		printf("Input da file non valido\n");
		exit(-1);
	}

	for (i = 0; i < N; i++) {
		ret_val = fscanf(infile, "%d %d %d %d %d %d %d %d %d", &elenco.elenco[0], &elenco.elenco[1], &elenco.elenco[3], &elenco.elenco[2], &elenco.valori[0], &elenco.valori[1], &elenco.valori[3], &elenco.valori[2], &elenco.max_rip);
		if (ret_val != 9) {
			printf("Input da file non valido\n");
			exit(-1);
		}

		c.lunghezza = 0;
		c.valore = 0;
		free(c.pietre);
		c.pietre = (enum pietra*)malloc((elenco.elenco[0] + elenco.elenco[1] + elenco.elenco[2] + elenco.elenco[3]) * sizeof(enum pietra));
		res = add_pietra(&elenco, &c);

		printf("Collana %d\n- Valore: %d\n- Lunghezza: %d\n", i + 1, res.valore, res.lunghezza);
		for (j = 0; j < res.lunghezza; j++) {
			switch (res.pietre[j])
			{
			case zaffiro:
				printf("z-");
				break;
			case rubino:
				printf("r-");
				break;
			case topazio:
				printf("t-");
				break;
			case smeraldo:
				printf("s-");
				break;
			}
		}
		printf("\n\n");
		free(res.pietre);
	}

	fclose(infile);

	return 0;
}