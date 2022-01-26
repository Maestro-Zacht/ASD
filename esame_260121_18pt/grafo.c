#include "grafo.h"
#include "tabella.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSTR 51

typedef struct edge {
	int v, w, trap;
	link next;
} Edge;

typedef Edge* link;

struct path_s {
	link head, tail;
	int max_tesoro;
};

typedef struct {
	int profondità, tesoro, monete, monete_prese;
} node_info_s;

struct graph_s {
	int** mat_adj;
	int V, entrata;
	st_table table;
	node_info_s* info_stanze;
};

void PATHaddEdge(Path p, int v, int w) {
	link new_e = (Edge*)malloc(sizeof(Edge));

	new_e->v = v;
	new_e->w = w;
	new_e->trap = -1;
	new_e->next = NULL;
	
	if (p->tail == NULL)
		p->tail = p->head = new_e;
	else {
		p->tail->next = new_e;
		p->tail = new_e;
	}
}

void PATHremoveEdge(Path p) {
	link old = p->head;
	p->head = old->next;
	free(old);
}

void insertE(Graph G, int v, int w, int trap) {
	G->mat_adj[v][w] = G->mat_adj[w][v] = (trap) ? -1 : 1;
}

Graph GRAPHload(FILE* fp) {
	int i, trap, id1,id2;
	char nome1[MAXSTR], nome2[MAXSTR];
	Graph G = (struct graph_s*)malloc(sizeof(struct graph_s));

	fscanf(fp, "%d", &G->V);

	G->table = st_init(G->V);
	G->info_stanze = (node_info_s*)malloc(G->V * sizeof(node_info_s));
	G->mat_adj = (int**)malloc(G->V * sizeof(int*));

	for (i = 0; i < G->V; i++) {
		G->mat_adj[i] = (int*)calloc(G->V, sizeof(int));
		fscanf(fp, "%s %d %d %d", nome1, &G->info_stanze[i].profondità, &G->info_stanze[i].tesoro, &G->info_stanze[i].monete);
		st_insert(G->table, nome1);
		G->info_stanze[i].monete_prese = 0;
		if (G->info_stanze[i].profondità == 0)
			strcpy(nome2, nome1);
	}

	st_sort(G->table);

	G->entrata = st_search_by_id(G->table, nome2);

	while (!feof(fp) && fscanf(fp, "%s %s %d", nome1, nome2, &trap) == 3) {
		id1 = st_search_by_id(G->table, nome1);
		id2 = st_search_by_id(G->table, nome2);
		insertE(G, id1, id2, trap);
	}

	return G;
}

Path GRAPHpathLoad(Graph g, FILE* fp) {
	Path p = (struct path_s*)malloc(sizeof(struct path_s));
	char nome1[MAXSTR];
	int id, idnext;

	p->head = p->tail = NULL;
	p->max_tesoro = 0;

	fscanf(fp, "%s", nome1);
	id = st_search_by_id(g->table, nome1);
	if (id == g->entrata) {
		fscanf(fp, "%s", nome1);
		id = st_search_by_id(g->table, nome1);
	}
	PATHaddEdge(p, g->entrata, id);
	
	while (!feof(fp) && fscanf(fp, "%s", nome1) == 1) {
		idnext = st_search_by_id(g->table, nome1);
		PATHaddEdge(p, id, idnext);
		id = idnext;
	}

	return p;
}

int GRAPHpathCheck(Graph g, Path p, int M, int PF) {
	link i;
	int max_tesoro = 0, last_room;

	p->max_tesoro = 0;

	for (i = p->head; i != NULL && M >= 0 && PF >= 0; i = i->next) {
		if (g->mat_adj[i->v][i->w] == 0) {
			p->max_tesoro = 0;
			return 0;
		}

		if (g->mat_adj[i->v][i->w] == -1)
			PF--;

		if (!g->info_stanze[i->w].monete_prese) {
			p->max_tesoro += g->info_stanze[i->w].monete;
			g->info_stanze[i->w].monete_prese = 1;
		}

		if (g->info_stanze[i->w].tesoro > max_tesoro)
			max_tesoro = g->info_stanze[i->w].tesoro;

		M--;
		last_room = i->w;
	}

	if (M >= 0 && PF >= 0) {
		if (g->info_stanze[last_room].profondità >= 3)
			p->max_tesoro = 0;
		else if (g->info_stanze[last_room].profondità == 2)
			p->max_tesoro = (p->max_tesoro + 1) / 2;
		else if (g->info_stanze[last_room].profondità == 1)
			p->max_tesoro = (int)ceilf(p->max_tesoro / 3.0);
	}
	else
		p->max_tesoro = 0;

	return M >= 0 && PF >= 0;
}

Path GRAPHpathBestR(Graph g, int M, int PF, int stanza) {

}

Path GRAPHpathBest(Graph g, int M, int PF) {

}
