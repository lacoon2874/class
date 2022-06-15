#include <stdio.h>
#include <stdlib.h>

#define infinity 9000

typedef struct pt *pathPointer;
typedef struct pt {
	int num;
	pathPointer next;
}p;

typedef struct {
	int bound;
	int level;
	pathPointer path;
}vertex;

typedef struct leftist *leftistTree;
typedef struct leftist {
	int shortest;
	vertex data;
	leftistTree leftChild, rightChild;
}left;

int **matrix;
int *row, *col;
int n;

void tsp(int *opttour, int *minlength);
int getBound(vertex ptr);
void makeNewPath(vertex *ptr, int vertexNum);
void copyPath(vertex *a, vertex *b);
void insertPQ(leftistTree *PQ, vertex key);
void removePQ(leftistTree *PQ, vertex *key);
leftistTree makeTree(vertex key);
leftistTree meld(leftistTree a, leftistTree b);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100];
	int *opttour;
	int i, j, minlength;

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));
	fopen_s(&ifp, inFileName, "r");
	fopen_s(&ofp, "output.txt", "w");

	fscanf_s(ifp, "%d", &n);
	opttour = (int*)malloc(sizeof(*opttour)*(n + 1));
	matrix = (int**)malloc(sizeof(*matrix)*(n + 1));
	row = (int*)malloc(sizeof(*row)*(n + 1));
	col = (int*)malloc(sizeof(*col)*(n + 1));
	for (i = 1; i <= n; i++) {
		matrix[i] = (int*)malloc(sizeof(*matrix[i])*(n + 1));
		for (j = 1; j <= n; j++) {
			fscanf_s(ifp, "%d", &matrix[i][j]);
		}
		matrix[i][i] = infinity;
	}

	tsp(opttour, &minlength);

	for (i = 0; i < n; i++) {
		fprintf_s(ofp, "%d\n", opttour[i]);
	}
	fprintf_s(ofp, "%d", opttour[i]);

	return 0;
}

void tsp(int *opttour, int *minlength) {
	leftistTree PQ;
	pathPointer temp, trail;
	vertex v, u;
	int *col;
	int i, length;

	v.path = NULL;
	u.path = NULL;
	col = (int*)malloc(sizeof(*col)*(n + 1));

	PQ = NULL;
	v.level = 0;
	makeNewPath(&v, 1);
	v.bound = getBound(v);
	*minlength = infinity;

	insertPQ(&PQ, v);
	while (PQ) {
		removePQ(&PQ, &v);

		for (i = 1; i <= n; i++)
			col[i] = 1;

		if (v.bound < *minlength) {
			u.level = v.level + 1;
			if (u.level == n - 1) {
				u.path = NULL;
				copyPath(&u, &v);
				for (temp = u.path->next; temp; temp = temp->next) {
					col[temp->num] = 0;
				}
				for (i = 2; i <= n; i++) {
					if (col[i])
						makeNewPath(&u, i);
				}
				makeNewPath(&u, 1);

				length = 0;
				trail = u.path;
				for (temp = u.path->next; temp; temp = temp->next) {
					length += matrix[trail->num][temp->num];
					trail = temp;
				}

				if (length < *minlength) {
					*minlength = length;
					temp = u.path;
					for (i = 0; i <= n; i++) {
						opttour[i] = temp->num;
						temp = temp->next;
					}
				}
			}
			else {
				for (i = 2; i <= n; i++) {
					col[i] = 1;
				}
				for (temp = v.path->next; temp; temp = temp->next) {
					col[temp->num] = 0;
				}
				for (i = 2; i <= n; i++) {
					if (col[i]) {
						u.path = NULL;
						copyPath(&u, &v);
						makeNewPath(&u, i);
						u.bound = getBound(u);
						if (u.bound < *minlength)
							insertPQ(&PQ, u);
					}
				}
			}
		}
	}
}

int getBound(vertex ptr) {
	pathPointer temp, trail, t_t = NULL;
	int i, j, min, sum = 0, back;

	if (ptr.path->next) {
		trail = ptr.path;
		for (i = 1; i <= n; i++) {
			row[i] = 1;
			col[i] = 1;
		}
		for (temp = ptr.path->next; temp; temp = temp->next) {
			sum += matrix[trail->num][temp->num];
			row[temp->num] = 0;
			col[temp->num] = 0;
			t_t = trail;
			trail = temp;
		}
		if(t_t)
			back = t_t->num;
		for (i = 2; i <= n; i++) {
			min = 0;
			if (row[i]) {
				min = infinity;
				for (j = 1; j <= n; j++) {
					if (col[j]) {
						if (matrix[i][j] < min)
							min = matrix[i][j];
					}
				}
			}
			else if (i == trail->num) {
				min = infinity;
				for (j = 1; j <= n; j++) {
					if (back != j)
						if (matrix[i][j] < min)
							min = matrix[i][j];
				}
			}
			sum += min;
		}
	}
	else {
		for (i = 1; i <= n; i++) {
			min = matrix[i][1];
			for (j = 2; j <= n; j++) {
				if (matrix[i][j] < min)
					min = matrix[i][j];
			}
			sum += min;
		}
	}

	return sum;
}

void makeNewPath(vertex *ptr, int vertexNum) {
	pathPointer temp, a, trail = NULL;

	temp = (pathPointer)malloc(sizeof(*temp));
	temp->num = vertexNum;
	temp->next = NULL;

	for (a = (*ptr).path; a; a = a->next) {
		trail = a;
	}
	if (trail) {
		trail->next = temp;
	}
	else
		(*ptr).path = temp;
}

void copyPath(vertex *a, vertex *b) {
	pathPointer temp, trail;

	for (temp = (*a).path; temp; ) {
		trail = temp;
		temp = temp->next;
		free(trail);
	}
	(*a).path = NULL;

	for (temp = (*b).path; temp; temp = temp->next) {
		makeNewPath(a, temp->num);
	}
}

void insertPQ(leftistTree *PQ, vertex key) {
	leftistTree temp;

	temp = makeTree(key);

	if (!(*PQ)) {
		*PQ = temp;
	}
	else {
		*PQ = meld(*PQ, temp);
	}
}

void removePQ(leftistTree *PQ, vertex *key) {
	leftistTree temp;

	*key = (*PQ)->data;
	temp = (*PQ);
	*PQ = meld((*PQ)->leftChild, (*PQ)->rightChild);
	if (!((*PQ)->shortest)) {
		free(*PQ);
		*PQ = NULL;
	}
	free(temp);
}

leftistTree makeTree(vertex key) {
	leftistTree temp[3];
	int i;

	for (i = 0; i < 3; i++) {
		temp[i] = (leftistTree)malloc(sizeof(*temp[i]));
	}

	for (i = 1; i < 3; i++) {
		temp[i]->shortest = 0;
		temp[i]->leftChild = NULL;
		temp[i]->rightChild = NULL;
	}

	temp[0]->data = key;
	temp[0]->shortest = 1;
	temp[0]->leftChild = temp[1];
	temp[0]->rightChild = temp[2];

	return temp[0];
}

leftistTree meld(leftistTree a, leftistTree b) {
	leftistTree temp;

	if (!(a->shortest)) {
		free(a);
		return b;
	}
	else if (!(b->shortest)) {
		free(b);
		return a;
	}

	if (a->data.bound < b->data.bound) {
		a->rightChild = meld(a->rightChild, b);

		if (a->rightChild->shortest > a->leftChild->shortest) {
			temp = a->leftChild;
			a->leftChild = a->rightChild;
			a->rightChild = temp;
		}

		a->shortest = a->rightChild->shortest + 1;

		return a;
	}

	// else
	b->rightChild = meld(a, b->rightChild);
	if (b->rightChild->shortest > b->leftChild->shortest) {
		temp = b->leftChild;
		b->leftChild = b->rightChild;
		b->rightChild = temp;
	}

	b->shortest = b->rightChild->shortest + 1;

	return b;
}