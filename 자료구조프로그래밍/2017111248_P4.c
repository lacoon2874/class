/*
	2017111248 황성재
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

typedef struct Node *nodePointer;                     // adjacency list 구조체
typedef struct Node{
	int vertex;
	nodePointer edge;
};

typedef struct list *listPointer;                         // path 저장을 위한 구조체
typedef struct list {
	int vertex[26];
	int count;
	listPointer next;
};

nodePointer graph[26];
listPointer path;                // path
int stack[26], visited[26];
int top = -1, count = 0;

void insert(int a, int b);                    // graph 생성
void findPath(int a, int b);                  // path 찾는 함수
void insertPath();                            // path 찾았을 경우 추가하는 함수
void push(int key);                           // stack push
int pop();                                    // stack pop

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;                // 입력, 출력 파일 포인터
	listPointer ptr;
	int num_v, num_e, start, finish, i;
	char ch1, ch2, buf;

	if (argc < 3) {                                 // 입출력 파일 이름이 인자로 들어오지 않을 경우 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                    // 입력 파일이 열리지 않을 경우 종료
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);\
	}

	if (!(ofp = fopen(argv[2], "w"))) {                    // 출력 파일이 열리지 않을 경우 종료
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d %d %c %c", &num_v, &num_e, &ch1, &ch2);    // vertex개수, edge개수, 출발vertex, 도착 vertex
	start = ch1 - 'A';            // 숫자로 연산하기 위해서 아스키코드 이용
	finish = ch2 - 'A';           // 숫자로 연산하기 위해서 아스키코드 이용
	fscanf(ifp, "%c", &buf);      // 줄바꿈 문자 버퍼제거

	MALLOC(path, sizeof(*path));     // 찾은 path를 저장할 공간 동적할당 및 초기화
	path->next = NULL;

	for (i = 0; i < 26; i++) {
		MALLOC(graph[i], sizeof(*graph[i]));             // graph 저장할 공간 동적할당 및 초기화
		graph[i]->edge = NULL;
		visited[i] = 0;                                  // visited 초기화
	}

	for (i = 0; i < num_e; i++) {                      // directed edge 입력 및 버퍼제거
		fscanf(ifp, "%c %c%c", &ch1, &ch2, &buf);
		insert(ch1 - 'A', ch2 - 'A');                  // 숫자로 연산하기 위해서 아스키코드 이용
	}

	findPath(start, finish);                           // path 찾는 함수 호출

	fprintf(ofp, "%d\n", count);                  // 첫 줄에 path 개수 출력

	for (ptr = path->next; ptr; ptr = ptr->next) {               // path 출력
		for (i = 0; i < ptr->count; i++) {
			fprintf(ofp, "%c ", ptr->vertex[i] + 'A');
		}
		fprintf(ofp, "\n");
	}

	return 0;
}

void insert(int a, int b) {
	nodePointer temp, ptr = graph[a];

	MALLOC(temp, sizeof(*temp));              // 새로 넣을 vertex 공간 할당

	temp->vertex = b;                         // vertex 초기화
	temp->edge = NULL;

	while (ptr->edge) {                       // vertex의 마지막위치 찾아서 연결
		ptr = ptr->edge;
	}

	ptr->edge = temp;
}

void findPath(int a, int b) {
	nodePointer ptr;

	push(a);                     // stack에 현재 vertex push

	if (a == b) {
		insertPath();               // 도착 vertex에 도달했을 경우 path추가
		count++;                    // path 수 증가
		pop();                      // stack에서 현재 vertex pop
		return;                     // 함수 종료
	}

	visited[a] = 1;                  // 현재 vertex 방문
	for (ptr = graph[a]->edge; ptr; ptr = ptr->edge) {          // 연결된 모든 vertex 검사
		findPath(ptr->vertex, b);
	}
	visited[a] = 0;                  // 현재 vertex에서 돌아감을 표시
	pop();                           // stack에서 현재 vertex pop
}

void insertPath() {
	listPointer temp, ptr = path;
	int i;

	MALLOC(temp, sizeof(*temp));               // path list에 넣을 공간 할당 및 초기화

	temp->next = NULL;

	while (ptr->next) {                   // path list의 마지막 위치 찾음
		ptr = ptr->next;
	}

	for (i = 0; i <= top; i++) {                   // path 입력
		temp->vertex[i] = stack[i];
	}
	temp->count = top + 1;                         // path 길이 입력

	ptr->next = temp;                           // path list의 마지막에 새로운 path 연결
}

void push(int key) {               // stack push
	stack[++top] = key;
}

int pop() {                        // stack pop
	return stack[top--];
}