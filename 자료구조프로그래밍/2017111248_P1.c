#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int imove[8] = { -1, -1, -1, 0, 1, 1, 1, 0 }, jmove[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };   // 왼쪽대각선 위가 0이고 시계방향으로 direction 증가
	int m, n, count, dir, x, y, i, j;
	int **arr;
	FILE *fp;
	
	if (argc < 3) {                                          // 인자값이 3개(출력파일이름)까지 안들어왔을 경우 종료
		fprintf(stderr, "argument is missing!\n");
		exit(EXIT_FAILURE);
	}

	m = atoi(argv[1]);                                      // 2, 3번째 인자로 들어오는 m, n이 문자이므로 숫자로 변환
	n = atoi(argv[2]);

	if (m <= 0 || n <= 0) {                          // m, n이 0 이하일 경우 종료
		return 0;
	}

	if (!(fp = fopen(argv[3], "w"))) {                     // 출력할 파일이 열리지 않을 경우 종료
		fprintf(stderr, "fail to file open!\n");
		exit(EXIT_FAILURE);
	}

	count = m*n - 1;                                // 처음 시작 점을 제외한 배열 수

	srand((long)time(NULL));                    // 랜덤 시드 변경

	x = m / 2;                             // 시작점 x = 행의 중간
	y = n / 2;                             // 시작점 y = 열의 중간

	arr = (int**)malloc(sizeof(*arr)*m);                      // 동적할당 및 초기화
	for (i = 0; i < m; i++) {
		arr[i] = (int*)malloc(sizeof(*arr[i])*n);
		for (j = 0; j < n; j++)
			arr[i][j] = 0;
	}

	arr[x][y] = 1;                                          // 처음 시작점은 간 곳이므로 1로 저장

	while (count > 0) {                                    // 안간 곳이 없을 때까지 반복
		dir = rand() % 8;                                 // 갈 방향 랜덤으로 얻어옴

		if (x + imove[dir] >= 0 && x + imove[dir] < m && y + jmove[dir] >= 0 && y + jmove[dir] < n) {             // 랜덤 방향으로 이동했을 시 배열을 벗어나는 지 검사
			x += imove[dir];                 // 이동
			y += jmove[dir];

			if (arr[x][y] == 0)             // 처음 가는 곳일 경우 남은 갯수 감소
				count--;

			arr[x][y]++;                // 방문 횟수 증가
		}
	}
	
	fprintf(fp, "%d %d\n", x, y);                  // 최종 도착지 파일 출력
	for (i = 0; i < m; i++) {                   // 방문 횟수 배열 전체 출력
		for (j = 0; j < n; j++) {
			fprintf(fp, "%4d ", arr[i][j]);
		}
		fprintf(fp, "\n");
	}

	return 0;
}