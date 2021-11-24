#include <stdio.h>

#define MAXBIT 8
#define MAXCOD 256

int pow2(int e) {
	return 1 << e;
}

void gray_1(int codici[MAXCOD][MAXBIT], int pos, int nbit) {
	int i, j, ncodici = pow2(nbit - pos);

	if (pos == nbit - 1) {
		codici[0][pos] = 0;
		codici[1][pos] = 1;
	}
	else {
		gray_1(codici, pos + 1, nbit);

		for (i = 0; i < ncodici / 2; i++) {
			codici[i][pos] = 0;
			codici[ncodici / 2 + i][pos] = 1;

			for (j = pos + 1; j < nbit; j++)
				codici[ncodici / 2 + i][j] = codici[ncodici / 2 - 1 - i][j];
		}
	}
}

void gray(int codici[MAXCOD][MAXBIT], int nbit) {
	gray_1(codici, 0, nbit);
}

void printmat(int codici[MAXCOD][MAXBIT], int r, int c) {
	int i, j;
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			printf("%d", codici[i][j]);
		}
		printf("\n");
	}
}

int main() {
	int codici[MAXCOD][MAXBIT], bit, ret_val;
	do
	{
		printf("num bit: ");
		ret_val = scanf("%d", &bit);
		getchar();
	} while (ret_val != 1 || bit > 8 || bit < 1);

	gray(codici, bit);

	printmat(codici, pow2(bit), bit);

	return 0;
}