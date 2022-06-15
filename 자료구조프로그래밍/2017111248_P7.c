/*
	2017111248 Ȳ����
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

typedef struct queue *queuePointer;                      // level order ����� ���� Queue
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

	if (argc < 3) {                                          // ���ڰ� ������ ��� ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                      // �Է� ������ ������ ���� ��� ����
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                      // ��� ������ ������ ���� ��� ����
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                                   // key ���� �Է�
	for (i = 0; i < n; i++) {                                // Ʈ�� ���� �� ���
		fscanf(ifp, "%d", &key);

		avlInsert(&avlTree, key, &unbalanced);               // Ʈ�� ����

		printTree(avlTree, ofp, i);                          // Ʈ�� ���
	}

	return 0;
}

void printTree(nodePointer ptr, FILE *fp, int n) {           // Ʈ�� ��� �Լ�
	int count = 0, a = 0, b = 1;

	addQ(ptr);

	fprintf(fp, "root : ");                                  // Ʈ�� ���� �κ� ǥ�ø� ���� ��Ʈ ���

	while (count <= n) {
		if (a == b) {                                        // level ���濡 ���� �ٹٲ޹��� ���
			fprintf(fp, "\n");
			a = 0;
			b *= 2;
		}

		ptr = deleteQ();

		if (ptr != NULL) {                                   // �� ��尡 �ƴϾ��� ��� key ���
			fprintf(fp, "%d ", ptr->key);
			count++;                                         // key�� ����� �������� Ȯ���ϴ� �뵵

			addQ(ptr->leftChild);
			addQ(ptr->rightChild);
		}
		else {                                               // �� ��忴�� ��� "-" ���
			fprintf(fp, "- ");
		}
		a++;                                                 // ���������� ����� ��� ���� ����
	}
	while (front) {                                          // NULL�� ��� ������ �Ǿ� �����Ƿ� �����Ҵ� ������ ���� deleteQ
		deleteQ();
	}
	fprintf(fp, "\n\n");                                     // ��µ� Ʈ�� ������ ���� �ٹٲ� ���� ���
}

void addQ(nodePointer ptr) {                                 // add queue
	queuePointer temp;

	MALLOC(temp, sizeof(*temp));                             // ���ο� ���� ���� ���� �Ҵ� �� �ʱ�ȭ
	temp->data = ptr;
	temp->next = NULL;

	if (!front) {                                            // �� queue���� ���
		front = temp;
		rear = temp;
	}
	else {                                                   // queue�� ���� �־��� ��� �ڿ� �߰�
		rear->next = temp;
		rear = temp;
	}
}

nodePointer deleteQ() {                                      // delete queue
	nodePointer temp;
	queuePointer avail;

	temp = front->data;                                      // ��ȯ�� �� ����
	avail = front;                                           // ���� ���� ������ ����
	front = front->next;                                     // front ����
	
	free(avail);                                             // �����Ҵ� ����

	return temp;                                             // ��ȯ
}

void avlInsert(nodePointer *parent, int x, int *unbalanced) {// AVL tree�� ���ο� �� insert
	if (!*parent) {                                          // ���� tree�� ������� ���
		*unbalanced = TRUE;                                  // unbalance
		MALLOC(*parent, sizeof(nodePointer));                // ���ο� ��� �Ҵ� �� �ʱ�ȭ
		(*parent)->leftChild = NULL;
		(*parent)->rightChild = NULL;
		(*parent)->bf = 0;
		(*parent)->key = x;
	}
	else if (x < (*parent)->key) {                           // ���� ���� ���� �ڱ��ڽź��� ���� ���
		avlInsert(&(*parent)->leftChild, x, unbalanced);     // ���ʿ� insert
		if(*unbalanced)                                      // unbalance�̸� bf�� ����
			switch ((*parent)->bf) {
			case -1:                                         // �������� height�� ū ��쿴�µ� ���� insert�� ���� 0�̵�, balance
				(*parent)->bf = 0;
				*unbalanced = FALSE;
				break;
			case 0:                                          // balance������ ���� height�� 1 Ŀ��
				(*parent)->bf = 1;
				break;
			case 1:                                          // ������ height�� 1 ūä�� 1�� �߰��Ǿ� Leftȸ�� �ʿ�
				leftRotation(parent, unbalanced);
			}
	}
	else if (x > (*parent)->key) {                           // ���� ���� ���� �ڱ��ڽź��� ���� ���
		avlInsert(&(*parent)->rightChild, x, unbalanced);    // �����ʿ� insert
		if(*unbalanced)                                      // unbalance�̸� bf�� ����
			switch ((*parent)->bf) {
			case 1:                                          // ������ height�� ū ��쿴�µ� ������ insert�� ���� 0�̵�, balance
				(*parent)->bf = 0;
				*unbalanced = FALSE;
				break;
			case 0:                                          // balance������ ������ height�� 1 Ŀ��
				(*parent)->bf = -1;
				break;
			case -1:                                         // �������� height�� 1 ūä�� 1�� �߰��Ǿ� rightȸ�� �ʿ�
				rightRotation(parent, unbalanced);
			}
	}
	else {                                                   // ���� ���� ���� ��� ���� ó��
		*unbalanced = FALSE;
		printf("The key is already in the tree");
	}
}

void leftRotation(nodePointer *parent, int *unbalanced) {    // left ȸ��
	nodePointer grandChild, child;
	
	child = (*parent)->leftChild;							 // ���ʿ� �������Ƿ� child�� left
	if (child->bf == 1) {                                    // LL ȸ��
		(*parent)->leftChild = child->rightChild;
		child->rightChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {                                                   // LR ȸ��
		grandChild = child->rightChild;
		child->rightChild = grandChild->leftChild;
		grandChild->leftChild = child;
		(*parent)->leftChild = grandChild->rightChild;
		grandChild->rightChild = *parent;

		switch (grandChild->bf) {                            // grandChlid�� bf�� child�� parent�� bf�� ������ ����, ch->left�� pa->right�� h��
		case 1:                                              // gr->left�� h, gr->right�� h-1�̾��� ���
			(*parent)->bf = -1;
			child->bf = 0;
			break;
		case 0:                                              // gr->left�� gr->right�� h���� ���
			(*parent)->bf = 0;
			child->bf = 0;
			break;
		case -1:                                             // gr->left�� h-1, gr->right�� h�������
			(*parent)->bf = 0;
			child->bf = 1;
		}
		*parent = grandChild;
	}
	(*parent)->bf = 0;
	*unbalanced = FALSE;                                     // balance
}

void rightRotation(nodePointer *parent, int *unbalanced) {   // left ȸ��
	nodePointer grandChild, child;

	child = (*parent)->rightChild;                           // �����ʿ� �������Ƿ� child�� right
	if (child->bf == -1) {                                   // RR ȸ��
		(*parent)->rightChild = child->leftChild;
		child->leftChild = *parent;
		(*parent)->bf = 0;
		(*parent) = child;
	}
	else {                                                   // RL ȸ��
		grandChild = child->leftChild;
		child->leftChild = grandChild->rightChild;
		grandChild->rightChild = child;
		(*parent)->rightChild = grandChild->leftChild;
		grandChild->leftChild = *parent;

		switch (grandChild->bf) {                            // grandChlid�� bf�� child�� parent�� bf�� ������ ����, ch->right�� pa->left�� h��
		case 1:                                              // gr->left�� h, gr->right�� h-1�̾��� ���
			(*parent)->bf = 0;
			child->bf = -1;
			break;
		case 0:                                              // gr->left�� gr->right�� h���� ���
			(*parent)->bf = 0;
			child->bf = 0;
			break;
		case -1:                                             // gr->left�� h-1, gr->right�� h�������
			(*parent)->bf = 1;
			child->bf = 0;
		}
		*parent = grandChild;
	}
	(*parent)->bf = 0;
	*unbalanced = FALSE;                                     // balance
}