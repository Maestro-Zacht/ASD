#pragma once

typedef struct scaffali_s* Scaffali;

Scaffali SCAFFALIinit(int K);
void SCAFFALIinsert(Scaffali s, char* key);
void SCAFFALIbalance(Scaffali s);

int SCAFFALIisSlotFree(Scaffali s, char* key, int slot);
void SCAFFALIinsertPacco(Scaffali s, char* key, char* pacco, int slot);
void SCAFFALIremovePacco(Scaffali s, char* key, int slot);