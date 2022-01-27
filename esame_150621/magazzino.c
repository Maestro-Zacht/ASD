#include "magazzino.h"
#include <stdlib.h>
#include <string.h>

#include "corridoio.h"

struct corridoi_s {
	corridoio* corridoi;
	int C;
	Pacchi pacchi;
};

Magazzino MAGAZZINOinit(FILE* fp) {
	char codice[MAXSTR];
	int i, j, K, C_s;
	Magazzino m = (struct corridoi_s*)malloc(sizeof(struct corridoi_s));

	fscanf("%d %d", &m->C, &K);
	m->corridoi = (corridoio*)malloc(m->C * sizeof(corridoio));

	for (i = 0; i < m->C; i++)
		m->corridoi[i] = CORRIDOIOinit(fp, K);

	m->pacchi = PACCHIinit();

	return m;
}

void MAGAZZINOaddPacco(Magazzino m, pacco_item data) {
	if (pacco_item_check_null(PACCHIsearch(m->pacchi, data.key))) {
	}
	else
		printf("Pacco già inserito\n");
}

void MAGAZZINOremovePacco(Magazzino m, char* pacco) {
	pacco_item data = PACCHIsearch(m->pacchi, pacco);
	CORRIDOIOremovePacco(m->corridoi[data.corridoio], data.scaffale, data.slot);
	PACCHIextract(m->pacchi, data.key);
}

void MAGAZZINOinsertPacco(Magazzino m, pacco_item data) {
	if (CORRIDOIOisSlotFree(m->corridoi[data.corridoio], data.scaffale, data.slot)) {
		PACCHIinsert(m->pacchi, data);
		CORRIDOIOinsertPacco(m->corridoi[data.corridoio], data.scaffale, data.slot, data.key);
	}
	else
		printf("Slot occupato\n");
}

void MAGAZZINOmovePacco(Magazzino m, pacco_item data) {
	if (CORRIDOIOisSlotFree(m->corridoi[data.corridoio], data.scaffale, data.slot)) {
		MAGAZZINOremovePacco(m, data.key);
		MAGAZZINOinsertPacco(m, data);
	}
	else
		printf("Slot occupato");
}
