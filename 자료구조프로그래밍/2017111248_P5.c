// 2017111248 황성재

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

#define REALLOC(p, s)\
	if(!(p = realloc(p, s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct leftist *leftistTree;           // leftist Tree 구조체
typedef struct leftist {
	int data, shortest;
	leftistTree leftChild, rightChild;
};

leftistTree *queue, *levelQueue;
int size, front = -1, rear, levelFront = -1, levelRear = -1, levelSize, depth;

void findDepth(leftistTree ptr, int *max, int cur);  // tree의 depth 찾는 함수
void printTree(leftistTree ptr, FILE *fp);           // tree 출력
leftistTree meld(leftistTree a, leftistTree b);      // tree 합병
int findMin(int a, int b);                           // 두 숫자 비교 후 작은 수 return
leftistTree makeTree(int key);                       // tree 생성
void addQ(leftistTree ptr);                          // queue add
leftistTree deleteQ();                               // queue delete
void addLQ(leftistTree ptr);                         // level order queue add
leftistTree deleteLQ();                              // level order queue delete

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	leftistTree a = NULL, b, temp;
	int i, data, count = 0;

	if (argc < 3) {                               // 인자가 적으면 프로그램 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {           // 입력파일이 열리지 않으면 종료
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {           // 출력파일이 열리지 않으면 종료
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &size);
	MALLOC(queue, sizeof(*queue)*(size + 1));     // circular queue 동적할당
	MALLOC(levelQueue, sizeof(*levelQueue) * 2);  // leverorder circular queue 동적할당

	rear = size - 1;

	depth = 1;
	levelSize = 2;

	for (i = 0; i < size; i++) {
		fscanf(ifp, "%d", &data);                 // 숫자 입력
		
		MALLOC(queue[i], sizeof(*queue[i]));      // 트리를 생성할 공간 동적 할당
		queue[i] = makeTree(data);                // 입력받은 수로 트리 생성
		fprintf(ofp, "%d번째 ", ++count);         // 몇 번째 트리인지 출력
		printTree(queue[i], ofp);                 // 트리 출력
	}

	while (front != rear) {                       // queue가 빌 때까지
		a = deleteQ();
		b = deleteQ();
		if (b == NULL)                            // b가 NULL인 것은 하나 밖에 안남은 것이므로 종료
			break;

		temp = meld(a, b);                        // a와 b 합병

		depth = -1;                               // 합병된 트리의 깊이 및 levelorder queue 크기 계산
		findDepth(temp, &depth, 1);
		levelSize = pow(2, depth - 1) + 1;
		REALLOC(levelQueue, sizeof(*levelQueue)*levelSize);        // levelorder queue 크기 조정

		fprintf(ofp, "%d번째 ", ++count);         // 몇 번째 트리인지 출력
		printTree(temp, ofp);                     // 트리 출력

		addQ(temp);                               // 합병된 트리 circular queue에 다시 add
	}

	return 0;
}

void findDepth(leftistTree ptr, int *max, int cur) {     // 트리의 depth를 찾아서 max에 넣음
	if (ptr->shortest) {                          // 트리의 루트노드가 internal일 때
		if (cur > *max)                           // 현재 level이 max보다 크면 교체
			*max = cur;

		findDepth(ptr->leftChild, max, cur + 1);  // preorder 방식으로 탐색
		findDepth(ptr->rightChild, max, cur + 1);
	}
}

void printTree(leftistTree ptr, FILE *fp) {       // 트리 출력
	leftistTree temp;
	int count = 0, i, level = 1, max_index = 1, index_count = 0;

	addLQ(ptr);                                   // 루트노드 leverorder queue에 add
	fprintf(fp, "root: ");                        // 시작점 출력
	while (levelFront != levelRear) {             // leverorder queue가 빌 때까지
		temp = deleteLQ();                        // queue에서 노드를 받아옴

		index_count++;                            // 현재 레벨에서 몇번째인지 체크

		if (!(temp->shortest)) count++;           // external일 경우 -출력을 위해 counting
		else {                                    // internal일 경우 출력
			for (i = 0; i < count; i++)           // internal 나오기 전까지의 external 노드 수만큼 출력
				fprintf(fp, "- ");
			fprintf(fp, "%d ", temp->data);       // internal 노드(현재 탐색중인 노드) 출력
		}

		if (level != depth) {                     // level이 마지막이 아닐 때
			if (temp->shortest) {                 // internal 노드일 경우 자식 노드들을 queue에 add
				addLQ(temp->leftChild);           // 왼쪽 자식
				addLQ(temp->rightChild);          // 오른쪽 자식
			}

			if (index_count == max_index) {       // 레벨의 마지막 노드일 경우
				for (i = 0; i < count; i++)       // external 노드 수만큼 출력
					fprintf(fp, "- ");
				count = 0;                        // 초기화
				fprintf(fp, "\n");                // 레벨이 바뀌므로 줄바꿈 문자 출력
				index_count = 0;                  // 레벨이 바뀌므로 레벨의 0번째로 초기화
				max_index = pow(2, level);        // 레벨의 최대 노드 수 계산
				level++;                          // 레벨 증가
			}
		}
	}
	fprintf(fp, "\n\n");                          // 트리 구분을 위한 줄바꿈 문자
}

leftistTree meld(leftistTree a, leftistTree b) {  // 트리 합병
	leftistTree temp;

	if (!(a->shortest)) {                         // a 트리의 root 노드가 external일 경우 a 제거 후 b return
		free(a);
		return b;
	}
	else if (!(b->shortest)) {                    // b 트리의 root 노드가 external일 경우 b 제거 후 a return
		free(b);
		return a;
	}

	if (a->data < b->data) {                      // a 트리의 root 노드가 더 작을 경우
		a->rightChild = meld(a->rightChild, b);   // a의 오른쪽 서브 트리와 b 합병
		
		if (a->rightChild->shortest > a->leftChild->shortest) {    // a의 오른쪽 자식 노드의 shortest가 더 클 경우 자식 둘의 위치 변경
			temp = a->leftChild;
			a->leftChild = a->rightChild;
			a->rightChild = temp;
		}

		a->shortest = a->rightChild->shortest + 1;// a의 shortest 다시 계산

		return a;                                 // 합병 된 트리 return
	}
	b->rightChild = meld(a, b->rightChild);       // b 트리의 root 노드가 더 작을 경우 b의 오른쪽 서브 트리와 a 합병

	if (b->rightChild->shortest > b->leftChild->shortest) {      // b의 오른쪽 자식 노드의 shortest가 더 클 경우 자식 둘의 위치 변경
		temp = b->leftChild;
		b->leftChild = b->rightChild;
		b->rightChild = temp;
	}

	b->shortest = b->rightChild->shortest + 1;    // b의 shortest 다시 계산

	return b;                                     // 합병 된 트리 return
}

int findMin(int a, int b) {                       // a와 b 중 작은 수 return
	if (a < b)
		return a;
	return b;
}

leftistTree makeTree(int key) {                   // 트리 생성
	leftistTree temp[3];
	int i;

	for (i = 0; i < 3; i++) {                     // root와 그에 따라올 external 노드 2개 생성
		MALLOC(temp[i], sizeof(*temp[i]));
	}

	for (i = 1; i < 3; i++) {                     // external 노드 초기화
		temp[i]->shortest = 0;
		temp[i]->leftChild = NULL;
		temp[i]->rightChild = NULL;
	}

	temp[0]->data = key;                          // root 노드 초기화 후 external 노드를 자식노드로 붙임
	temp[0]->shortest = 1;
	temp[0]->leftChild = temp[1];
	temp[0]->rightChild = temp[2];

	return temp[0];
}

void addQ(leftistTree ptr) {                      // add circular queue
	rear = (rear + 1) % (size + 1);
	
	queue[rear] = ptr;
}

leftistTree deleteQ() {                           // delete circular queue
	if (rear == front)
		return NULL;

	front = (front + 1) % (size + 1);

	return queue[front];
}

void addLQ(leftistTree ptr) {                     // add level order circular queue
	levelRear = (levelRear + 1) % (levelSize);

	levelQueue[levelRear] = ptr;
}

leftistTree deleteLQ() {                          // delete lever order circular queue
	if (levelRear == levelFront)
		return NULL;

	levelFront = (levelFront + 1) % levelSize;

	return levelQueue[levelFront];
}