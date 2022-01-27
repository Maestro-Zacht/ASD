#include "corridoio.h"
#include <stdlib.h>

#include "scaffale.h"
#include "pacco.h"

struct corridoio_s {
	int C_s;
	Scaffali scaffali;
};

corridoio CORRIDOIOinit(FILE* fp, int K) {
	corridoio c = (struct corridoio_s*)malloc(sizeof(struct corridoio_s));
	int i;
	char codice[MAXSTR];
	
	fscanf(fp, "%d", &c->C_s);
	c->scaffali = SCAFFALIinit(K);

	for (i = 0; i < c->C_s; i++) {
		fscanf(fp, "%s", codice);
		SCAFFALIinsert(c->scaffali, codice);
	}

	return c;
}

int CORRIDOIOisSlotFree(corridoio c, char* scaffale, int slot) {
	return SCAFFALIisSlotFree(c->scaffali, scaffale, slot);
}

void CORRIDOIOinsertPacco(corridoio c, char* scaffale, int slot, char* pacco) {
	SCAFFALIinsertPacco(c->scaffali, scaffale, pacco, slot);
}

void CORRIDOIOremovePacco(corridoio c, char* scaffale, int slot) {
	SCAFFALIremovePacco(c->scaffali, scaffale, slot);
}
