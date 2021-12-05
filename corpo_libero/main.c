#include <stdio.h>
#include "programma.h"
#include "elemento.h"

#define ELEMFILENAME "elementi.txt"
#define TESTSETFILENAME "testset_e_risultati.txt"

int main() {
	programma_t prog;
	elementi_l elementi;
	int DD, DP, retv, i;
	float TOT_EXPECTED, TOT_CALC;
	FILE* infile = fopen(ELEMFILENAME, "r");

	elementi = elem_l_read(infile);
	fclose(infile);

	do {
		do {
			printf("Inserire DD e DP (0 per uscire): ");
			retv = scanf("%d %d", &DD, &DP);
			getchar();
		} while (retv != 2 || DD < 0 || DP < 0);

		if (DD != 0 && DP != 0) {
			prog = prog_init();

			TOT_CALC = prog_make(prog, DD, DP, elementi);

			printf("----------\nDD = %d DP = %d\nTotale calcolato: %f\n", DD, DP, TOT_CALC);
			prog_print(stdout, prog);
			printf("\n");

			prog_free(prog);
		}
	} while (DD != 0 && DP != 0);

	fclose(infile);

	return 0;
}