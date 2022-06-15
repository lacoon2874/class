/*
	2017111248 황성재
	Program #7
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define MALLOC(p,s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct node *nodePointer;
typedef struct node{
	int key, bf;
	nodePointer leftChild, rightChild;
};

typedef struct queue *queuePointer;                      // level order 출력을 위한 Queue
typedef struct queue {
	nodePointer data;
	queuePointer next;
};

queuePointer front = NULL, rear = NULL;

void printTree(nodePointer ptr, FILE *fp, int n);
void addQ(nodePointer ptr);
nodePointer deleteQ();
void avlInsert(nodePointer *parent, int x, int *unbalanced);
void leftRotation(nodePointer *parent, int *unbalanced);
void rightRotation(nodePointer *parent, int *unbalanced);

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	int n, i, key, unbalanced;
	nodePointer avlTree = NULL;                              // AVL Tree

	if (argc < 3) {                                          // 인자가 부족할 경우 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                      // 입력 파일이 열리지 않을 경우 종료
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                      // 출력 파일이 열리지 않을 경우 종료
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                                   // key 갯수 입력
	for (i = 0; i < n; i++) {                                // 트리 생성 및 출력
		fscanf(ifp, "%d", &key);

		avlInsert(&avlTree, key, &unbalanced);               // 트리 생성

		printTree(avlTree, ofp, i);                          // 트리 출력
	}

	return 0;
}

void printTree(nodePointer ptr, FILE *fp, int n) {           // 트리 출력 함수
	int count = 0, a = 0, b = 1;

	addQ(ptr);

	fprintf(fp, "root : ");                                  // 트리 시작 부분 표시를 위한 루트 출력

	while (count <= n) {
		if (a == b) {                                        // level 변경에 따른 줄바꿈문자 출력
			fprintf(fp, "\n");
			a = 0;
			b *= 2;
		}

		ptr = deleteQ();

		if (ptr != NULL) {                                   // 빈 노드가 아니었을 경우 key 출력
			fprintf(fp, "%d ", ptr->key);
			count++;                                         // key의 출력이 끝났는지 확인하는 용도

			addQ(ptr->leftChild);
			addQ(ptr->rightChild);
		}
		else {                                               // 빈 노드였을 경우 "-" 출력
			fprintf(fp, "- ");
		}
		a++;                                                 // 레벨에서의 출력한 노드 갯수 증가
	}
	while (front) {                                          // NULL이 계속 들어가도록 되어 있으므로 동적할당 해제를 위한 deleteQ
		deleteQ();
	}
	fprintf(fp, "\n\n");                                     // 출력된 트리 구분을 위해 줄바꿈 문자 출력
}

void addQ(nodePointer ptr) {                                 // add queue
	queuePointer temp;

	MALLOC(temp, sizeof(*temp));                             // 새로운 값을 넣을 공간 할당 및 초기화
	temp->data = ptr;
	temp->next = NULL;

	if (!front) {                                            // 빈 queue였을 경우
		front = temp;
		rear = temp;
	}
	else {                                                   // queue에 값이 있었을 경우 뒤에 추가
		rear->next = temp;
		rear = temp;
	}
}

nodePointer deleteQ() {                                      // delete queue
	nodePointer temp;
	queuePointer avail;

	temp = front->data;                                      // 반환할 값 저장
	avail = front;                                           // 없앨 값의 포인터 저장
	front = front->next;                                     // front 증가
	
	free(avail);                                             // 동적할당 해제

	return temp;                                             // 반환
}

void avlInsert(nodePointer *parent, int x, int *unbalanced) {// AVL tree에 새로운 값 insert
	if (!*parent) {                                          // 현재 tree가 비어있을 경우
		*unbalanced = TRUE;                                  // unbalance
		MALLOC(*parent, sizeof(nodePointer));                // 새로운 노드 할당 및 초기화
		(*parent)->leftChild = NULL;
		(*parent)->rightChild = NULL;
		(*parent)->bf = 0;
		(*parent)->key = x;
	}
	else if (x < (*parent)->key) {                           // 새로 들어온 값이 자기자신보다 작을 경우
		avlInsert(&(*parent)->leftChild, x, unbalanced);     // 왼쪽에 insert
		if(*unbalanced)                                      // unbalance이면 bf값 정리
			switch ((*parent)->bf) {
			case -1:                                         // 오른쪽의 height가 큰 경우였는데 왼쪽 insert를 통해 0이됨, balance
				(*parent)->bf = 0;
				*unbalanced = FALSE;
				break;
			case 0:                                          // balance였지만 왼쪽 height가 1 커짐
				(*parent)->bf = 1;
				break;
			case 1:                                          // 왼쪽의 height가 1 큰채로 1이 추가되어 Left회전 필요
				leftRotation(parent, unbalanced);
			}
	}
	else if (x > (*parent)->key) {                           // 새로 들어온 값이 자기자신보다 작을 경우
		avlInsert(&(*parent)->rightChild, x, unbalanced);    // 오른쪽에 insert
		if(*unbalanced)                                      // unbalance이면 bf값 정리
			switch ((*parent)->bf) {
			case 1:                                          // 왼쪽의 height가 큰 경우였는데 오른쪽 insert를 통해 0이됨, balance
				(*parent)->bf = 0;
				*unbalanced = FALSE;
				break;
			case 0:                                          // balance였지만 오른쪽 height가 1 커짐
				(*parent)->bf = -1;
				break;
			case -1:                                         // 오른쪽의 height가 1 큰채로 1이 추가되어 right회전 필요
				rightRotation(parent, unbalanced);
			}
	}
	else {                                                   // 같은 값이 있을 경우 예외 처리
		*unbalanced = FALSE;
		printf("The key is already in the tree");
	}
}

void leftRotation(nodePointer *parent, int *unbalanced) {    // left 회전
	nodePointer grandChild, child;
	
	child = (*parent)->leftChild;							 // 왼쪽에 들어왔으므로 child는 left
	if (child->bf == 1) {                                    // LL 회전
		(*parent)->leftChild = child->rightChild;
		child->rightChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {                                                   // LR 회전
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		grandChild->leftChild = child;
		(*parent)->leftChild = grandChild->rightChild;
		grandChild->rightChild = *parent;

		switch (grandChild->bf) {                            // grandChlid의 bf에 child와 parent의 bf가 영향을 받음, ch->left와 pa->right가 h임
		case 1:                                              // gr->left는 h, gr->right는 h-1이었던 경우
			(*parent)->bf = -1;
			child->bf = 0;
			break;
		case 0:                                              // gr->left와 gr->right가 h였던 경우
			(*parent)->bf = 0;
			child->bf = 0;
			break;
		case -1:                                             // gr->left는 h-1, gr->right는 h였던경우
			(*parent)->bf = 0;
			child->bf = 1;
		}
		*parent = grandChild;
	}
	(*parent)->bf = 0;
	*unbalanced = FALSE;                                     // balance
}

void rightRotation(nodePointer *parent, int *unbalanced) {   // left 회전
	nodePointer grandChild, child;

	child = (*parent)->rightChild;                           // 오른쪽에 들어왔으므로 child는 right
	if (child->bf == -1) {                                   // RR 회전
		(*parent)->rightChild = child->leftChild;
		child->leftChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {                                                   // RL 회전
		grandChild = child->leftChild;
		child->leftChild = grandChild->rightChild;
		grandChild->rightChild = child;
		(*parent)->rightChild = grandChild->leftChild;
		grandChild->leftChild = *parent;

		switch (grandChild->bf) {                            // grandChlid의 bf에 child와 parent의 bf가 영향을 받음, ch->right와 pa->left가 h임
		case 1:                                              // gr->left는 h, gr->right는 h-1이었던 경우
			(*parent)->bf = 0;
			child->bf = -1;
			break;
		case 0:                                              // gr->left와 gr->right가 h였던 경우
			(*parent)->bf = 0;
			child->bf = 0;
			break;
		case -1:                                             // gr->left는 h-1, gr->right는 h였던경우
			(*parent)->bf = 1;
			child->bf = 0;
		}
		*parent = grandChild;
	}
	(*parent)->bf = 0;
	*unbalanced = FALSE;                                     // balance
}