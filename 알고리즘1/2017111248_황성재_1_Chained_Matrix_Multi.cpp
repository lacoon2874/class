#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int row;
	int col;
}matrix_index;

typedef struct {
	int num;
	int index;
}mult_matrix;

void printAnswer(FILE *fp, mult_matrix **answer, int l, int r);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100] = { 0 };
	int *matrix;
	int i, n, row, col, min, min_index, calcul;
	matrix_index left, down;
	mult_matrix **result;

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));

	fopen_s(&ifp, inFileName, "r");
	if (!ifp) {
		fprintf_s(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}
	fopen_s(&ofp, "output.txt", "w");
	if (!ofp) {
		fprintf_s(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf_s(ifp, "%d", &n);
	matrix = (int*)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) {
		fscanf_s(ifp, "%d", &matrix[i]);
	}

	result = (mult_matrix**)malloc(sizeof(*result)*n);
	for (i = 1; i < n; i++) {
		result[i] = (mult_matrix*)malloc(sizeof(*result[i])*n);
		result[i][i].num = 0;
	}

	for (i = 2; i < n; i++) {
		row = 1;

		for (col = i; col < n; col++) {
			left.row = row;
			left.col = row;
			down.row = row + 1;
			down.col = col;
			min = INT_MAX;

			while (left.col < col) {
				calcul = result[left.row][left.col].num + result[down.row][down.col].num + matrix[left.row - 1] * matrix[left.col] * matrix[down.col];
				if (min > calcul) {
					min = calcul;
					min_index = left.col;
				}
				left.col++;
				down.row++;
			}

			result[row][col].num = min;
			result[row][col].index = min_index;

			row++;
		}
	}

	printAnswer(ofp, result, 1, n - 1);

	return 0;
}

void printAnswer(FILE *fp, mult_matrix **answer, int l, int r) {
	if (l == r) {
		fprintf_s(fp, "%d ", l);
	}
	else {
		fprintf_s(fp, "( ");
		printAnswer(fp, answer, l, answer[l][r].index);
		printAnswer(fp, answer, answer[l][r].index + 1, r);
		fprintf_s(fp, ") ");
	}
}