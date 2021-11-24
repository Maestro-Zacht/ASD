#include "pg.h"

int pg_read(FILE* fp, pg_t* pgp) {
	if (fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe) != 3)
		return 0;
	stat_read(fp, &pgp->b_stat);
	pgp->eq_stat = pgp->b_stat;
	pgp->equip = equipArray_init();
	return 1;
}

void pg_clean(pg_t* pgp) {
	equipArray_free(pgp->equip);
}

void pg_print(FILE* fp, pg_t* pgp, invArray_t invArray) {
	int i;
	if (pgp == NULL)
		fprintf(fp, "Personaggio non presente\n");
	else {
		fprintf(fp, "\n%s\n%s (%s)\n\nEquipaggiamento:\n", pgp->cod, pgp->nome, pgp->classe);
		equipArray_print(fp, pgp->equip, invArray);
		fprintf(fp, "\nStatistiche base:\n");
		stat_print(fp, &pgp->b_stat);
		fprintf(fp, "\nStatistiche con equipaggiamento:\n");
		stat_print(fp, &pgp->eq_stat);
	}
}

void pg_updateEquip(pg_t* pgp, invArray_t invArray) {
	int i;
	inv_t* item;
	stat_t statistiche;
	if (pgp != NULL) {
		equipArray_update(pgp->equip, invArray);
		statistiche = equipArray_getStatModifiers(pgp->equip, invArray);

		pgp->eq_stat.atk = pgp->b_stat.atk + statistiche.atk;
		if (pgp->eq_stat.atk < MIN_STAT)
			pgp->eq_stat.atk = MIN_STAT;

		pgp->eq_stat.def = pgp->b_stat.def + statistiche.def;
		if (pgp->eq_stat.def < MIN_STAT)
			pgp->eq_stat.def = MIN_STAT;

		pgp->eq_stat.hp = pgp->b_stat.hp + statistiche.hp;
		if (pgp->eq_stat.hp < MIN_STAT)
			pgp->eq_stat.hp = MIN_STAT;

		pgp->eq_stat.mp = pgp->b_stat.mp + statistiche.mp;
		if (pgp->eq_stat.mp < MIN_STAT)
			pgp->eq_stat.mp = MIN_STAT;

		pgp->eq_stat.mag = pgp->b_stat.mag + statistiche.mag;
		if (pgp->eq_stat.mag < MIN_STAT)
			pgp->eq_stat.mag = MIN_STAT;

		pgp->eq_stat.spr = pgp->b_stat.spr + statistiche.spr;
		if (pgp->eq_stat.spr < MIN_STAT)
			pgp->eq_stat.spr = MIN_STAT;

	}
}
