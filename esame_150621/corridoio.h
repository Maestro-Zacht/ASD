#pragma once

#include <stdio.h>

typedef struct corridoio_s* corridoio;

corridoio CORRIDOIOinit(FILE* fp, int K);
int CORRIDOIOisSlotFree(corridoio c, char* scaffale, int slot);
void CORRIDOIOinsertPacco(corridoio c, char* scaffale, int slot, char* pacco);
void CORRIDOIOremovePacco(corridoio c, char* scaffale, int slot);