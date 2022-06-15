/*
	2017111248 황성재
	Program #9
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC(p,s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct patriciaTree *patricia;
struct patriciaTree {
	int bitNumber;
	int data;
	patricia leftChild, rightChild;
};

typedef struct queueStruct *queue;                    // level order 출력을 위한 queue 구조체
struct queueStruct {
	patricia data;
	queue next;
};

patricia root = NULL;
queue front, rear;
int bit_len;

int insert(patricia *t, int key);             // patricia key 삽입
patricia search(patricia t, unsigned k);      // patricia key 탐색
int bit(unsigned k, int num);                 // 왼쪽에서 num번째 bit를 반환
void printTree(patricia t, FILE *fp);         // level order로 Tree 출력
char *printBit(int data);                     // 입력을 십진수로 받아 이진수 출력을 위해 비트 계산후 string으로 반환
void addQ(patricia ptr);                      // level order queue add
patricia deleteQ();                           // level order queue delete

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	char str[20];
	int n, i;
	int k;

	if (argc < 3) {                                        // 인자가 부족할 경우 종료
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

	fscanf(ifp, "%d", &n);                                 // key 갯수 입력
	
	fscanf(ifp, "%s", str);                                // key의 비트 수를 알아내기 위해 첫 key는 string으로 입력 받고 int 변환 후 삽입
	bit_len = strlen(str);
	k = atoi(str);
	insert(&root, k);
	printTree(root, ofp);

	for (i = 1; i < n; i++) {                              // 두번째 key부터 바로 입력 후 삽입
		fscanf(ifp, "%d", &k);

		if (insert(&root, k))                              // 정상적으로 삽입되었을 경우 level order로 트리 출력
			printTree(root, ofp);
		else                                               // 이미 트리에 값이 존재할 경우 0을 반환하여 존재함을 출력
			fprintf(ofp, "key가 이미 트리에 존재합니다.\n\n");
	}

	return 0;
}

int insert(patricia *t, int key) {                         // 트리에 key값 삽입
	patricia cur, parent, last, newNode;
	int i;

	if (!(*t)) {                                           // header가 null이었을 경우 header에 값 초기화
		MALLOC(*t, sizeof(**t));
		(*t)->bitNumber = 0;
		(*t)->data = key;
		(*t)->leftChild = *t;
	}
	else {
		last = search(*t, key);                            // 트리에 key값이 존재하는지 검색
		if (key == last->data)                             // 있을 경우 0 반환
			return 0;

		for (i = 1; bit(key, i) == bit(last->data, i); i++);   // 마지막으로 찾은 노드와 다른 비트를 탐색

		cur = (*t)->leftChild;
		parent = *t;

		while (cur->bitNumber > parent->bitNumber && cur->bitNumber < i) {       // 루트부터 시작하여 key 값이 들어갈 자리 탐색
			parent = cur;
			cur = (bit(key, cur->bitNumber)) ? cur->rightChild : cur->leftChild;
		}
		MALLOC(newNode, sizeof(*newNode));                           // 노드 생성 후 데이터 초기화
		newNode->data = key;
		newNode->bitNumber = i;
		newNode->leftChild = (bit(key, i)) ? cur : newNode;
		newNode->rightChild = (bit(key, i)) ? newNode : cur;
		if (cur == parent->leftChild)                                // parent에서 연결할 위치 탐색
			parent->leftChild = newNode;
		else parent->rightChild = newNode;
	}

	return 1;                                                        // 정상적으로 삽입되었을 경우 1 반환
}

patricia search(patricia t, unsigned k) {                     // tree에서 값 탐색
	patricia cur, next;

	if (!t)                                        // 트리가 비어있을 경우 NULL 반환
		return NULL;
	next = t->leftChild;
	cur = t;
	while (next->bitNumber > cur->bitNumber) {     // 트리가 내려가지 않는 구간을 찾음
		cur = next;
		next = (bit(k, next->bitNumber)) ? next->rightChild : next->leftChild;
	}
	return next;                                   // 올라가거나 셀프 탐색한 것을 반환
}

int bit(unsigned k, int num) {                     // k의 왼쪽에서 num번째 비트 반환
	unsigned a = 1;
	int i;

	for (i = 0; i <= bit_len - num; i++) {         // 십진수로 입력받아 10을 곱하여 계산
		a *= 10;
	}

	k %= a;                                        // 해당 비트 위의 값을 버려준 후
	a /= 10;

	return (k / a);                                // 해당 비트 반환
}

void printTree(patricia t, FILE *fp) {             // level order로 트리 출력
	patricia ptr;
	int count = 0, num = 1, num_child = 0, exist = 0;

	front = NULL;                                  // queue 초기화
	rear = NULL;

	if (t) {                                       // 트리에 값이 있을 경우
		fprintf(fp, "header : %s (%d %s)\n", printBit(t->data), t->bitNumber, printBit(t->leftChild->data)); // header 노드 출력
		if (t->leftChild->bitNumber > 0)           // header의 왼쪽 자식에 값이 있을 경우 queue에 넣어서 계속 출력
			addQ(t->leftChild);
		while (front) {
			ptr = deleteQ();
			if (!ptr)                              // 빈 노드도 출력
				fprintf(fp, "NULL ");
			else                                   // 값이 있을 경우
			{                                      // 값과 좌우 child 출력
				fprintf(fp, "%s (%d %s %s) ", printBit(ptr->data), ptr->bitNumber, printBit(ptr->leftChild->data), printBit(ptr->rightChild->data));

				if (ptr->leftChild->bitNumber > ptr->bitNumber) {    // 왼쪽 자식이 밑으로 갈 경우 add queue
					addQ(ptr->leftChild);
					exist = 1;
				}
				else addQ(NULL);                                     // 왼쪽 자식이 셀프 루프거나 위로 갈 경우 NULL을 add
				if (ptr->rightChild->bitNumber > ptr->bitNumber) {   // 오른쪽 자식이 밑으로 갈 경우 add queue
					addQ(ptr->rightChild);
					exist = 1;
				}
				else addQ(NULL);                                     // 오른쪽 자식이 셀프 루프거나 위로 갈 경우 NULL을 add
				num_child += 2;
			}

			count++;
			if (count == num) {                                      // 레벨이 변경될 경우 줄바꿈 문자 출력
				fprintf(fp, "\n");
				num = num_child;                                     // null로 인해 한 레벨이 무조건 2의 n승이 아니므로 갯수를 따로 체크
				num_child = 0;
				count = 0;
				if (!exist)
					break;
				exist = 0;
			}
		}
		while (front)                                                // queue free를 위한 호출
			deleteQ();
		fprintf(fp, "\n");                                           // 트리 구분을 위한 줄바꿈 문자
	}
}

char *printBit(int data) {                         // 십진수로 입력받아서 비트 계산 후 string으로 변환하여 반환
	char *str;
	int a = 1, i;

	MALLOC(str, sizeof(*str)*(bit_len+1));         // 마지막 '\0'을 위해 +1 해줌

	for (i = 1; i < bit_len; i++)                  // 비트 최상위와 크기를 맞춰줌
		a *= 10;
	for (i = bit_len; i > 0; i--) {
		str[bit_len - i] = data / a + '0';         // 문자로 바꾸기 위한 + '0'
		data %= a;
		a /= 10;
	}
	str[bit_len] = 0;                              // 마지막 null문자를 채워줌

	return str;                                    // bit로 변환한 string 반환
}

void addQ(patricia ptr) {                    // level order queue add
	queue temp;

	MALLOC(temp, sizeof(*temp));             // 동적할당 및 초기화
	temp->data = ptr;
	temp->next = NULL;
	
	if (!front) {                            // queue가 비었을 경우 front와 rear 둘 다 초기화
		front = temp;
		rear = temp;
	}
	else {                                   // queue에 값이 있을 경우 rear에 연결
		rear->next = temp;
		rear = rear->next;
	}
}

patricia deleteQ() {                         // level order queue delete
	patricia temp;
	queue avail;

	temp = front->data;                      // 반환할 값을 temp에 저장후 rear를 옮겨줌
	avail = front;
	front = front->next;

	free(avail);                             // 반환해 줄 값을 갖고 있던 queue는 동적할당해제

	return temp;                             // 값 반환
}