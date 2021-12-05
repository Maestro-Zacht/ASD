#pragma once

#define MAXSTR 31

typedef struct elab_s* elaboratori_t;

elaboratori_t elab_init(const char* filename);
void elab_free(elaboratori_t elabs);
