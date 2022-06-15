#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int *col;
int n, isEnd;

void nQueen(int index);
int promising(int i);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100];
	int i;

	srand((long)time(NULL));

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));
	fopen_s(&ifp, inFileName, "r");
	fopen_s(&ofp, "output.txt", "w");

	fscanf_s(ifp, "%d", &n);

	col = (int*)malloc(sizeof(*col)*n);

	isEnd = 0;
	while (!isEnd) {
		nQueen(0);
	}

	for (i = 0; i < n - 1; i++) {
		fprintf_s(ofp, "%d\n", col[i] + 1);
	}
	fprintf_s(ofp, "%d", col[i] + 1);

	free(col);
	fclose(ifp);
	fclose(ofp);

	return 0;
}

void nQueen(int index) {
	int *valid_child;
	int i, count;

	if (index < n) {
		valid_child = (int*)malloc(sizeof(*valid_child)*n);
		count = 0;

		for (i = 0; i < n; i++) {
			col[index] = i;
			if (promising(index)) {
				valid_child[count++] = i;
			}
		}

		if (count == 0) {
			isEnd = 0;
			return;
		}

		col[index] = valid_child[rand() % count];
		
		nQueen(index + 1);
	}
	else {
		isEnd = 1;
	}
}

int promising(int i) {
	int k, answer = 1;

	for (k = 0; k < i && answer; k++) {
		if (col[i] == col[k] || abs(col[i] - col[k]) == (i - k)) {
			answer = 0;
		}
	}

	return answer;
}