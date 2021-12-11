#include <stdio.h>
#include <stdlib.h>

#define FILENAME "att2.txt"

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


void Merge(activity_l activities, activity_l aux, int left, int center, int right) {
	int i = left, j = center + 1, k = 0;

	while (i <= center && j <= right) {
		if (activities.activities[i].end <= activities.activities[j].end)
			aux.activities[k++] = activities.activities[i++];
		else
			aux.activities[k++] = activities.activities[j++];
	}

	while (i <= center)
		aux.activities[k++] = activities.activities[i++];

	while (j <= right)
		aux.activities[k++] = activities.activities[j++];


	for (k = left; k <= right; k++)
		activities.activities[k] = aux.activities[k - left];
}

void mergeSortR(activity_l activities, activity_l aux, int left, int right) {
	int med = (left + right) / 2;
	if (right > left) {
		mergeSortR(activities, aux, left, med);
		mergeSortR(activities, aux, med + 1, right);
		Merge(activities, aux, left, med, right);
	}
}

void mergesort_activities(activity_l activities) {
	activity_l aux;

	aux.N = activities.N;
	aux.activities = (struct activity*)malloc(aux.N * sizeof(struct activity));

	mergeSortR(activities, aux, 0, activities.N - 1);

	free(aux.activities);
}

int find_last_compat_act(activity_l activities, int N, int value) {
	int right, left = 0, medium;
	if (N > activities.N)
		N = activities.N;
	right = N - 1;

	while (left <= right) {
		medium = left + (right - left) / 2;
		if (activities.activities[medium].end <= value)
			left = medium + 1;
		else if (activities.activities[medium - 1].end <= value)
			return medium - 1;
		else
			right = medium - 1;
	}
	return -1;
}

void attPrint(activity_l acts, int* previous, int* lenghts, int index) {
	if (previous[index] != -1)
		attPrint(acts, previous, lenghts, previous[index]);
	if (lenghts[index] != lenghts[previous[index]])
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

		j = find_last_compat_act(activities, i, activities.activities[i].start);
		if (j != -1) {
			if (lenghts[i - 1] > lenghts[j] + current_duration) {
				previous[i] = i - 1;
				lenghts[i] = lenghts[i - 1];
			}
			else {
				previous[i] = j;
				lenghts[i] = lenghts[j] + current_duration;
			}
		}

		if (lenghts[last] < lenghts[i])
			last = i;
	}
	printf("Una delle sequenze di attivita' con durata massima di %d e':\n", lenghts[last]);
	attPrint(activities, previous, lenghts, last);
}


int main() {
	FILE* infile = fopen(FILENAME, "r");
	activity_l activities = leggiFile(infile);
	fclose(infile);

	mergesort_activities(activities);

	attSel(activities);

	return 0;
}