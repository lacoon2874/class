/*
2017111248 황성재
Program #2
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//동적 할당
#define MALLOC(p, s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

typedef struct {                       // stack 구조체
	int val;
}term;

term *stack;
int top = -1;

void push(int val);
int pop();

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;                       // input, output file pointer
	int *arr, *span1, *span2;                     // 입력 값과 1, 2번 방법에 의한 span 수
	int n, i, j, span, count;

	if (argc < 3) {                                     // output filename 까지 들어오지 않으면 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                 // input file이 열리지 않으면 종료
		fprintf(stderr, "fail to file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                // output file이 열리지 않으면 종료
		fprintf(stderr, "fail to file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                         // day 수 입력
	MALLOC(arr, sizeof(int)*n);                 // 사용 배열 및 스택 동적할당
	MALLOC(span1, sizeof(int)*n);
	MALLOC(span2, sizeof(int)*n);
	MALLOC(stack, sizeof(*stack)*n);

	for (i = 0; i < n; i++) {                       // day 수 만큼 배열 입력
		fscanf(ifp, "%d", &arr[i]);
	}

	count = 0;                               // 방법 1
	for (i = 0; i < n; i++) {
		span = 1;
		for (j = i - 1; j > -1; j--) {               // arr index 비교할 값 앞에서 0까지
			count++;                                        // 비교 횟수 체크
			if (arr[j] > arr[i])                         // 비교
				break;
			span++;
		}
		span1[i] = span;                            // span 값 저장
	}
	
	fprintf(ofp, "%d\n", count);                       // 1번 방법의 비교횟수
	for (i = 0; i < n; i++)                              // 1번 방법의 span 수
		fprintf(ofp, "%d ", span1[i]);
	fprintf(ofp, "\n");

	count = 0;                                         // 방법2
	for (i = 0; i < n; i++) {
		while (top > -1) {                              // stack empty 까지
			count++;                                     // 비교 횟수 체크
			if (arr[stack[top].val] > arr[i]) {            // 비교
				break;
			}
			pop();
		}
		if (top == -1)                                  // stack empty 일 경우 index - (-1)
			span2[i] = i + 1;
		else span2[i] = i - stack[top].val;             // span = index - stack value
		push(i);
	}

	fprintf(ofp, "%d\n", count);                      // 2번 방법의 비교횟수
	for (i = 0; i < n; i++)                               // 2번 방법의 span 수
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