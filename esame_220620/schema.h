#pragma once

#include <stdio.h>

#define MAXSTR 21

typedef struct parole* Parole;

typedef struct schema* Schema;

Schema SCHEMAload(FILE* infile);

void SCHEMAfree(Schema s);

int verificaSchema(Schema s, Parole p, char** m);

void solve(Schema s, Parole p);

Parole PAROLEload(FILE* infile);

int PAROLEcheck(Parole p, char* parola);

void PAROLEfree(Parole p);