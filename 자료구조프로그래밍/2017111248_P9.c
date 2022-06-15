/*
	2017111248 Ȳ����
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

typedef struct queueStruct *queue;                    // level order ����� ���� queue ����ü
struct queueStruct {
	patricia data;
	queue next;
};

patricia root = NULL;
queue front, rear;
int bit_len;

int insert(patricia *t, int key);             // patricia key ����
patricia search(patricia t, unsigned k);      // patricia key Ž��
int bit(unsigned k, int num);                 // ���ʿ��� num��° bit�� ��ȯ
void printTree(patricia t, FILE *fp);         // level order�� Tree ���
char *printBit(int data);                     // �Է��� �������� �޾� ������ ����� ���� ��Ʈ ����� string���� ��ȯ
void addQ(patricia ptr);                      // level order queue add
patricia deleteQ();                           // level order queue delete

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	char str[20];
	int n, i;
	int k;

	if (argc < 3) {                                        // ���ڰ� ������ ��� ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                    // input file�� ������ ���� ��� ����
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                    // output file�� ������ ���� ��� ����
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                                 // key ���� �Է�
	
	fscanf(ifp, "%s", str);                                // key�� ��Ʈ ���� �˾Ƴ��� ���� ù key�� string���� �Է� �ް� int ��ȯ �� ����
	bit_len = strlen(str);
	k = atoi(str);
	insert(&root, k);
	printTree(root, ofp);

	for (i = 1; i < n; i++) {                              // �ι�° key���� �ٷ� �Է� �� ����
		fscanf(ifp, "%d", &k);

		if (insert(&root, k))                              // ���������� ���ԵǾ��� ��� level order�� Ʈ�� ���
			printTree(root, ofp);
		else                                               // �̹� Ʈ���� ���� ������ ��� 0�� ��ȯ�Ͽ� �������� ���
			fprintf(ofp, "key�� �̹� Ʈ���� �����մϴ�.\n\n");
	}

	return 0;
}

int insert(patricia *t, int key) {                         // Ʈ���� key�� ����
	patricia cur, parent, last, newNode;
	int i;

	if (!(*t)) {                                           // header�� null�̾��� ��� header�� �� �ʱ�ȭ
		MALLOC(*t, sizeof(**t));
		(*t)->bitNumber = 0;
		(*t)->data = key;
		(*t)->leftChild = *t;
	}
	else {
		last = search(*t, key);                            // Ʈ���� key���� �����ϴ��� �˻�
		if (key == last->data)                             // ���� ��� 0 ��ȯ
			return 0;

		for (i = 1; bit(key, i) == bit(last->data, i); i++);   // ���������� ã�� ���� �ٸ� ��Ʈ�� Ž��

		cur = (*t)->leftChild;
		parent = *t;

		while (cur->bitNumber > parent->bitNumber && cur->bitNumber < i) {       // ��Ʈ���� �����Ͽ� key ���� �� �ڸ� Ž��
			parent = cur;
			cur = (bit(key, cur->bitNumber)) ? cur->rightChild : cur->leftChild;
		}
		MALLOC(newNode, sizeof(*newNode));                           // ��� ���� �� ������ �ʱ�ȭ
		newNode->data = key;
		newNode->bitNumber = i;
		newNode->leftChild = (bit(key, i)) ? cur : newNode;
		newNode->rightChild = (bit(key, i)) ? newNode : cur;
		if (cur == parent->leftChild)                                // parent���� ������ ��ġ Ž��
			parent->leftChild = newNode;
		else parent->rightChild = newNode;
	}

	return 1;                                                        // ���������� ���ԵǾ��� ��� 1 ��ȯ
}

patricia search(patricia t, unsigned k) {                     // tree���� �� Ž��
	patricia cur, next;

	if (!t)                                        // Ʈ���� ������� ��� NULL ��ȯ
		return NULL;
	next = t->leftChild;
	cur = t;
	while (next->bitNumber > cur->bitNumber) {     // Ʈ���� �������� �ʴ� ������ ã��
		cur = next;
		next = (bit(k, next->bitNumber)) ? next->rightChild : next->leftChild;
	}
	return next;                                   // �ö󰡰ų� ���� Ž���� ���� ��ȯ
}

int bit(unsigned k, int num) {                     // k�� ���ʿ��� num��° ��Ʈ ��ȯ
	unsigned a = 1;
	int i;

	for (i = 0; i <= bit_len - num; i++) {         // �������� �Է¹޾� 10�� ���Ͽ� ���
		a *= 10;
	}

	k %= a;                                        // �ش� ��Ʈ ���� ���� ������ ��
	a /= 10;

	return (k / a);                                // �ش� ��Ʈ ��ȯ
}

void printTree(patricia t, FILE *fp) {             // level order�� Ʈ�� ���
	patricia ptr;
	int count = 0, num = 1, num_child = 0, exist = 0;

	front = NULL;                                  // queue �ʱ�ȭ
	rear = NULL;

	if (t) {                                       // Ʈ���� ���� ���� ���
		fprintf(fp, "header : %s (%d %s)\n", printBit(t->data), t->bitNumber, printBit(t->leftChild->data)); // header ��� ���
		if (t->leftChild->bitNumber > 0)           // header�� ���� �ڽĿ� ���� ���� ��� queue�� �־ ��� ���
			addQ(t->leftChild);
		while (front) {
			ptr = deleteQ();
			if (!ptr)                              // �� ��嵵 ���
				fprintf(fp, "NULL ");
			else                                   // ���� ���� ���
			{                                      // ���� �¿� child ���
				fprintf(fp, "%s (%d %s %s) ", printBit(ptr->data), ptr->bitNumber, printBit(ptr->leftChild->data), printBit(ptr->rightChild->data));

				if (ptr->leftChild->bitNumber > ptr->bitNumber) {    // ���� �ڽ��� ������ �� ��� add queue
					addQ(ptr->leftChild);
					exist = 1;
				}
				else addQ(NULL);                                     // ���� �ڽ��� ���� �����ų� ���� �� ��� NULL�� add
				if (ptr->rightChild->bitNumber > ptr->bitNumber) {   // ������ �ڽ��� ������ �� ��� add queue
					addQ(ptr->rightChild);
					exist = 1;
				}
				else addQ(NULL);                                     // ������ �ڽ��� ���� �����ų� ���� �� ��� NULL�� add
				num_child += 2;
			}

			count++;
			if (count == num) {                                      // ������ ����� ��� �ٹٲ� ���� ���
				fprintf(fp, "\n");
				num = num_child;                                     // null�� ���� �� ������ ������ 2�� n���� �ƴϹǷ� ������ ���� üũ
				num_child = 0;
				count = 0;
				if (!exist)
					break;
				exist = 0;
			}
		}
		while (front)                                                // queue free�� ���� ȣ��
			deleteQ();
		fprintf(fp, "\n");                                           // Ʈ�� ������ ���� �ٹٲ� ����
	}
}

char *printBit(int data) {                         // �������� �Է¹޾Ƽ� ��Ʈ ��� �� string���� ��ȯ�Ͽ� ��ȯ
	char *str;
	int a = 1, i;

	MALLOC(str, sizeof(*str)*(bit_len+1));         // ������ '\0'�� ���� +1 ����

	for (i = 1; i < bit_len; i++)                  // ��Ʈ �ֻ����� ũ�⸦ ������
		a *= 10;
	for (i = bit_len; i > 0; i--) {
		str[bit_len - i] = data / a + '0';         // ���ڷ� �ٲٱ� ���� + '0'
		data %= a;
		a /= 10;
	}
	str[bit_len] = 0;                              // ������ null���ڸ� ä����

	return str;                                    // bit�� ��ȯ�� string ��ȯ
}

void addQ(patricia ptr) {                    // level order queue add
	queue temp;

	MALLOC(temp, sizeof(*temp));             // �����Ҵ� �� �ʱ�ȭ
	temp->data = ptr;
	temp->next = NULL;
	
	if (!front) {                            // queue�� ����� ��� front�� rear �� �� �ʱ�ȭ
		front = temp;
		rear = temp;
	}
	else {                                   // queue�� ���� ���� ��� rear�� ����
		rear->next = temp;
		rear = rear->next;
	}
}

patricia deleteQ() {                         // level order queue delete
	patricia temp;
	queue avail;

	temp = front->data;                      // ��ȯ�� ���� temp�� ������ rear�� �Ű���
	avail = front;
	front = front->next;

	free(avail);                             // ��ȯ�� �� ���� ���� �ִ� queue�� �����Ҵ�����

	return temp;                             // �� ��ȯ
}