#include "inventario.h"
#include <stdlib.h>
#include <string.h>

inventario_t inv_setup_from_file(const FILE* infile) {
	inventario_t inv;
	char nome[MAXSTR], tipologia[MAXSTR];
	int i;

	if (infile == NULL || feof(infile))
		exit(-1);

	fscanf(infile, "%d", &inv.n_inv);

	inv.vettInv = (oggetto_t*)malloc(inv.n_inv * sizeof(oggetto_t));
	inv.NULLOBJ.nome = inv.NULLOBJ.tipologia = NULL;
	inv.NULLOBJ.statistiche.atk = inv.NULLOBJ.statistiche.def = inv.NULLOBJ.statistiche.hp = inv.NULLOBJ.statistiche.mag = inv.NULLOBJ.statistiche.mp = inv.NULLOBJ.statistiche.spr = 0;

	for (i = 0; i < inv.n_inv; i++) {
		fscanf(infile, "%s %s %d %d %d %d %d %d", nome, tipologia, &inv.vettInv[i].statistiche.hp, &inv.vettInv[i].statistiche.mp, &inv.vettInv[i].statistiche.atk, &inv.vettInv[i].statistiche.def, &inv.vettInv[i].statistiche.mag, &inv.vettInv[i].statistiche.spr);
		inv.vettInv[i].nome = strdup(nome);
		inv.vettInv[i].tipologia = strdup(tipologia);
	}

	return inv;
}

oggetto_t* find_item(const char* name, inventario_t inventario) {
	int i;
	for (i = 0; i < inventario.n_inv; i++)
		if (strcmp(name, inventario.vettInv[i].nome) == 0)
			return &inventario.vettInv[i];

	return &inventario.NULLOBJ;
}

void print_item(const FILE* fp, oggetto_t item) {
	fprintf(fp, "------------------------------\n%s (%s):\n", item.nome, item.tipologia);
	print_stats(fp, item.statistiche);
}

void print_stats(const FILE* fp, stat_t stat) {
	fprintf(fp, "Hp:\t%-5d\tMp:\t%-5d\nAtk:\t%-5d\tDef:\t%-5d\nMag:\t%-5d\tSpr:\t%-5d\n", stat.hp, stat.mp, stat.atk, stat.def, stat.mag, stat.spr);
}


void free_inv(inventario_t* inventario) {
	int i;
	for (i = 0; i < inventario->n_inv; i++) {
		free(inventario->vettInv[i].nome);
		free(inventario->vettInv[i].tipologia);
	}
	free(inventario->vettInv);
}