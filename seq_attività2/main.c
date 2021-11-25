#include <stdio.h>
#include <stdlib.h>

#define FILENAME "att.txt"

struct activity {
	int start, end;
};

typedef struct {
	struct activity* activities;
	int N;
} activity_l;

activity_l leggiFile(const FILE* infile) {
	int i;
	activity_l act;

	fscanf(infile, "%d", &act.N);
	act.activities = (struct activity*)malloc(act.N * sizeof(struct activity));
	
	for (i = 0; i < act.N; i++)
		fscanf(infile, " %d %d", &(act.activities[i].start), &(act.activities[i].end));

	return act;
}

void sort_activities(activity_l acts) {
	int i, j, continua = 1, current_swap = -1, last_swap = acts.N - 1;
	struct activity tmp;
	for (i = 0; i < acts.N - 1 && continua; i++) {
		continua = 0;
		for (j = 0; j < last_swap; j++) {
			if (acts.activities[j].end > acts.activities[j+1].end) {
				continua = 1;
				tmp = acts.activities[j];
				acts.activities[j] = acts.activities[j + 1];
				acts.activities[j + 1] = tmp;
				current_swap = j;
			}
		}
		last_swap = current_swap;
	}
}

void attPrint(activity_l acts, int* previous, int index) {
	if (previous[index] != -1)
		attPrint(acts, previous, previous[index]);
	printf("(%d, %d) ", acts.activities[index].start, acts.activities[index].end);
}


void attSel(activity_l activities) {
	int i, j, last = 0, current_duration, * lenghts = (int*)malloc(activities.N * sizeof(int)), * previous = (int*)malloc(activities.N * sizeof(int));
	lenghts[0] = activities.activities[0].end - activities.activities[0].start;
	previous[0] = -1;

	for (i = 1; i < activities.N; i++) {
		current_duration = activities.activities[i].end - activities.activities[i].start;
		lenghts[i] = current_duration;
		previous[i] = -1;
		for (j = 0; j < i && activities.activities[j].end <= activities.activities[i].start; j++) {
			if (lenghts[i] < current_duration + lenghts[j]) {
				lenghts[i] = current_duration + lenghts[j];
				previous[i] = j;
			}
		}
		if (lenghts[last] < lenghts[i])
			last = i;
	}
	printf("Una delle sequenze di attivita' con durata massima di %d e':\n", lenghts[last]);
	attPrint(activities, previous, last);
}


int main() {
	FILE* infile = fopen(FILENAME, "r");
	activity_l activities = leggiFile(infile);
	fclose(infile);

	sort_activities(activities);

	attSel(activities);

	return 0;
}