/*
	2017111248 Ȳ����
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

typedef struct node *nodePointer;                  // tree node ����ü
typedef struct node {
	int degree, data;
	nodePointer child, link;
};

typedef struct queue *queuePointer;                // level order queue ����ü
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

int main(int argc, char *argv[]) {                         // main �Լ�
	FILE *ifp, *ofp;
	int size, i, n, key;

	if (argc < 3) {                                        // ���ڰ� ���� ��� ����
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

	fscanf(ifp, "%d", &size);                              // size �Է� �� 1/2�� ���
	n = size / 2;

	for (i = 0; i < n; i++) {                              // 1/2�� ���� �Է� �� insert
		fscanf(ifp, "%d", &key);

		insert(key);
	}
	fprintf(ofp, "1/2 �Է� ���� ��\n");                    // ���� �� ���
	printAll(ofp);
	fprintf(ofp, "\n1/2 �Է� ���� ��\n");                  // ���� �� ���� �� ���
	deletion();
	printAll(ofp);

	for (; i < size; i++) {                                // 1/2�� ���� �Է� �� insert
		fscanf(ifp, "%d", &key);

		insert(key);
	}
	fprintf(ofp, "\n��ü �Է� ���� ��\n");                 // ���� �� ���
	printAll(ofp);
	fprintf(ofp, "\n��ü �Է� ���� ��\n");                 // ���� �� ���� �� ���
	deletion();
	printAll(ofp);

	return 0;
}

void insert(int key) {                                     // ���� �Լ�
	nodePointer temp;

	MALLOC(temp, sizeof(*temp));

	temp->degree = 0;                                      // data�� �� �ְ� �ʱ�ȭ
	temp->child = NULL;
	temp->data = key;

	if (!min) {                                            // heap�� ������� ��� �ڱ� �ڽ� link
		temp->link = temp;
		min = temp;
	}
	else {                                                 // heap�� tree�� ���� ���
		temp->link = min->link;                            // min�� �ٷ� �ڿ� link ����
		min->link = temp;

		if (temp->data < min->data)                        // ���� ���� ���� ���� ���� ��� min ����
			min = temp;

		meld();                                            // Ʈ�� �պ�
	}
}

int deletion() {                                           // �� ���� �� ��ȯ
	nodePointer a, b, ptr, a_trail, b_trail;
	int min_value, x;

	if (min) {                                             // heap�� ���� ���� ���
		x = min->data;                                     // ��ȯ�� �� ����

		if (min->link != min) {                            // min�� ���Ե� Ʈ�� ���� �ٸ� Ʈ���� ������ ���
			min_value = min->link->data;                   // �ٸ� Ʈ������ ���� ���� ��(min) ã��
			ptr = min->link;
			a_trail = min;

			for (a = min->link; a != min; a = a->link) {
				if (a->data < min_value) {
					min_value = a->data;
					ptr = a;
				}

				a_trail = a;
			}

			if (min->child) {                              // min�� child�� ������ ��� chlid������ �߰��� ã��
				b = min->child;
				do {
					if (b->data < min_value) {
						min_value = b->data;
						ptr = b;
					}

					b_trail = b;
					b = b->link;
				} while (b != min->child);

				a_trail->link = min->child;                // min�� child�� link�� heap���� circular link�� ����
				b_trail->link = min->link;
			}
			else a_trail->link = min->link;                // min�� link ������

			free(min);

			min = ptr;                                     // ã�� ���� ���� ��Ʈ�� min���� ����

			meld();                                        // ���� �� �պ�
		}
		else if (min->child) {                             // min���� child�� �־��� ���
			min_value = min->child->data;                  // ���� ���� �� ã��
			ptr = min->child;
			a = min->child;

			do {
				if (a->data < min_value) {
					min_value = a->data;
					ptr = a;
				}

				a = a->link;
			} while (a != min->child);

			free(min);                                     // ����

			min = ptr;                                     // min ����
		}
		else min = NULL;                                   // heap�� min�� ��Ʈ��� �ϳ��� �������� ��� min�� NULL

		return x;
	}

	return -1;
}

void meld() {                                              // �պ�
	nodePointer ptr = min, a, b, a_trail, b_trail, temp = NULL;
	int max = -1, degree = 0, join_fail;

	do {                                                   // �պ��� ���� ��ũ ������ ��ġ�� ã�� ���� Ž��
		if (ptr->degree > max)
			max = ptr->degree;

		if (ptr->link == min)
			temp = ptr;

		ptr = ptr->link;

	} while (ptr != min);

	while (degree <= max) {                                // degree�� ���� ū ���� �� ������ �պ�
		join_fail = 1;                                     // �պ��� �����ߴ��� �˻�
		ptr = min;
		a = NULL;
		a_trail = ptr;
		b_trail = min;

		do {
			if (ptr->degree == degree) {                   // ���� ���� degree���� Ž��
				if (!a)                                    // ù��° �պ��� Ʈ��
					a = ptr;
				else {
					b = ptr;                               // �ι�° �պ��� Ʈ��

					if (a->data < b->data) {               // a�� ��Ʈ��尡 �� ������ b�� a�� child�� ����
						b_trail->link = b->link;

						if (b == temp)                     // ��ũ �������� ���� temp���� ����� ����̹Ƿ� temp���� temp���� ���� ����
							b_trail = temp;

						ptr = join(a, b);
					}
					else {                                 // b�� ��Ʈ��尡 �� ������ a�� b�� child�� ����
						a_trail->link = a->link;
						ptr = join(b, a);
					}

					join_fail = 0;                         // �պ��� ����̹Ƿ� ǥ��

					if (ptr->degree > max)                 // �պ��� ���� max degree�� �ٲ� ��� ����
						max = ptr->degree;

					break;                                 // break�� ó������ �ٽ� Ž��
				}
			}
			
			if (!a)
				a_trail = ptr;
			else b_trail = ptr;
			ptr = ptr->link;
		} while (ptr != min);

		if (join_fail)                                     // �պ��� ������ ��� ����(����) degree Ž��
			degree++;
	}
}

nodePointer join(nodePointer a, nodePointer b) {           // Ʈ�� ��ħ
	if (!(a->child)) {                                     // a���� child�� ������ ��� b�� �ڰ� ��ȯ
		b->link = b;
	}
	else {                                                 // a���� child�� �־��� ��� link�� child ����
		b->link = a->child->link;
		a->child->link = b;
	}
	a->child = b;                                          // a�� child b�� ����
	(a->degree)++;                                         // a�� child�� �þ����Ƿ� degree ����

	return a;
}

void printAll(FILE *fp) {                                  // heap ���
	nodePointer ptr, start, root = min;

	if (min) {                                             // heap�� ���� ������ ���
		do {
			fprintf(fp, "degree %d : ", root->degree);     // Ʈ���� degree ���

			addQ(root);                                    // queue�� ���� level order ���

			while (front) {
				ptr = deleteQ();
				fprintf(fp, "%d ", ptr->data);

				ptr = ptr->child;

				if (ptr) {                                 // child�� �־��� ��� queue�� ����
					start = ptr;

					do {
						addQ(ptr);

						ptr = ptr->link;
					} while (ptr != start);
				}
			}

			fprintf(fp, "\n");

			root = root->link;                             // heap�� ���� Ʈ�� Ž�� �� ���
		} while (root != min);
	}
}

void addQ(nodePointer key) {                               // add queue
	queuePointer temp;

	MALLOC(temp, sizeof(*temp));
	
	temp->data = key;                                      // �� �ʱ�ȭ
	temp->next = NULL;

	if (!front) {                                          // queue�� ����־��� ���
		front = temp;
		rear = temp;
	}
	else {                                                 // queue�� ���� �־��� ��� rear�� ����
		rear->next = temp;
		rear = temp;
	}
}

nodePointer deleteQ() {                                    // delete queue
	queuePointer ptr;
	nodePointer key;

	key = front->data;                                     // ��ȯ�� �� ����
	ptr = front;
	
	front = front->next;

	free(ptr);                                             // ������� ����(����)

	return key;                                            // �� ��ȯ
}