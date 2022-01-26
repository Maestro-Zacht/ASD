#pragma once

#include <stdio.h>

typedef struct graph_s* Graph;
typedef struct path_s* Path;

Graph GRAPHload(FILE* fp);

Path GRAPHpathLoad(Graph g, FILE* fp);
int GRAPHpathCheck(Graph g, Path p, int M, int PF);

Path GRAPHpathBest(Graph g, int M, int PF);