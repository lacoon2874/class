#include <stdio.h>
#include <stdlib.h>

int *profit, *weight, *include, *bestset;
int n, w, maxprofit, numbest;

void knapsack(int index, int curprofit, int curweight);
int promising(int i, int curweight, int curprofit);

int main(void) {
	FILE *ifp, *ofp;
	char inFileName[100];
	int i;

	maxprofit = 0;
	numbest = 0;

	printf_s("input file name?");
	scanf_s("%s", inFileName, sizeof(inFileName));
	fopen_s(&ifp, inFileName, "r");
	fopen_s(&ofp, "output.txt", "w");
	
	fscanf_s(ifp, "%d %d", &n, &w);
	profit = (int*)malloc(sizeof(*profit)*(n + 1));
	weight = (int*)malloc(sizeof(*weight)*(n + 1));
	include = (int*)malloc(sizeof(*include)*(n + 1));
	bestset = (int*)malloc(sizeof(*bestset)*(n + 1));

	profit[0] = 0;
	weight[0] = 0;
	for (i = 1; i <= n; i++) {
		fscanf_s(ifp, "%d %d", &profit[i], &weight[i]);
		include[i] = 0;
	}

	knapsack(0, 0, 0);

	fprintf_s(ofp, "%d\n", maxprofit);
	for (i = 1; i < n; i++) {
		fprintf_s(ofp, "%d\n", bestset[i]);
	}
	fprintf_s(ofp, "%d", bestset[i]);

	free(profit);
	free(weight);
	free(include);
	free(bestset);
	fclose(ifp);
	fclose(ofp);

	return 0;
}

void knapsack(int index, int curprofit, int curweight) {
	int i;
	
	if (curweight <= w && curprofit > maxprofit) {
		maxprofit = curprofit;
		numbest = index;
		for (i = 1; i <= n; i++) {
			bestset[i] = include[i];
		}
	}
	if (index < n) {
		if (promising(index, weight[index], profit[index])) {
			include[index + 1] = 1;
			knapsack(index + 1, curprofit + profit[index + 1], curweight + weight[index + 1]);
			include[index + 1] = 0;
			knapsack(index + 1, curprofit, curweight);
		}
	}
}

int promising(int i, int curweight, int curprofit) {
	int j, answer = 1, bound, totweight;

	if (curweight > w)
		answer = 0;
	else {
		j = i + 1;
		bound = curprofit;
		totweight = curweight;
		while (j <= n && totweight + weight[j] <= w) {
			totweight += weight[j];
			bound = bound + profit[j];
			j++;
		}
		if (j <= n) {
			bound += (w - totweight)*(profit[j] / weight[j]);
			answer = bound > maxprofit;
		}
	}

	return answer;
}