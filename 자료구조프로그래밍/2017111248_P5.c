// 2017111248 Ȳ����

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

typedef struct leftist *leftistTree;           // leftist Tree ����ü
typedef struct leftist {
	int data, shortest;
	leftistTree leftChild, rightChild;
};

leftistTree *queue, *levelQueue;
int size, front = -1, rear, levelFront = -1, levelRear = -1, levelSize, depth;

void findDepth(leftistTree ptr, int *max, int cur);  // tree�� depth ã�� �Լ�
void printTree(leftistTree ptr, FILE *fp);           // tree ���
leftistTree meld(leftistTree a, leftistTree b);      // tree �պ�
int findMin(int a, int b);                           // �� ���� �� �� ���� �� return
leftistTree makeTree(int key);                       // tree ����
void addQ(leftistTree ptr);                          // queue add
leftistTree deleteQ();                               // queue delete
void addLQ(leftistTree ptr);                         // level order queue add
leftistTree deleteLQ();                              // level order queue delete

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	leftistTree a = NULL, b, temp;
	int i, data, count = 0;

	if (argc < 3) {                               // ���ڰ� ������ ���α׷� ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {           // �Է������� ������ ������ ����
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {           // ��������� ������ ������ ����
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &size);
	MALLOC(queue, sizeof(*queue)*(size + 1));     // circular queue �����Ҵ�
	MALLOC(levelQueue, sizeof(*levelQueue) * 2);  // leverorder circular queue �����Ҵ�

	rear = size - 1;

	depth = 1;
	levelSize = 2;

	for (i = 0; i < size; i++) {
		fscanf(ifp, "%d", &data);                 // ���� �Է�
		
		MALLOC(queue[i], sizeof(*queue[i]));      // Ʈ���� ������ ���� ���� �Ҵ�
		queue[i] = makeTree(data);                // �Է¹��� ���� Ʈ�� ����
		fprintf(ofp, "%d��° ", ++count);         // �� ��° Ʈ������ ���
		printTree(queue[i], ofp);                 // Ʈ�� ���
	}

	while (front != rear) {                       // queue�� �� ������
		a = deleteQ();
		b = deleteQ();
		if (b == NULL)                            // b�� NULL�� ���� �ϳ� �ۿ� �ȳ��� ���̹Ƿ� ����
			break;

		temp = meld(a, b);                        // a�� b �պ�

		depth = -1;                               // �պ��� Ʈ���� ���� �� levelorder queue ũ�� ���
		findDepth(temp, &depth, 1);
		levelSize = pow(2, depth - 1) + 1;
		REALLOC(levelQueue, sizeof(*levelQueue)*levelSize);        // levelorder queue ũ�� ����

		fprintf(ofp, "%d��° ", ++count);         // �� ��° Ʈ������ ���
		printTree(temp, ofp);                     // Ʈ�� ���

		addQ(temp);                               // �պ��� Ʈ�� circular queue�� �ٽ� add
	}

	return 0;
}

void findDepth(leftistTree ptr, int *max, int cur) {     // Ʈ���� depth�� ã�Ƽ� max�� ����
	if (ptr->shortest) {                          // Ʈ���� ��Ʈ��尡 internal�� ��
		if (cur > *max)                           // ���� level�� max���� ũ�� ��ü
			*max = cur;

		findDepth(ptr->leftChild, max, cur + 1);  // preorder ������� Ž��
		findDepth(ptr->rightChild, max, cur + 1);
	}
}

void printTree(leftistTree ptr, FILE *fp) {       // Ʈ�� ���
	leftistTree temp;
	int count = 0, i, level = 1, max_index = 1, index_count = 0;

	addLQ(ptr);                                   // ��Ʈ��� leverorder queue�� add
	fprintf(fp, "root: ");                        // ������ ���
	while (levelFront != levelRear) {             // leverorder queue�� �� ������
		temp = deleteLQ();                        // queue���� ��带 �޾ƿ�

		index_count++;                            // ���� �������� ���°���� üũ

		if (!(temp->shortest)) count++;           // external�� ��� -����� ���� counting
		else {                                    // internal�� ��� ���
			for (i = 0; i < count; i++)           // internal ������ �������� external ��� ����ŭ ���
				fprintf(fp, "- ");
			fprintf(fp, "%d ", temp->data);       // internal ���(���� Ž������ ���) ���
		}

		if (level != depth) {                     // level�� �������� �ƴ� ��
			if (temp->shortest) {                 // internal ����� ��� �ڽ� ������ queue�� add
				addLQ(temp->leftChild);           // ���� �ڽ�
				addLQ(temp->rightChild);          // ������ �ڽ�
			}

			if (index_count == max_index) {       // ������ ������ ����� ���
				for (i = 0; i < count; i++)       // external ��� ����ŭ ���
					fprintf(fp, "- ");
				count = 0;                        // �ʱ�ȭ
				fprintf(fp, "\n");                // ������ �ٲ�Ƿ� �ٹٲ� ���� ���
				index_count = 0;                  // ������ �ٲ�Ƿ� ������ 0��°�� �ʱ�ȭ
				max_index = pow(2, level);        // ������ �ִ� ��� �� ���
				level++;                          // ���� ����
			}
		}
	}
	fprintf(fp, "\n\n");                          // Ʈ�� ������ ���� �ٹٲ� ����
}

leftistTree meld(leftistTree a, leftistTree b) {  // Ʈ�� �պ�
	leftistTree temp;

	if (!(a->shortest)) {                         // a Ʈ���� root ��尡 external�� ��� a ���� �� b return
		free(a);
		return b;
	}
	else if (!(b->shortest)) {                    // b Ʈ���� root ��尡 external�� ��� b ���� �� a return
		free(b);
		return a;
	}

	if (a->data < b->data) {                      // a Ʈ���� root ��尡 �� ���� ���
		a->rightChild = meld(a->rightChild, b);   // a�� ������ ���� Ʈ���� b �պ�
		
		if (a->rightChild->shortest > a->leftChild->shortest) {    // a�� ������ �ڽ� ����� shortest�� �� Ŭ ��� �ڽ� ���� ��ġ ����
			temp = a->leftChild;
			a->leftChild = a->rightChild;
			a->rightChild = temp;
		}

		a->shortest = a->rightChild->shortest + 1;// a�� shortest �ٽ� ���

		return a;                                 // �պ� �� Ʈ�� return
	}
	b->rightChild = meld(a, b->rightChild);       // b Ʈ���� root ��尡 �� ���� ��� b�� ������ ���� Ʈ���� a �պ�

	if (b->rightChild->shortest > b->leftChild->shortest) {      // b�� ������ �ڽ� ����� shortest�� �� Ŭ ��� �ڽ� ���� ��ġ ����
		temp = b->leftChild;
		b->leftChild = b->rightChild;
		b->rightChild = temp;
	}

	b->shortest = b->rightChild->shortest + 1;    // b�� shortest �ٽ� ���

	return b;                                     // �պ� �� Ʈ�� return
}

int findMin(int a, int b) {                       // a�� b �� ���� �� return
	if (a < b)
		return a;
	return b;
}

leftistTree makeTree(int key) {                   // Ʈ�� ����
	leftistTree temp[3];
	int i;

	for (i = 0; i < 3; i++) {                     // root�� �׿� ����� external ��� 2�� ����
		MALLOC(temp[i], sizeof(*temp[i]));
	}

	for (i = 1; i < 3; i++) {                     // external ��� �ʱ�ȭ
		temp[i]->shortest = 0;
		temp[i]->leftChild = NULL;
		temp[i]->rightChild = NULL;
	}

	temp[0]->data = key;                          // root ��� �ʱ�ȭ �� external ��带 �ڽĳ��� ����
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