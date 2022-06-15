#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int *col;
int n, isEnd;

void nQueen(int index);
int promising(int i);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100];
	int i;

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));
	fopen_s(&ifp, inFileName, "r");
	fopen_s(&ofp, "output.txt", "w");

	fscanf_s(ifp, "%d", &n);

	col = (int*)malloc(sizeof(*col)*n);

	isEnd = 0;
	nQueen(0);

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
	int i;

	for (i = 0; i < n && !isEnd; i++) {
		col[index] = i;
		if (promising(index)) {
			if (index == n - 1) {
				isEnd = 1;
				break;
			}
			nQueen(index + 1);
		}
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