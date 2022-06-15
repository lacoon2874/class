/*
	2017111248 황성재
	Program #6
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct node *nodePointer;                  // tree node 구조체
typedef struct node {
	int degree, data;
	nodePointer child, link;
};

typedef struct queue *queuePointer;                // level order queue 구조체
typedef struct queue {
	nodePointer data;
	queuePointer next;
};

nodePointer min = NULL;
queuePointer front = NULL, rear = NULL;

void insert(int key);
int deletion();
void meld();
nodePointer join(nodePointer a, nodePointer b);
void printAll(FILE *fp);
void addQ(nodePointer key);
nodePointer deleteQ();

int main(int argc, char *argv[]) {                         // main 함수
	FILE *ifp, *ofp;
	int size, i, n, key;

	if (argc < 3) {                                        // 인자가 적을 경우 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                    // input file이 열리지 않을 경우 종료
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                    // output file이 열리지 않을 경우 종료
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &size);                              // size 입력 후 1/2선 계산
	n = size / 2;

	for (i = 0; i < n; i++) {                              // 1/2선 까지 입력 및 insert
		fscanf(ifp, "%d", &key);

		insert(key);
	}
	fprintf(ofp, "1/2 입력 삭제 전\n");                    // 삭제 전 출력
	printAll(ofp);
	fprintf(ofp, "\n1/2 입력 삭제 후\n");                  // 삭제 및 삭제 후 출력
	deletion();
	printAll(ofp);

	for (; i < size; i++) {                                // 1/2선 이후 입력 및 insert
		fscanf(ifp, "%d", &key);

		insert(key);
	}
	fprintf(ofp, "\n전체 입력 삭제 전\n");                 // 삭제 전 출력
	printAll(ofp);
	fprintf(ofp, "\n전체 입력 삭제 후\n");                 // 삭제 및 삭제 후 출력
	deletion();
	printAll(ofp);

	return 0;
}

void insert(int key) {                                     // 삽입 함수
	nodePointer temp;

	MALLOC(temp, sizeof(*temp));

	temp->degree = 0;                                      // data에 값 넣고 초기화
	temp->child = NULL;
	temp->data = key;

	if (!min) {                                            // heap이 비어있을 경우 자기 자신 link
		temp->link = temp;
		min = temp;
	}
	else {                                                 // heap에 tree가 있을 경우
		temp->link = min->link;                            // min의 바로 뒤에 link 연결
		min->link = temp;

		if (temp->data < min->data)                        // 새로 들어온 값이 제일 작을 경우 min 변경
			min = temp;

		meld();                                            // 트리 합병
	}
}

int deletion() {                                           // 값 삭제 및 반환
	nodePointer a, b, ptr, a_trail, b_trail;
	int min_value, x;

	if (min) {                                             // heap에 값이 있을 경우
		x = min->data;                                     // 반환할 값 저장

		if (min->link != min) {                            // min이 포함된 트리 말고 다른 트리가 존재할 경우
			min_value = min->link->data;                   // 다른 트리에서 가장 작은 값(min) 찾음
			ptr = min->link;
			a_trail = min;

			for (a = min->link; a != min; a = a->link) {
				if (a->data < min_value) {
					min_value = a->data;
					ptr = a;
				}

				a_trail = a;
			}

			if (min->child) {                              // min의 child가 존재할 경우 chlid에서도 추가로 찾음
				b = min->child;
				do {
					if (b->data < min_value) {
						min_value = b->data;
						ptr = b;
					}

					b_trail = b;
					b = b->link;
				} while (b != min->child);

				a_trail->link = min->child;                // min의 child와 link를 heap으로 circular link로 연결
				b_trail->link = min->link;
			}
			else a_trail->link = min->link;                // min의 link 재조정

			free(min);

			min = ptr;                                     // 찾은 가장 작은 루트를 min으로 변경

			meld();                                        // 삭제 후 합병
		}
		else if (min->child) {                             // min에게 child만 있었을 경우
			min_value = min->child->data;                  // 가장 작은 값 찾음
			ptr = min->child;
			a = min->child;

			do {
				if (a->data < min_value) {
					min_value = a->data;
					ptr = a;
				}

				a = a->link;
			} while (a != min->child);

			free(min);                                     // 삭제

			min = ptr;                                     // min 변경
		}
		else min = NULL;                                   // heap에 min의 루트노드 하나만 존재했을 경우 min은 NULL

		return x;
	}

	return -1;
}

void meld() {                                              // 합병
	nodePointer ptr = min, a, b, a_trail, b_trail, temp = NULL;
	int max = -1, degree = 0, join_fail;

	do {                                                   // 합병의 끝과 링크 재조정 위치를 찾기 위한 탐색
		if (ptr->degree > max)
			max = ptr->degree;

		if (ptr->link == min)
			temp = ptr;

		ptr = ptr->link;

	} while (ptr != min);

	while (degree <= max) {                                // degree가 가장 큰 값이 될 때까지 합병
		join_fail = 1;                                     // 합병이 존재했는지 검사
		ptr = min;
		a = NULL;
		a_trail = ptr;
		b_trail = min;

		do {
			if (ptr->degree == degree) {                   // 가장 낮은 degree부터 탐색
				if (!a)                                    // 첫번째 합병할 트리
					a = ptr;
				else {
					b = ptr;                               // 두번째 합병할 트리

					if (a->data < b->data) {               // a의 루트노드가 더 작으면 b를 a의 child에 붙임
						b_trail->link = b->link;

						if (b == temp)                     // 링크 재조정을 위한 temp값이 변경될 경우이므로 temp값을 temp앞의 노드로 변경
							b_trail = temp;

						ptr = join(a, b);
					}
					else {                                 // b의 루트노드가 더 작으면 a를 b의 child에 붙임
						a_trail->link = a->link;
						ptr = join(b, a);
					}

					join_fail = 0;                         // 합병한 경우이므로 표시

					if (ptr->degree > max)                 // 합병을 통해 max degree가 바뀔 경우 변경
						max = ptr->degree;

					break;                                 // break후 처음부터 다시 탐색
				}
			}
			
			if (!a)
				a_trail = ptr;
			else b_trail = ptr;
			ptr = ptr->link;
		} while (ptr != min);

		if (join_fail)                                     // 합병이 없었을 경우 다음(높은) degree 탐색
			degree++;
	}
}

nodePointer join(nodePointer a, nodePointer b) {           // 트리 합침
	if (!(a->child)) {                                     // a에게 child가 없었을 경우 b는 자가 순환
		b->link = b;
	}
	else {                                                 // a에게 child가 있었을 경우 link로 child 연결
		b->link = a->child->link;
		a->child->link = b;
	}
	a->child = b;                                          // a의 child b로 변경
	(a->degree)++;                                         // a의 child가 늘었으므로 degree 증가

	return a;
}

void printAll(FILE *fp) {                                  // heap 출력
	nodePointer ptr, start, root = min;

	if (min) {                                             // heap에 값이 존재할 경우
		do {
			fprintf(fp, "degree %d : ", root->degree);     // 트리의 degree 출력

			addQ(root);                                    // queue를 통해 level order 출력

			while (front) {
				ptr = deleteQ();
				fprintf(fp, "%d ", ptr->data);

				ptr = ptr->child;

				if (ptr) {                                 // child가 있었을 경우 queue에 삽입
					start = ptr;

					do {
						addQ(ptr);

						ptr = ptr->link;
					} while (ptr != start);
				}
			}

			fprintf(fp, "\n");

			root = root->link;                             // heap의 다음 트리 탐색 및 출력
		} while (root != min);
	}
}

void addQ(nodePointer key) {                               // add queue
	queuePointer temp;

	MALLOC(temp, sizeof(*temp));
	
	temp->data = key;                                      // 값 초기화
	temp->next = NULL;

	if (!front) {                                          // queue가 비어있었을 경우
		front = temp;
		rear = temp;
	}
	else {                                                 // queue에 값이 있었을 경우 rear에 연결
		rear->next = temp;
		rear = temp;
	}
}

nodePointer deleteQ() {                                    // delete queue
	queuePointer ptr;
	nodePointer key;

	key = front->data;                                     // 반환할 값 저장
	ptr = front;
	
	front = front->next;

	free(ptr);                                             // 저장공간 해제(삭제)

	return key;                                            // 값 반환
}