#include <stdio.h>
#include "programma.h"
#include "elemento.h"

#define ELEMFILENAME "elementi.txt"
#define TESTSETFILENAME "testset_e_risultati.txt"

int main() {
	programma_t prog;
	elementi_l elementi;
	int DD, DP, np, i;
	float TOT_EXPECTED, TOT_CALC;
	FILE* infile = fopen(ELEMFILENAME, "r");

	elementi = elem_l_read(infile);
	fclose(infile);

	infile = fopen(TESTSETFILENAME, "r");

	while (!feof(infile) && fscanf(infile, "--- Test Case #%d ---\nDD = %d DP = %d\nTOT = %f", &np, &DD, &DP, &TOT_EXPECTED) == 4) {
		prog = prog_init();

		TOT_CALC = prog_make(prog, DD, DP, elementi);

		printf("Programma #%d\nTotale calcolato: %f\nTotale previsto: %f\n", np, TOT_CALC, TOT_EXPECTED);

		prog_print(stdout, prog);
		printf("\n");

		prog_free(prog);
	}

	fclose(infile);

	return 0;
}