#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int imove[8] = { -1, -1, -1, 0, 1, 1, 1, 0 }, jmove[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };   // ���ʴ밢�� ���� 0�̰� �ð�������� direction ����
	int m, n, count, dir, x, y, i, j;
	int **arr;
	FILE *fp;
	
	if (argc < 3) {                                          // ���ڰ��� 3��(��������̸�)���� �ȵ����� ��� ����
		fprintf(stderr, "argument is missing!\n");
		exit(EXIT_FAILURE);
	}

	m = atoi(argv[1]);                                      // 2, 3��° ���ڷ� ������ m, n�� �����̹Ƿ� ���ڷ� ��ȯ
	n = atoi(argv[2]);

	if (m <= 0 || n <= 0) {                          // m, n�� 0 ������ ��� ����
		return 0;
	}

	if (!(fp = fopen(argv[3], "w"))) {                     // ����� ������ ������ ���� ��� ����
		fprintf(stderr, "fail to file open!\n");
		exit(EXIT_FAILURE);
	}

	count = m*n - 1;                                // ó�� ���� ���� ������ �迭 ��

	srand((long)time(NULL));                    // ���� �õ� ����

	x = m / 2;                             // ������ x = ���� �߰�
	y = n / 2;                             // ������ y = ���� �߰�

	arr = (int**)malloc(sizeof(*arr)*m);                      // �����Ҵ� �� �ʱ�ȭ
	for (i = 0; i < m; i++) {
		arr[i] = (int*)malloc(sizeof(*arr[i])*n);
		for (j = 0; j < n; j++)
			arr[i][j] = 0;
	}

	arr[x][y] = 1;                                          // ó�� �������� �� ���̹Ƿ� 1�� ����

	while (count > 0) {                                    // �Ȱ� ���� ���� ������ �ݺ�
		dir = rand() % 8;                                 // �� ���� �������� ����

		if (x + imove[dir] >= 0 && x + imove[dir] < m && y + jmove[dir] >= 0 && y + jmove[dir] < n) {             // ���� �������� �̵����� �� �迭�� ����� �� �˻�
			x += imove[dir];                 // �̵�
			y += jmove[dir];

			if (arr[x][y] == 0)             // ó�� ���� ���� ��� ���� ���� ����
				count--;

			arr[x][y]++;                // �湮 Ƚ�� ����
		}
	}
	
	fprintf(fp, "%d %d\n", x, y);                  // ���� ������ ���� ���
	for (i = 0; i < m; i++) {                   // �湮 Ƚ�� �迭 ��ü ���
		for (j = 0; j < n; j++) {
			fprintf(fp, "%4d ", arr[i][j]);
		}
		fprintf(fp, "\n");
	}

	return 0;
}