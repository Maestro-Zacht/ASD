#pragma once

#include <stdio.h>

#define MAXSTR 51

typedef struct pacchi_s* Pacchi;

typedef struct item_p {
	char key[MAXSTR], scaffale[MAXSTR];
	int corridoio, slot;
} pacco_item;

pacco_item pacco_item_load(FILE* fp);
int pacco_item_check_null(pacco_item data);

Pacchi PACCHIinit();
void PACCHIinsert(Pacchi p, pacco_item data);

pacco_item PACCHIsearch(Pacchi p, char* key);
void PACCHIextract(Pacchi p, char* key);
void PACCHIposition(Pacchi p, char* key, int corridoio, char* scaffale, int slot);

void PACCHIbalance(Pacchi p);