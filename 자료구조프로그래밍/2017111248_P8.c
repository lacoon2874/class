/*
	2017111248 황성재
	Program #8
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MALLOC(p,s)\
	if(!(p = malloc(s))){\
		fprintf(stderr, "insufficient memory\n");\
		exit(EXIT_FAILURE);\
	}

#define M 3
#define TRUE 1
#define FALSE 0

typedef struct node *nodePointer;
typedef struct node {
	int count;
	int key[M - 1];
	nodePointer child;
	nodePointer data[M - 1];
};
typedef struct stack *stackPointer;
typedef struct stack {
	int i;
	nodePointer data;
	stackPointer next;
};
typedef struct queue *queuePointer;
typedef struct queue {
	int level;
	nodePointer data;
	queuePointer next;
};

nodePointer root = NULL;
stackPointer top;
queuePointer front, rear;

int insertKey(int key);
int deleteKey(int key);
void printTree(FILE *fp);
void push(int i, nodePointer ptr);
stackPointer pop();
void addQ(int level, nodePointer ptr);
nodePointer deleteQ(int *level);

int main(int argc, char *argv[]) {
	FILE *ifp, *ofp;
	int n, key, i;

	if (argc < 3) {                                         // 인자가 부족하면 종료
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                     // 입력파일이 안열리면 종료
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                     // 출력파일이 안열리면 종료
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                                  // key 갯수
	for (i = 0; i < n; i++) {
		fscanf(ifp, "%d", &key);
		if (key < 0) {                                      // key가 음수면 삭제
			if (deleteKey(-key))
				printTree(ofp);
			else
				fprintf(ofp, "%d는 Tree에 존재하지 않습니다.\n\n", key);
		}
		else {                                              // key가 양수면 추가
			if (insertKey(key))
				printTree(ofp);
			else
				fprintf(ofp, "%d는 Tree에 이미 존재합니다.\n\n", key);
		}
	}

	return 0;
}

int insertKey(int key) {                                    // key 추가 함수
	nodePointer leaf, temp, avail, temp2;
	stackPointer ret_val;
	int i, j, k, end = FALSE, key_temp;

	if (!root) {                                            // 값이 하나도 없을 경우 tree 생성
		MALLOC(temp, sizeof(*temp));
		temp->count = 1;
		temp->key[0] = key;
		MALLOC(temp2, sizeof(*temp2));                      // external node
		temp2->count = 0;
		temp->child = temp2;
		MALLOC(temp2, sizeof(*temp2));                      // external node
		temp2->count = 0;
		temp->data[0] = temp2;
		for (i = 1; i < M - 1; i++) {                       // 노드 연결 없음
			temp->data[i] = NULL;
		}
		root = temp;
	}
	else {                                                  // 트리가 있을 경우 leaf에 추가
		leaf = root;
		top = NULL;

		while (1) {                                         // 추가할 위치를 찾음
			avail = leaf->child;
			for (i = 0; i < M - 1; i++) {
				key_temp = leaf->key[i];
				temp = leaf->data[i];
				if (temp) {
					if (key == key_temp)
						return 0;
					else if (key < key_temp)
						break;
					avail = temp;
				}
				else break;
			}
			push(i, leaf);
			if (!(avail->count))
				break;
			leaf = avail;
		}
		MALLOC(temp, sizeof(*temp));
		temp->count = 0;
		while (!end) {                                       // 추가 및 분할
			if (top) {
				ret_val = pop();
				i = ret_val->i;
				leaf = ret_val->data;
				if (leaf->count < M - 1) {                    // 노드에 key 값을 추가할 자리가 있을 경우 추가 후 종료
					leaf->count++;
					for (j = M - 2; j > i; j--) {
						if (leaf->data[j - 1]); {
							leaf->data[j] = leaf->data[j - 1];
							leaf->key[j] = leaf->key[j - 1];
						}
					}
					leaf->data[i] = temp;
					leaf->key[i] = key;
					end = TRUE;
				}
				else {                                // 노드에 자리가 없을 경우 분할
					j = (M + 1) / 2 - 1;
					if (i == j) {                      // 추가할 노드가 올라가는 경우
						MALLOC(temp2, sizeof(*temp2));
						temp2->count = M / 2;
						temp2->child = temp;
						for (k = j; k < M - 1; k++) {
							temp2->data[k - j] = leaf->data[k];
							temp2->key[k - j] = leaf->key[k];
						}
						for (k = M - 1 - j; k < M - 1; k++)
							temp2->data[k] = NULL;
						temp = temp2;
						leaf->count = j;
						for (k = j; k < M - 1; k++)
							leaf->data[k] = NULL;
					}
					else if (i < j) {                     // 추가할 노드가 왼쪽의 서브트리가 되는 경우
						key_temp = leaf->key[j - 1];
						MALLOC(temp2, sizeof(*temp2));
						temp2->count = M / 2;
						temp2->child = leaf->data[j - 1];
						for (k = j; k < M - 1; k++) {
							temp2->data[k - j] = leaf->data[k];
							temp2->key[k - j] = leaf->key[k];
						}
						for (k = M - j - 1; k < M - 1; k++)
							temp2->data[k] = NULL;
						leaf->count = j;
						for (k = j - 1; k > i; k--) {
							leaf->data[k] = leaf->data[k - 1];
							leaf->key[k] = leaf->key[k - 1];
						}
						leaf->key[i] = key;
						leaf->data[i] = temp;
						for (k = j; k < M - 1; k++)
							leaf->data[k] = NULL;
						key = key_temp;
						temp = temp2;
					}
					else {                                     // 추가할 노드가 오른쪽 서브트리가 되는 경우
						key_temp = leaf->key[j];
						MALLOC(temp2, sizeof(*temp2));
						temp2->count = M / 2;
						temp2->child = leaf->data[j];
						for (k = j + 1; k < M; k++) {
							if (k == i) {
								temp2->key[k - j - 1] = key;
								temp2->data[k - j - 1] = temp;
							}
							else if (k > i) {
								temp2->key[k - j - 1] = leaf->key[k - 1];
								temp2->data[k - j - 1] = leaf->data[k - 1];
							}
							else {
								temp2->key[k - j - 1] = leaf->key[k];
								temp2->data[k - j - 1] = leaf->data[k];
							}
						}
						for (k = M - j - 1; k < M - 1; k++)
							temp2->data[k] = NULL;
						temp = temp2;
						leaf->count = j;
						for (k = j; k < M - 1; k++)
							leaf->data[k] = NULL;
						key = key_temp;
					}
				}
				free(ret_val);
			}
			else {                                    // root 분할일 경우 높이가 늘어남
				MALLOC(temp2, sizeof(*temp2));
				temp2->count = 1;
				temp2->child = root;
				temp2->data[0] = temp;
				for (k = 1; k < M - 1; k++)
					temp2->data[k] = NULL;
				temp2->key[0] = key;

				root = temp2;
				
				end = TRUE;
			}
		}
		while (top) {
			ret_val = pop();
			free(ret_val);
		}
	}

	return 1;
}

int deleteKey(int key) {                            // key 값 삭제
	nodePointer temp, avail, cur, leaf, parent;
	stackPointer ret_val;
	int i, cur_i, j, parent_i, k, end = FALSE;

	if (root) {                                         // 트리가 있을 경우 탐색 및 삭제
		cur = root;
		while (1) {                                    // 삭제할 key 위치 탐색
			avail = cur->child;
			for (i = 0; i < M - 1; i++) {
				temp = cur->data[i];
				if (temp) {
					if (key == cur->key[i])
						break;
					else if (key < cur->key[i])
						break;
					avail = temp;
				}
				else break;
			}
			if (key == cur->key[i]) {                    // key가 있으면 넘어가서 삭제
				push(i, cur);
				cur_i = i;
				break;
			}
			if (!(avail->count))              // leaf 노드까지 왔음에도 없을 경우 0 return으로 없다고 출력
				return 0;
			push(i - 1, cur);
			cur = avail;
		}

		if (avail->count) {                             // leaf가 아니었을 경우 key값을 leaf node에서 올려줌
			leaf = avail;
			while (1) {
				i = leaf->count - 1;
				avail = leaf->data[i];
				push(i, leaf);
				if (!(avail->count)) {
					cur->key[cur_i] = leaf->key[i];
					break;
				}
			}
		}

		if (!(top->next)) {                             // leaf노드가 root였을 경우 값 삭제 또는 트리 삭제
			root->count--;
			free(cur->data[cur_i]);
			for (i = cur_i + 1; i < M - 1; i++) {
				if (cur->data[i]) {
					cur->key[i - 1] = cur->key[i];
					cur->data[i - 1] = cur->data[i];
				}
				else break;
			}
			cur->data[i - 1] = NULL;
			if (!(root->count))
				root = NULL;
		}
		else {                                   // leaf노드 값 삭제 및 병합
			ret_val = pop();
			i = ret_val->i;
			leaf = ret_val->data;

			leaf->count--;
			free(leaf->data[i]);
			for (j = i + 1; j < M - 1; j++) {               // leaf 노드에서 값 삭제 후 자리 조정
				if (leaf->data[j]) {
					leaf->key[j - 1] = leaf->key[j];
					leaf->data[j - 1] = leaf->data[j];
				}
				else break;
			}
			leaf->data[j - 1] = NULL;

			free(ret_val);

			j = (M + 1) / 2 - 1;
			if (leaf->count < j) {                            // leaf노드의 값 갯수가 부족할 경우 회전 또는 합병
				while (!end && top && root->count > 0) {
					ret_val = pop();
					parent = ret_val->data;
					parent_i = ret_val->i;

					if (parent_i == -1) {
						if (parent->data[0]->count > j) {                   // 왼쪽 회전
							cur = parent->data[0];

							leaf->key[leaf->count] = parent->key[0];
							leaf->data[leaf->count] = cur->child;

							parent->key[0] = cur->key[0];

							cur->child = cur->data[0];

							for (k = 0; k < cur->count - 1; k++) {
								cur->key[k] = cur->key[k + 1];
								cur->data[k] = cur->data[k + 1];
							}
							cur->data[k] = NULL;

							cur->count--;
							leaf->count++;

							end = TRUE;
						}
						else {                                       // 오른쪽 결합
							cur = parent->data[0];

							leaf->key[leaf->count] = parent->key[0];
							leaf->data[leaf->count++] = cur->child;

							for (k = 0; k < cur->count; k++) {
								leaf->key[leaf->count] = cur->key[k];
								leaf->data[leaf->count++] = cur->data[k];
							}

							free(cur);

							for (k = 0; k < parent->count - 1; k++) {
								parent->key[k] = parent->key[k + 1];
								parent->data[k] = parent->data[k + 1];
							}
							parent->data[k] = NULL;

							parent->count--;
							if (parent->count < j)
								leaf = parent;
							else
								end = TRUE;
						}
					}
					else if (parent_i == 0) {
						if (parent->child->count > j) {                 // 오른쪽 회전
							for (k = leaf->count - 1; k >= 0; k--) {
								leaf->key[k + 1] = leaf->key[k];
								leaf->data[k + 1] = leaf->data[k];
							}
							leaf->data[0] = leaf->child;

							cur = parent->data[0];
							leaf->child = cur->data[cur->count - 1];
							leaf->key[0] = parent->key[0];
							parent->key[0] = cur->key[cur->count - 1];

							cur->data[cur->count - 1] = NULL;
							cur->count--;
							leaf->count++;

							end = TRUE;
						}
						else if (parent->data[1]) {
							if (parent->data[1]->count > j) {                 // 왼쪽 회전
								cur = parent->data[1];

								leaf->key[leaf->count] = parent->key[1];
								leaf->data[leaf->count] = cur->child;

								parent->key[1] = cur->key[0];

								cur->child = cur->data[0];

								for (k = 0; k < cur->count - 1; k++) {
									cur->key[k] = cur->key[k + 1];
									cur->data[k] = cur->data[k + 1];
								}
								cur->data[k] = NULL;

								cur->count--;
								leaf->count++;

								end = TRUE;
							}
							else {                                         // 오른쪽 결합
								cur = parent->data[1];

								leaf->key[leaf->count] = parent->key[1];
								leaf->data[leaf->count++] = cur->child;

								for (k = 0; k < cur->count; k++) {
									leaf->key[leaf->count] = cur->key[k];
									leaf->data[leaf->count++] = cur->data[k];
								}

								free(cur);

								for (k = 1; k < parent->count - 1; k++) {
									parent->key[k] = parent->key[k + 1];
									parent->data[k] = parent->data[k + 1];
								}
								parent->data[k] = NULL;

								parent->count--;
								if (parent->count < j)
									leaf = parent;
								else
									end = TRUE;
							}
						}
						else {                                           // 왼쪽 결합
							cur = parent->child;

							cur->key[cur->count] = parent->key[0];
							cur->data[cur->count++] = leaf->child;

							for (k = 0; k < leaf->count; k++) {
								cur->key[cur->count] = leaf->key[k];
								cur->data[cur->count++] = leaf->data[k];
							}

							free(leaf);

							for (k = 0; k < parent->count - 1; k++) {
								parent->key[k] = parent->key[k + 1];
								parent->data[k] = parent->data[k + 1];
							}
							parent->data[k] = NULL;

							parent->count--;
							if (parent->count < j)
								leaf = parent;
							else
								end = TRUE;
						}
					}
					else {
						if (parent->data[parent_i - 1]->count > j) {            // 오른쪽 회전
							for (k = leaf->count - 1; k >= 0; k--) {
								leaf->key[k + 1] = leaf->key[k];
								leaf->data[k + 1] = leaf->data[k];
							}
							leaf->data[0] = leaf->child;

							cur = parent->data[parent_i - 1];
							leaf->child = cur->data[cur->count - 1];
							leaf->key[0] = parent->key[parent_i];
							parent->key[parent_i] = cur->key[cur->count - 1];

							cur->data[cur->count - 1] = NULL;
							cur->count--;
							leaf->count++;

							end = TRUE;
						}
						else if (parent->count > parent_i + 1 && parent->data[parent_i + 1]) {
							if (parent->data[parent_i + 1]->count > j) {                    // 왼쪽 회전
								cur = parent->data[parent_i + 1];

								leaf->key[leaf->count] = parent->key[parent_i + 1];
								leaf->data[leaf->count] = cur->child;

								parent->key[parent_i + 1] = cur->key[0];

								cur->child = cur->data[0];

								for (k = 0; k < cur->count - 1; k++) {
									cur->key[k] = cur->key[k + 1];
									cur->data[k] = cur->data[k + 1];
								}
								cur->data[k] = NULL;

								cur->count--;
								leaf->count++;

								end = TRUE;
							}
							else {                                           // 오른쪽 결합
								cur = parent->data[parent_i + 1];

								leaf->key[leaf->count] = parent->key[parent_i + 1];
								leaf->data[leaf->count++] = cur->child;

								for (k = 0; k < cur->count; k++) {
									leaf->key[leaf->count] = cur->key[k];
									leaf->data[leaf->count++] = cur->data[k];
								}

								free(cur);

								for (k = parent_i + 1; k < parent->count - 1; k++) {
									parent->key[k] = parent->key[k + 1];
									parent->data[k] = parent->data[k + 1];
								}
								parent->data[k] = NULL;

								parent->count--;
								if (parent->count < j)
									leaf = parent;
								else
									end = TRUE;
							}
						}
						else {                                       // 왼쪽 결합
							cur = parent->data[parent_i - 1];

							cur->key[cur->count] = parent->key[parent_i];
							cur->data[cur->count++] = leaf->child;

							for (k = 0; k < leaf->count; k++) {
								cur->key[cur->count] = leaf->key[k];
								cur->data[cur->count++] = leaf->data[k];
							}

							free(leaf);

							for (k = parent_i; k < parent->count - 1; k++) {
								parent->key[k] = parent->key[k + 1];
								parent->data[k] = parent->data[k + 1];
							}
							parent->data[k] = NULL;

							parent->count--;
							if (parent->count < j)
								leaf = parent;
							else
								end = TRUE;
						}
					}
				}
				if (!end) {
					if (!(root->count)) {
						temp = root;
						root = root->child;
						free(temp);
					}
				}
				while (top) {
					ret_val = pop();
					free(ret_val);
				}
			}
		}

		return 1;
	}
	return 0;
}

void printTree(FILE *fp) {                // Tree level order로 출력
	nodePointer ptr = root;
	int i, avail = 1, level = 1;

	front = NULL;
	rear = NULL;
	
	if (root) {
		addQ(level, ptr);
		fprintf(fp, "root : ");
		while (front) {
			ptr = deleteQ(&level);
			if (level != avail)
				fprintf(fp, "\n");
			avail = level;
			fprintf(fp, "N ");

			if (ptr->child->count)
				addQ(level + 1, ptr->child);
			for (i = 0; i < M - 1; i++) {
				if (ptr->data[i]) {
					fprintf(fp, "%d ", ptr->key[i]);
					if (ptr->data[i]->count)
						addQ(level + 1, ptr->data[i]);
				}
				else break;
			}
		}

		fprintf(fp, "\n\n");
	}
}

void push(int i, nodePointer ptr) {           // push stack
	stackPointer temp;

	MALLOC(temp, sizeof(*temp));
	temp->i = i;
	temp->data = ptr;

	if (!top) {
		temp->next = NULL;
		top = temp;
	}
	else {
		temp->next = top;
		top = temp;
	}
}

stackPointer pop() {                         // pop stack
	stackPointer temp;

	temp = top;

	top = top->next;

	return temp;
}

void addQ(int level, nodePointer ptr) {                         // add queue
	queuePointer temp;

	MALLOC(temp, sizeof(*temp));
	temp->data = ptr;
	temp->next = NULL;
	temp->level = level;

	if (!front) {
		front = temp;
		rear = temp;
	}
	else {
		rear->next = temp;
		rear = temp;
	}
}

nodePointer deleteQ(int *level) {                           // delete queue
	nodePointer temp;
	queuePointer ptr;

	temp = front->data;
	*level = front->level;

	ptr = front;
	front = front->next;
	free(ptr);

	return temp;
}