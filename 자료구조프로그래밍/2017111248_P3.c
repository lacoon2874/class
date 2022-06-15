/*
	2017111248 황성재
	Program #3
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct {
	int key;
}element;

element *heap;

void push(element item, int *n);
element pop(int *n);
void printHeap(int n, FILE *fp);

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	int n, i, count = 0, print1, print2;
	element item;

	if (argc < 3) {                                            // output file name 까지 들어오지 않았을 경우 exit
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                            // 입력 파일이 열리지 않을 경우 exit
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                             // 출력 파일이 열리지 않을 경우 exit
		fprintf(stderr, "fail to output file open\n");
	}
	
	fscanf(ifp, "%d", &n);                                        // key 갯수 입력 받은 후 배열 생성
	MALLOC(heap, sizeof(*heap)*(n + 1));

	print1 = n / 3;                               // 1/3지점
	print2 = n * 2 / 3;                           // 2/3지점

	for (i = 1; i <= n; i++) {
		fscanf(ifp, "%d", &item.key);               // 키 값 입력

		push(item, &count);                         // min heap에 push

		if (i == print1)                        // 1/3 지점 출력
			printHeap(print1, ofp);
		else if (i == print2)                   // 2/3 지점 출력
			printHeap(print2, ofp);
	}
	printHeap(count, ofp);                       // 다 입력 받은 후 출력

	for (i = 0; i < 3; i++) {
		pop(&count);                        // 루트 노드 삭제
		printHeap(count, ofp);              // 삭제 후 출력
	}

	free(heap);
	fclose(ifp);
	fclose(ofp);

	return 0;
}

void push(element item, int *n) {
	int i;

	i = ++(*n);                                                 // 노드 수 증가
	while ((i != 1) && (item.key < heap[i / 2].key)) {               // 루트까지 오거나 부모 노드보다 작은 동안
		heap[i] = heap[i / 2];                           // 부모노드를 현재 자신의 위치로 이동
		i /= 2;                                      // 자신의 위치를 부모노드의 위치로 이동
	}
	heap[i] = item;                       // 루트 또는 부모노드 보다 클 경우 그 자리로 결정
}

element pop(int *n) {
	int parent, child;
	element item, temp;

	if (*n == 0) {                                    // heap 이 비었을 경우 exit
		fprintf(stderr, "heap is empty\n");
		exit(EXIT_FAILURE);
	}

	item = heap[1];                    // 삭제할 노드(루트 노드)
	temp = heap[(*n)--];               // 가장 마지막 노드
	parent = 1;
	child = 2;
	
	while (child <= *n) {                                                // child가 있는 동안 검사
		if ((child < *n) && (heap[child].key > heap[child + 1].key))     // child가 둘이면서 오른쪽 child가 더 작을 경우
			child++;                                                     // 오른쪽 child로 이동
		if (heap[child].key >= temp.key) break;                          // child 보다 작을 경우 그 자리로 결정
		heap[parent] = heap[child];                                      // child 보다 클 경우 child를 현재 자신의 자리로 이동
		parent = child;                                                  // 자신의 자리를 child 자리로 이동
		child *= 2;                                                      // 다음 검사할 것을 찾기 위해 child 2배
	}
	heap[parent] = temp;                // 결정된 자리에 넣음
	return item;                        // 삭제된 노드 반환
}

void printHeap(int n, FILE *fp) {
	int i, count = 2;

	for (i = 1; i <= n; i++) {
		if (i == count) {                    // 새로운 level로 갔을 경우 줄바꿈
			fprintf(fp, "\n");
			count *= 2;                      // 레벨 당 노드 수가 2배이므로 2를 곱해줌
		}
		fprintf(fp, "%d ", heap[i].key);     // 출력
	}
	fprintf(fp, "\n");                       // heap 출력 후 다른 heap과의 구분을 위해 줄바꿈
	fprintf(fp, "\n");
}