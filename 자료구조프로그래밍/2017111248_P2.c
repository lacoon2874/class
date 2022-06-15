/*
2017111248 Ȳ����
Program #2
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//���� �Ҵ�
#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct {                       // stack ����ü
	int val;
}term;

term *stack;
int top = -1;

void push(int val);
int pop();

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;                       // input, output file pointer
	int *arr, *span1, *span2;                     // �Է� ���� 1, 2�� ����� ���� span ��
	int n, i, j, span, count;

	if (argc < 3) {                                     // output filename ���� ������ ������ ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                 // input file�� ������ ������ ����
		fprintf(stderr, "fail to file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                // output file�� ������ ������ ����
		fprintf(stderr, "fail to file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                         // day �� �Է�
	MALLOC(arr, sizeof(int)*n);                 // ��� �迭 �� ���� �����Ҵ�
	MALLOC(span1, sizeof(int)*n);
	MALLOC(span2, sizeof(int)*n);
	MALLOC(stack, sizeof(*stack)*n);

	for (i = 0; i < n; i++) {                       // day �� ��ŭ �迭 �Է�
		fscanf(ifp, "%d", &arr[i]);
	}

	count = 0;                               // ��� 1
	for (i = 0; i < n; i++) {
		span = 1;
		for (j = i - 1; j > -1; j--) {               // arr index ���� �� �տ��� 0����
			count++;                                        // �� Ƚ�� üũ
			if (arr[j] > arr[i])                         // ��
				break;
			span++;
		}
		span1[i] = span;                            // span �� ����
	}
	
	fprintf(ofp, "%d\n", count);                       // 1�� ����� ��Ƚ��
	for (i = 0; i < n; i++)                              // 1�� ����� span ��
		fprintf(ofp, "%d ", span1[i]);
	fprintf(ofp, "\n");

	count = 0;                                         // ���2
	for (i = 0; i < n; i++) {
		while (top > -1) {                              // stack empty ����
			count++;                                     // �� Ƚ�� üũ
			if (arr[stack[top].val] > arr[i]) {            // ��
				break;
			}
			pop();
		}
		if (top == -1)                                  // stack empty �� ��� index - (-1)
			span2[i] = i + 1;
		else span2[i] = i - stack[top].val;             // span = index - stack value
		push(i);
	}

	fprintf(ofp, "%d\n", count);                      // 2�� ����� ��Ƚ��
	for (i = 0; i < n; i++)                               // 2�� ����� span ��
		fprintf(ofp, "%d ", span2[i]);
	fprintf(ofp, "\n");

	return 0;
}

void push(int val) {                            // stack push (index)
	stack[++top].val = val;
}

int pop() {                               // stack value pop
	return stack[top--].val;
}