#include <stdio.h>
#include <stdlib.h>

#define FILENAME "hard_test_set.txt"

int**** fZ_m = NULL, **** fS_m = NULL, **** fR_m = NULL, **** fT_m = NULL;

int fZ(int z, int r, int t, int s);
int fS(int z, int r, int t, int s);
int fR(int z, int r, int t, int s);
int fT(int z, int r, int t, int s);

int main() {
	int N, i, nz, nr, nt, ns, ret_val, ris_parz, ris_max, iz, ir, it;
	FILE* infile = fopen(FILENAME, "r");

	fscanf(infile, "%d", &N);
	for (i = 0; i < N; i++) {
		ret_val = fscanf(infile, "%d %d %d %d", &nz, &nr, &nt, &ns);
		if (ret_val != 4) {
			printf("Errore in lettura da file\n");
			exit(-1);
		}
		
		fZ_m = (int****)malloc((nz + 1) * sizeof(int***));
		fR_m = (int****)malloc((nz + 1) * sizeof(int***));
		fS_m = (int****)malloc((nz + 1) * sizeof(int***));
		fT_m = (int****)malloc((nz + 1) * sizeof(int***));
		for (iz = 0; iz <= nz; iz++) {
			fZ_m[iz] = (int***)malloc((nr + 1) * sizeof(int**));
			fS_m[iz] = (int***)malloc((nr + 1) * sizeof(int**));
			fR_m[iz] = (int***)malloc((nr + 1) * sizeof(int**));
			fT_m[iz] = (int***)malloc((nr + 1) * sizeof(int**));
			for (ir = 0; ir <= nr; ir++) {
				fZ_m[iz][ir] = (int**)malloc((nt + 1) * sizeof(int*));
				fS_m[iz][ir] = (int**)malloc((nt + 1) * sizeof(int*));
				fR_m[iz][ir] = (int**)malloc((nt + 1) * sizeof(int*));
				fT_m[iz][ir] = (int**)malloc((nt + 1) * sizeof(int*));
				for (it = 0; it <= nt; it++) {
					fZ_m[iz][ir][it] = (int*)calloc(ns + 1, sizeof(int));
					fS_m[iz][ir][it] = (int*)calloc(ns + 1, sizeof(int));
					fT_m[iz][ir][it] = (int*)calloc(ns + 1, sizeof(int));
					fR_m[iz][ir][it] = (int*)calloc(ns + 1, sizeof(int));
				}
			}
		}

		ris_max = fZ(nz, nr, nt, ns);

		ris_parz = fR(nz, nr, nt, ns);
		if (ris_parz > ris_max)
			ris_max = ris_parz;

		ris_parz = fT(nz, nr, nt, ns);
		if (ris_parz > ris_max)
			ris_max = ris_parz;

		ris_parz = fS(nz, nr, nt, ns);
		if (ris_parz > ris_max)
			ris_max = ris_parz;

		printf("Collana %d: lunghezza massima %d\n", i + 1, ris_max);

		for (iz = 0; iz <= nz; iz++) {
			for (ir = 0; ir <= nr; ir++) {
				for (it = 0; it <= nt; it++) {
					free(fZ_m[iz][ir][it]);
					free(fS_m[iz][ir][it]);
					free(fT_m[iz][ir][it]);
					free(fR_m[iz][ir][it]);
				}
				free(fZ_m[iz][ir]);
				free(fS_m[iz][ir]);
				free(fT_m[iz][ir]);
				free(fR_m[iz][ir]);
			}
			free(fZ_m[iz]);
			free(fS_m[iz]);
			free(fT_m[iz]);
			free(fR_m[iz]);
		}
		free(fZ_m);
		free(fS_m);
		free(fT_m);
		free(fR_m);
	}

	fclose(infile);


	return 0;
}


int fZ(int z, int r, int t, int s) {
	int ris;
	if (z <= 0) {
		return 0;
	}
	if (fZ_m[z][r][t][s] != 0)
		return fZ_m[z][r][t][s];

	ris = 1 + max(fZ(z - 1, r, t, s), fR(z - 1, r, t, s));
	fZ_m[z][r][t][s] = ris;
	return ris;
}

int fS(int z, int r, int t, int s) {
	int ris;
	if (s <= 0) {
		return 0;
	}
	if (fS_m[z][r][t][s] != 0)
		return fS_m[z][r][t][s];
	ris = 1 + max(fS(z, r, t, s - 1), fT(z, r, t, s - 1));
	fS_m[z][r][t][s] = ris;
	return ris;
}

int fR(int z, int r, int t, int s) {
	int ris;
	if (r <= 0) {
		return 0;
	}
	if (fR_m[z][r][t][s] != 0)
		return fR_m[z][r][t][s];
	ris = 1 + max(fS(z, r - 1, t, s), fT(z, r - 1, t, s));
	fR_m[z][r][t][s] = ris;
	return ris;
}

int fT(int z, int r, int t, int s) {
	int ris;
	if (t <= 0) {
		return 0;
	}
	if (fT_m[z][r][t][s] != 0)
		return fT_m[z][r][t][s];
	ris = 1 + max(fZ(z, r, t - 1, s), fR(z, r, t - 1, s));
	fT_m[z][r][t][s] = ris;
	return ris;
}
