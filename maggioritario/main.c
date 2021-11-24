#include <stdio.h>
#include <stdlib.h>

int maggioritario(int* a, int N) {
	int med = N / 2, first, second, i, count_f = 0, count_s = 0;

	if (N == 1)
		return a[0];

	first = maggioritario(a, med);
	second = maggioritario(a + med, N - med);


	if (first != second) {
		for (i = 0; i < N; i++) {
			if (a[i] == first)
				count_f++;
			else if (a[i] == second)
				count_s++;
		}
		if (count_s > med)
			return second;
		if (count_f > med)
			return first;

		return -1;
	}

	return first;
}


int main() {
	int N, *vet, r, i, m;

	do
	{
		printf("Inserire il numero di elementi: ");
		r = scanf("%d", &N);
		getchar();
	} while (r != 1 || N < 1);

	vet = (int*)malloc(N * sizeof(int));

	if (vet != NULL) {
		for (i = 0; i < N; i++) {
			do {
				printf("Elemento %d°: ", i + 1);
				r = scanf("%d", vet + i);
				getchar();
			} while (r != 1 || vet[i] < 0);
		}

		m = maggioritario(vet, N);
		if (m != -1)
			printf("\nElemento meggioritario: %d\n", m);
		else
			printf("\nElemento maggioritario non presente\n");
	}

	return 0;
}