#include <stdio.h>
#include <stdlib.h>

int *d, *ws, ***ee, ***pp;
int n;

void findAnswer(int l, int r);
void printAnswer(FILE *fp, int l, int r, int trail_dir);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100] = { 0 };
	int i, j, k, m;
	int *w;

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
	fscanf_s(ifp, "%d", &m);
	d = (int*)malloc(sizeof(*d)*(n + 1));
	w = (int*)malloc(sizeof(*w)*(n + 1));
	ws = (int*)malloc(sizeof(*ws)*(n + 1));

	for (i = 1; i <= n; i++) {
		fscanf_s(ifp, "%d", &d[i]);
		fscanf_s(ifp, "%d", &w[i]);
	}
	ws[1] = w[1];
	for (i = 2; i <= n; i++) {
		ws[i] = ws[i - 1] + w[i];
	}

	ee = (int***)malloc(sizeof(*ee)*(n + 1));
	pp = (int***)malloc(sizeof(*pp)*(n + 1));
	for (i = 1; i <= n; i++) {
		ee[i] = (int**)malloc(sizeof(*ee[i])*(n + 1));
		pp[i] = (int**)malloc(sizeof(*pp[i])*(n + 1));
		for (j = 1; j <= n; j++) {
			ee[i][j] = (int*)malloc(sizeof(int) * 2);
			pp[i][j] = (int*)malloc(sizeof(int) * 2);
			for (k = 0; k < 2; k++) {
				ee[i][j][k] = INT_MAX;
				pp[i][j][k] = 0;
			}
		}
	}
	ee[m][m][0] = 0;
	pp[m][m][1] = 0;

	findAnswer(m, m);

	if (ee[1][n][0] < ee[1][n][1]) {
		fprintf_s(ofp, "%d\n", ee[1][n][0]);
		printAnswer(ofp, 1, n, 0);
	}
	else {
		fprintf_s(ofp, "%d\n", ee[1][n][1]);
		printAnswer(ofp, 1, n, 1);
	}

	return 0;
}

void findAnswer(int l, int r) {
	int min, min_index, calcul;
	if (l > 1) {
		if (!pp[l - 1][r][0]) {
			min = INT_MAX;
			min_index = 0;
			if (ee[l][r][0] != INT_MAX) {
				calcul = ee[l][r][0] + (d[l] - d[l - 1])*(ws[n] - (ws[r] - ws[l - 1]));
				min = calcul;
				min_index = 1;
			}
			if (ee[l][r][1] != INT_MAX) {
				calcul = ee[l][r][1] + (d[r] - d[l - 1])*(ws[n] - (ws[r] - ws[l - 1]));
				if (min > calcul) {
					min = calcul;
					min_index = 2;
				}
			}
			if (min_index) {
				ee[l - 1][r][0] = min;
				if (min_index == 1) {
					pp[l - 1][r][0] = l;
				}
				else {
					pp[l - 1][r][0] = r;
				}
				findAnswer(l - 1, r);
			}
		}
	}
	if (r < n) {
		if (!pp[l][r + 1][1]) {
			min = INT_MAX;
			min_index = 0;
			if (ee[l][r][0] != INT_MAX) {
				calcul = ee[l][r][0] + (d[r + 1] - d[l])*(ws[n] - (ws[r] - ws[l - 1]));
				min = calcul;
				min_index = 1;
			}
			if (ee[l][r][1] != INT_MAX) {
				calcul = ee[l][r][1] + (d[r + 1] - d[r])*(ws[n] - (ws[r] - ws[l - 1]));
				if (min > calcul) {
					min = calcul;
					min_index = 2;
				}
			}
			if (min_index) {
				ee[l][r + 1][1] = min;
				if (min_index == 1) {
					pp[l][r + 1][1] = l;
				}
				else {
					pp[l][r + 1][1] = r;
				}
				findAnswer(l, r + 1);
			}
		}
	}
}

void printAnswer(FILE *fp, int l, int r, int trail_dir) {
	if (pp[l][r][trail_dir]) {
		if (!trail_dir) {
			if (pp[l][r][trail_dir] == l + 1) {
				printAnswer(fp, l + 1, r, 0);
			}
			else {
				printAnswer(fp, l + 1, r, 1);
			}
		}
		else {
			if (pp[l][r][trail_dir] == l) {
				printAnswer(fp, l, r - 1, 0);
			}
			else {
				printAnswer(fp, l, r - 1, 1);
			}
		}
	}
	if (!trail_dir) {
		fprintf_s(fp, "%d\n", l);
	}
	else {
		fprintf_s(fp, "%d\n", r);
	}
}