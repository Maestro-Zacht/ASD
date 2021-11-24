#include <stdio.h>
#include <ctype.h>

int isMatch(char c, const char* regex) {
	int i;
	switch (*regex)
	{
	case '.':
		return 1;
	case '[':
	{
		int not = 0, good = 0;
		i = 1;
		if (regex[1] == '^') {
			i++;
			not = 1;
		}

		while (regex[i] != ']')
		{
			if (not && regex[i] == c)
				return 0;
			else if (!not && regex[i] == c)
				good = 1;

			i++;
		}
		if (not || good)
			return i;
		else
			return 0;
	}
	case '\a':
		if (islower(c))
			return 1;
		else
			return 0;
	case '\A':
		if (isupper(c))
			return 1;
		else
			return 0;
	default:
		return c == *regex;
	}
}

int controllaMatch(const char* start, const char* regexp) {
	int i, j, res;
	for (i = j = 0; regexp[j] != '\0'; i++, j++) {
		res = isMatch(start[i], regexp + j);
		if (res == 0)
			return 0;
		else if (res > 1)
			j += res;
	}
	return 1;
}

char* cercaRegexp(const char* src, const char* regexp) {
	int i;
	for (i = 0; src[i] != '\0'; i++) 
		if (controllaMatch(src + i, regexp))
			return src + i;
	
	return NULL;
}

int main() {
	char* search = "sdbfverygfdbgHfmotokrbfiyerfbyG", * regex = "\A", *res;
	
	res = cercaRegexp(search, regex);

	if (res == NULL)
		printf("*Non trovato*\n");
	else
		puts(res);

	return 0;
}