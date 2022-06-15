/*
	2017111248 Ȳ����
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

	if (argc < 3) {                                         // ���ڰ� �����ϸ� ����
		fprintf(stderr, "argument is missing\n");
		exit(EXIT_FAILURE);
	}

	if (!(ifp = fopen(argv[1], "r"))) {                     // �Է������� �ȿ����� ����
		fprintf(stderr, "fail to input file open\n");
		exit(EXIT_FAILURE);
	}

	if (!(ofp = fopen(argv[2], "w"))) {                     // ��������� �ȿ����� ����
		fprintf(stderr, "fail to output file open\n");
		exit(EXIT_FAILURE);
	}

	fscanf(ifp, "%d", &n);                                  // key ����
	for (i = 0; i < n; i++) {
		fscanf(ifp, "%d", &key);
		if (key < 0) {                                      // key�� ������ ����
			if (deleteKey(-key))
				printTree(ofp);
			else
				fprintf(ofp, "%d�� Tree�� �������� �ʽ��ϴ�.\n\n", key);
		}
		else {                                              // key�� ����� �߰�
			if (insertKey(key))
				printTree(ofp);
			else
				fprintf(ofp, "%d�� Tree�� �̹� �����մϴ�.\n\n", key);
		}
	}

	return 0;
}

int insertKey(int key) {                                    // key �߰� �Լ�
	nodePointer leaf, temp, avail, temp2;
	stackPointer ret_val;
	int i, j, k, end = FALSE, key_temp;

	if (!root) {                                            // ���� �ϳ��� ���� ��� tree ����
		MALLOC(temp, sizeof(*temp));
		temp->count = 1;
		temp->key[0] = key;
		MALLOC(temp2, sizeof(*temp2));                      // external node
		temp2->count = 0;
		temp->child = temp2;
		MALLOC(temp2, sizeof(*temp2));                      // external node
		temp2->count = 0;
		temp->data[0] = temp2;
		for (i = 1; i < M - 1; i++) {                       // ��� ���� ����
			temp->data[i] = NULL;
		}
		root = temp;
	}
	else {                                                  // Ʈ���� ���� ��� leaf�� �߰�
		leaf = root;
		top = NULL;

		while (1) {                                         // �߰��� ��ġ�� ã��
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
		while (!end) {                                       // �߰� �� ����
			if (top) {
				ret_val = pop();
				i = ret_val->i;
				leaf = ret_val->data;
				if (leaf->count < M - 1) {                    // ��忡 key ���� �߰��� �ڸ��� ���� ��� �߰� �� ����
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
				else {                                // ��忡 �ڸ��� ���� ��� ����
					j = (M + 1) / 2 - 1;
					if (i == j) {                      // �߰��� ��尡 �ö󰡴� ���
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
					else if (i < j) {                     // �߰��� ��尡 ������ ����Ʈ���� �Ǵ� ���
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
					else {                                     // �߰��� ��尡 ������ ����Ʈ���� �Ǵ� ���
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
			else {                                    // root ������ ��� ���̰� �þ
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

int deleteKey(int key) {                            // key �� ����
	nodePointer temp, avail, cur, leaf, parent;
	stackPointer ret_val;
	int i, cur_i, j, parent_i, k, end = FALSE;

	if (root) {                                         // Ʈ���� ���� ��� Ž�� �� ����
		cur = root;
		while (1) {                                    // ������ key ��ġ Ž��
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
			if (key == cur->key[i]) {                    // key�� ������ �Ѿ�� ����
				push(i, cur);
				cur_i = i;
				break;
			}
			if (!(avail->count))              // leaf ������ �������� ���� ��� 0 return���� ���ٰ� ���
				return 0;
			push(i - 1, cur);
			cur = avail;
		}

		if (avail->count) {                             // leaf�� �ƴϾ��� ��� key���� leaf node���� �÷���
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

		if (!(top->next)) {                             // leaf��尡 root���� ��� �� ���� �Ǵ� Ʈ�� ����
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
		else {                                   // leaf��� �� ���� �� ����
			ret_val = pop();
			i = ret_val->i;
			leaf = ret_val->data;

			leaf->count--;
			free(leaf->data[i]);
			for (j = i + 1; j < M - 1; j++) {               // leaf ��忡�� �� ���� �� �ڸ� ����
				if (leaf->data[j]) {
					leaf->key[j - 1] = leaf->key[j];
					leaf->data[j - 1] = leaf->data[j];
				}
				else break;
			}
			leaf->data[j - 1] = NULL;

			free(ret_val);

			j = (M + 1) / 2 - 1;
			if (leaf->count < j) {                            // leaf����� �� ������ ������ ��� ȸ�� �Ǵ� �պ�
				while (!end && top && root->count > 0) {
					ret_val = pop();
					parent = ret_val->data;
					parent_i = ret_val->i;

					if (parent_i == -1) {
						if (parent->data[0]->count > j) {                   // ���� ȸ��
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
						else {                                       // ������ ����
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
						if (parent->child->count > j) {                 // ������ ȸ��
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
							if (parent->data[1]->count > j) {                 // ���� ȸ��
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
							else {                                         // ������ ����
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
						else {                                           // ���� ����
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
						if (parent->data[parent_i - 1]->count > j) {            // ������ ȸ��
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
							if (parent->data[parent_i + 1]->count > j) {                    // ���� ȸ��
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
							else {                                           // ������ ����
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
						else {                                       // ���� ����
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

void printTree(FILE *fp) {                // Tree level order�� ���
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