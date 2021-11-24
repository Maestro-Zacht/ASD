#include <stdio.h>

int gcd(int a, int b) {
	int tmp;

	if (a == 0)
		return b;

	if (a < b) {
		tmp = a;
		a = b;
		b = tmp;
	}


	if (1 & a & b)
		return gcd((a - b) / 2, b);

	if (1 & a) // allora b è pari
		return gcd(a, b / 2);

	if (1 & b) // allora a è pari
		return gcd(a / 2, b);

	return 2 * gcd(a / 2, b / 2);
}

int main() {
	int a, b, r;
	

	do {
		printf("Inserire due numeri naturali positivi\n");
		r = scanf("%d %d", &a, &b);
	} while (r != 2 || a < 1 || b < 1);

	printf("MCD: %d\n", gcd(a, b));

	return 0;
}