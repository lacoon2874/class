#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int x;
	int y;
}matrixIndex;

int **maze, **visited, **ee;
matrixIndex **pp;
int n;

void findMin(matrixIndex a, int weight);
void printAnswer(FILE *fp, matrixIndex a);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100] = { 0 };
	int i, j;
	matrixIndex a;

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));

	fopen_s(&ifp, inFileName, "r");
	if (!ifp) {
		fprintf_s(stderr, "insufficient memory\n");
		exit(EXIT_FAILURE);
	}
	fopen_s(&ofp, "output.txt", "w");
	if (!ofp) {
		fprintf_s(stderr, "insufficient memory\n");
		exit(EXIT_FAILURE);
	}

	fscanf_s(ifp, "%d", &n);

	maze = (int**)malloc(sizeof(*maze)*n);
	visited = (int**)malloc(sizeof(*visited)*n);
	ee = (int**)malloc(sizeof(*ee)*n);
	pp = (matrixIndex**)malloc(sizeof(*pp)*n);
	for (i = 0; i < n; i++) {
		maze[i] = (int*)malloc(sizeof(*maze[i])*n);
		visited[i] = (int*)malloc(sizeof(*visited[i])*n);
		ee[i] = (int*)malloc(sizeof(*ee[i])*n);
		pp[i] = (matrixIndex*)malloc(sizeof(*pp[i])*n);
		for (j = 0; j < n; j++) {
			fscanf_s(ifp, "%1d", &maze[i][j]);
			visited[i][j] = 0;
			ee[i][j] = INT_MAX;
		}
	}

	a.x = 0;
	a.y = 0;
	findMin(a, 0);

	fprintf_s(ofp, "%d\n", ee[n - 1][n - 1]);

	a.x = n - 1;
	a.y = n - 1;
	printAnswer(ofp, a);

	return 0;
}

void findMin(matrixIndex a, int weight) {
	matrixIndex b;
	int new_weight;

	visited[a.y][a.x] = 1;
	
	if (a.x < n - 1) {
		if (!visited[a.y][a.x + 1]) {
			b.x = a.x + 1;
			b.y = a.y;
			if (!maze[b.y][b.x]) {
				new_weight = weight + 1;
			}
			else {
				new_weight = weight;
			}
			if (ee[b.y][b.x] > new_weight) {
				ee[b.y][b.x] = new_weight;
				pp[b.y][b.x].x = a.x;
				pp[b.y][b.x].y = a.y;
				findMin(b, new_weight);
			}
		}
	}
	if (a.y < n - 1) {
		if (!visited[a.y + 1][a.x]) {
			b.x = a.x;
			b.y = a.y + 1;
			if (!maze[b.y][b.x]) {
				new_weight = weight + 1;
			}
			else {
				new_weight = weight;
			}
			if (ee[b.y][b.x] > new_weight) {
				ee[b.y][b.x] = new_weight;
				pp[b.y][b.x].x = a.x;
				pp[b.y][b.x].y = a.y;
				findMin(b, new_weight);
			}
		}
	}
	if (a.x > 0) {
		if (!visited[a.y][a.x - 1]) {
			b.x = a.x - 1;
			b.y = a.y;
			if (!maze[b.y][b.x]) {
				new_weight = weight + 1;
			}
			else {
				new_weight = weight;
			}
			if (ee[b.y][b.x] > new_weight) {
				ee[b.y][b.x] = new_weight;
				pp[b.y][b.x].x = a.x;
				pp[b.y][b.x].y = a.y;
				findMin(b, new_weight);
			}
		}
	}
	if (a.y > 0) {
		if (!visited[a.y - 1][a.x]) {
			b.x = a.x;
			b.y = a.y - 1;
			if (!maze[b.y][b.x]) {
				new_weight = weight + 1;
			}
			else {
				new_weight = weight;
			}
			if (ee[b.y][b.x] > new_weight) {
				ee[b.y][b.x] = new_weight;
				pp[b.y][b.x].x = a.x;
				pp[b.y][b.x].y = a.y;
				findMin(b, new_weight);
			}
		}
	}
	
	visited[a.y][a.x] = 0;
}

void printAnswer(FILE *fp, matrixIndex a) {
	matrixIndex b;

	if (a.x != 0 || a.y != 0) {
		b.x = pp[a.y][a.x].x;
		b.y = pp[a.y][a.x].y;
		printAnswer(fp, b);
	}
	
	fprintf_s(fp, "%d %d\n", a.y + 1, a.x + 1);
}