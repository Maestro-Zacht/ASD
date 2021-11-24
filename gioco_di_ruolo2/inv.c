#include "inv.h"

void stat_read(FILE* fp, stat_t* statp) {
	if (fp == NULL || fscanf(fp, "%d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr) != 6)
		exit(-1);

	if (statp->atk < MIN_STAT)
		statp->atk = MIN_STAT;

	if (statp->def < MIN_STAT)
		statp->def = MIN_STAT;

	if (statp->hp < MIN_STAT)
		statp->hp = MIN_STAT;

	if (statp->mp < MIN_STAT)
		statp->mp = MIN_STAT;

	if (statp->mag < MIN_STAT)
		statp->mag = MIN_STAT;

	if (statp->spr < MIN_STAT)
		statp->spr = MIN_STAT;
}

void stat_print(FILE* fp, stat_t* statp) {
	fprintf(fp, "Hp:\t%-5d\tMp:\t%-5d\nAtk:\t%-5d\tDef:\t%-5d\nMag:\t%-5d\tSpr:\t%-5d\n", statp->hp, statp->mp, statp->atk, statp->def, statp->mag, statp->spr);
}

void inv_read(FILE* fp, inv_t* invp) {
	if (fp == NULL || fscanf(fp, "%s %s", invp->nome, invp->tipo) != 2)
		exit(-1);
	stat_read(fp, &invp->stat);
}

void inv_print(FILE* fp, inv_t* invp) {
	fprintf(fp, "------------------------------\n%s (%s):\n", invp->nome, invp->tipo);
	stat_print(fp, &invp->stat);
}

stat_t inv_getStat(inv_t* invp) {
	return invp->stat;
}
