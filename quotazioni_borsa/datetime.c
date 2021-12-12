#include "datetime.h"
#include <stdlib.h>

data_t date_read(const FILE* fp) {
	data_t data;
	if (fscanf(fp, "%d/%d/%d %d:%d", &data.year, &data.month, &data.day, &data.time.hour, &data.time.min) != 5)
		exit(-1);
	return data;
}

int datecmp(data_t a, data_t b) {
	if (a.year > b.year)
		return 3;
	else if (a.year < b.year)
		return -3;
	else if (a.month > b.month)
		return 2;
	else if (a.month < b.month)
		return -2;
	else if (a.day > b.day)
		return 1;
	else if (a.day < b.day)
		return -1;
	else
		return 0;
}


int oracmp(ora_t a, ora_t b) {
	if (a.hour > b.hour)
		return 2;
	else if (a.hour < b.hour)
		return -2;
	else if (a.min > b.min)
		return 1;
	else if (a.min < b.min)
		return -1;
	else
		return 0;
}

int dateoracmp(data_t a, data_t b) {
	int dcmp = datecmp(a, b);
	if (dcmp != 0)
		return oracmp(a.time, b.time);
	else
		return dcmp;
}
