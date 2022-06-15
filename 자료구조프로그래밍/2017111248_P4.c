/*
	2017111248 Ȳ����
	Program #4
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct Node *nodePointer;                     // adjacency list ����ü
typedef struct Node{
	int vertex;
	nodePointer edge;
};

typedef struct list *listPointer;                         // path ������ ���� ����ü
typedef struct list {
	int vertex[26];
	int count;
	listPointer next;
};

nodePointer graph[26];
listPointer path;                // path
int stack[26], visited[26];
int top = -1, count = 0;

void insert(int a, int b);                    // graph ����
void findPath(int a, int b);                  // path ã�� �Լ�
void insertPath();                            // path ã���� ��� �߰��ϴ� �Լ�
void push(int key);                           // stack push
int pop();                                    // stack pop

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;                // �Է�, ��� ���� ������
	listPointer ptr;
	int num_v, num_e, start, finish, i;
	char ch1, ch2, buf;

	if (argc < 3) {                                 // ����� ���� �̸��� ���ڷ� ������ ���� ��� ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                    // �Է� ������ ������ ���� ��� ����
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);\
	}

	if (!(ofp = fopen(argv[2], "w"))) {                    // ��� ������ ������ ���� ��� ����
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d %d %c %c", &num_v, &num_e, &ch1, &ch2);    // vertex����, edge����, ���vertex, ���� vertex
	start = ch1 - 'A';            // ���ڷ� �����ϱ� ���ؼ� �ƽ�Ű�ڵ� �̿�
	finish = ch2 - 'A';           // ���ڷ� �����ϱ� ���ؼ� �ƽ�Ű�ڵ� �̿�
	fscanf(ifp, "%c", &buf);      // �ٹٲ� ���� ��������

	MALLOC(path, sizeof(*path));     // ã�� path�� ������ ���� �����Ҵ� �� �ʱ�ȭ
	path->next = NULL;

	for (i = 0; i < 26; i++) {
		MALLOC(graph[i], sizeof(*graph[i]));             // graph ������ ���� �����Ҵ� �� �ʱ�ȭ
		graph[i]->edge = NULL;
		visited[i] = 0;                                  // visited �ʱ�ȭ
	}

	for (i = 0; i < num_e; i++) {                      // directed edge �Է� �� ��������
		fscanf(ifp, "%c %c%c", &ch1, &ch2, &buf);
		insert(ch1 - 'A', ch2 - 'A');                  // ���ڷ� �����ϱ� ���ؼ� �ƽ�Ű�ڵ� �̿�
	}

	findPath(start, finish);                           // path ã�� �Լ� ȣ��

	fprintf(ofp, "%d\n", count);                  // ù �ٿ� path ���� ���

	for (ptr = path->next; ptr; ptr = ptr->next) {               // path ���
		for (i = 0; i < ptr->count; i++) {
			fprintf(ofp, "%c ", ptr->vertex[i] + 'A');
		}
		fprintf(ofp, "\n");
	}

	return 0;
}

void insert(int a, int b) {
	nodePointer temp, ptr = graph[a];

	MALLOC(temp, sizeof(*temp));              // ���� ���� vertex ���� �Ҵ�

	temp->vertex = b;                         // vertex �ʱ�ȭ
	temp->edge = NULL;

	while (ptr->edge) {                       // vertex�� ��������ġ ã�Ƽ� ����
		ptr = ptr->edge;
	}

	ptr->edge = temp;
}

void findPath(int a, int b) {
	nodePointer ptr;

	push(a);                     // stack�� ���� vertex push

	if (a == b) {
		insertPath();               // ���� vertex�� �������� ��� path�߰�
		count++;                    // path �� ����
		pop();                      // stack���� ���� vertex pop
		return;                     // �Լ� ����
	}

	visited[a] = 1;                  // ���� vertex �湮
	for (ptr = graph[a]->edge; ptr; ptr = ptr->edge) {          // ����� ��� vertex �˻�
		findPath(ptr->vertex, b);
	}
	visited[a] = 0;                  // ���� vertex���� ���ư��� ǥ��
	pop();                           // stack���� ���� vertex pop
}

void insertPath() {
	listPointer temp, ptr = path;
	int i;

	MALLOC(temp, sizeof(*temp));               // path list�� ���� ���� �Ҵ� �� �ʱ�ȭ

	temp->next = NULL;

	while (ptr->next) {                   // path list�� ������ ��ġ ã��
		ptr = ptr->next;
	}

	for (i = 0; i <= top; i++) {                   // path �Է�
		temp->vertex[i] = stack[i];
	}
	temp->count = top + 1;                         // path ���� �Է�

	ptr->next = temp;                           // path list�� �������� ���ο� path ����
}

void push(int key) {               // stack push
	stack[++top] = key;
}

int pop() {                        // stack pop
	return stack[top--];
}