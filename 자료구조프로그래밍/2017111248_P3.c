/*
	2017111248 Ȳ����
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

	if (argc < 3) {                                            // output file name ���� ������ �ʾ��� ��� exit
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                            // �Է� ������ ������ ���� ��� exit
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                             // ��� ������ ������ ���� ��� exit
		fprintf(stderr, "fail to output file open\n");
	}
	
	fscanf(ifp, "%d", &n);                                        // key ���� �Է� ���� �� �迭 ����
	MALLOC(heap, sizeof(*heap)*(n + 1));

	print1 = n / 3;                               // 1/3����
	print2 = n * 2 / 3;                           // 2/3����

	for (i = 1; i <= n; i++) {
		fscanf(ifp, "%d", &item.key);               // Ű �� �Է�

		push(item, &count);                         // min heap�� push

		if (i == print1)                        // 1/3 ���� ���
			printHeap(print1, ofp);
		else if (i == print2)                   // 2/3 ���� ���
			printHeap(print2, ofp);
	}
	printHeap(count, ofp);                       // �� �Է� ���� �� ���

	for (i = 0; i < 3; i++) {
		pop(&count);                        // ��Ʈ ��� ����
		printHeap(count, ofp);              // ���� �� ���
	}

	free(heap);
	fclose(ifp);
	fclose(ofp);

	return 0;
}

void push(element item, int *n) {
	int i;

	i = ++(*n);                                                 // ��� �� ����
	while ((i != 1) && (item.key < heap[i / 2].key)) {               // ��Ʈ���� ���ų� �θ� ��庸�� ���� ����
		heap[i] = heap[i / 2];                           // �θ��带 ���� �ڽ��� ��ġ�� �̵�
		i /= 2;                                      // �ڽ��� ��ġ�� �θ����� ��ġ�� �̵�
	}
	heap[i] = item;                       // ��Ʈ �Ǵ� �θ��� ���� Ŭ ��� �� �ڸ��� ����
}

element pop(int *n) {
	int parent, child;
	element item, temp;

	if (*n == 0) {                                    // heap �� ����� ��� exit
		fprintf(stderr, "heap is empty\n");
		exit(EXIT_FAILURE);
	}

	item = heap[1];                    // ������ ���(��Ʈ ���)
	temp = heap[(*n)--];               // ���� ������ ���
	parent = 1;
	child = 2;
	
	while (child <= *n) {                                                // child�� �ִ� ���� �˻�
		if ((child < *n) && (heap[child].key > heap[child + 1].key))     // child�� ���̸鼭 ������ child�� �� ���� ���
			child++;                                                     // ������ child�� �̵�
		if (heap[child].key >= temp.key) break;                          // child ���� ���� ��� �� �ڸ��� ����
		heap[parent] = heap[child];                                      // child ���� Ŭ ��� child�� ���� �ڽ��� �ڸ��� �̵�
		parent = child;                                                  // �ڽ��� �ڸ��� child �ڸ��� �̵�
		child *= 2;                                                      // ���� �˻��� ���� ã�� ���� child 2��
	}
	heap[parent] = temp;                // ������ �ڸ��� ����
	return item;                        // ������ ��� ��ȯ
}

void printHeap(int n, FILE *fp) {
	int i, count = 2;

	for (i = 1; i <= n; i++) {
		if (i == count) {                    // ���ο� level�� ���� ��� �ٹٲ�
			fprintf(fp, "\n");
			count *= 2;                      // ���� �� ��� ���� 2���̹Ƿ� 2�� ������
		}
		fprintf(fp, "%d ", heap[i].key);     // ���
	}
	fprintf(fp, "\n");                       // heap ��� �� �ٸ� heap���� ������ ���� �ٹٲ�
	fprintf(fp, "\n");
}