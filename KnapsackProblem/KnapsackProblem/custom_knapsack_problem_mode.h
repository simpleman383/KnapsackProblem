#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "tools.h"
#define MAX_STACK_DEPTH 4000

int err;

char input[32];
int N, WeightLimit;
int * value;
int * weight;
int ** A;

// to avoid stack overflow
int depth = 0;
int k_mem, s_mem;
int interrupt_flag = 0;


int enter_N()
{
	printf("Please, enter amount of items (integer value):\n");

	while (1)
	{
		printf("N = ");
		scanf("%s", input);

		if (!validate_amount(input, strlen(input)))
			printf("Error: value must be integer\n");
		else
		{
			N = convert_to_integer(input, strlen(input));
			if (N == 0)
				printf("Error: value must be greater than 0\n");
			else
				break;
		}
	}

	return 0;
}

int enter_weights(int amount) 
{
	weight = (int*)(malloc(amount * sizeof(int)));
	if (!weight)
	{
		printf("Fatal error!\n");
		return 1;
	}

	printf("Please, enter weight for each item:\n");
	for (int i = 0; i < amount; i++) {
		while (1)
		{
			printf("weight[%d] = ", i);
			scanf("%s", input);

			if (!validate_amount(input, strlen(input)))
				printf("Error: value must be integer\n");
			else
			{
				weight[i] = convert_to_integer(input, strlen(input));
				if (weight[i] == 0)
					printf("Error: value must be greater than 0\n");
				else
					break;
			}
		}
	}

	return 0;
}

int enter_values(int amount)
{
	value = (int*)(malloc(amount * sizeof(int)));
	if (!value)
	{
		printf("Fatal error!\n");
		return 1;
	}

	printf("Please, enter value for each item:\n");
	for (int i = 0; i < amount; i++) {
		while (1)
		{
			printf("value[%d] = ", i);
			scanf("%s", input);

			if (!validate_amount(input, strlen(input)))
				printf("Error: value must be integer\n");
			else
			{
				value[i] = convert_to_integer(input, strlen(input));
				if (value[i] == 0)
					printf("Error: value must be greater than 0\n");
				else
					break;
			}
		}
	}

	return 0;
}

int enter_WeightLimit()
{
	printf("Please, enter limit of weight (integer value):\n");

	while (1)
	{
		printf("Limit = ");
		scanf("%s", input);

		if (!validate_amount(input, strlen(input)))
			printf("Error: value must be integer\n");
		else
		{
			WeightLimit = convert_to_integer(input, strlen(input));
			if (WeightLimit == 0)
				printf("Error: value must be greater than 0\n");
			else
				break;
		}
	}

	return 0;
}

int calculateMatrix() {
	A = (int**)(malloc(sizeof(int*) * (N + 1)));
	if (!A)
	{
		printf("Fatal Error!\n");
		return 1;
	}

	for (int i = 0; i <= N; i++)
	{
		A[i] = (int*)malloc(sizeof(int) * (WeightLimit + 1));
		if (!A[i])
		{
			for (int j = 0; j < i; j++)
				free(A[j]);
			free(A);
			printf("Fatal Error!\n");
			return 1;
		}
	}

	for (int i = 0; i <= WeightLimit; i++) A[0][i] = 0;
	for (int i = 0; i <= N; i++) A[i][0] = 0;

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= WeightLimit; j++)
		{
			if (j >= weight[i - 1])
				A[i][j] = max(A[i - 1][j], A[i - 1][j  - weight[i - 1]] + value[i - 1]);
			else
				A[i][j] = A[i - 1][j];
		}

	printf("Matrix:\n");
	print_matrix(A, N, WeightLimit);
}

void solve(int k, int s) {
	depth++;
	if (depth > MAX_STACK_DEPTH) {
		k_mem = k;
		s_mem = s;
		interrupt_flag = 1;
		depth = 0;
		return;
	}

	if (A[k][s] == 0)
	{
		interrupt_flag = 0;
		return;
	}
	if (A[k - 1][s] == A[k][s])
		solve(k - 1, s);
	else
	{
		solve(k - 1, s - weight[k - 1]);
		printf("%d ", k - 1);
	}
}

void clean_up() 
{
	free(value);
	free(weight);
	
	for (int j = 0; j <= N; j++)
		free(A[j]);
	free(A);
}

void custom_knapsack_problem_mode() 
{
	err = enter_N();

	err = enter_weights(N);
	if (err) return;

	err = enter_values(N);
	if (err) return;

	err = enter_WeightLimit();

	printf("\nSOURCE DATA:\n");
	printf("N = %d\n", N);
	printf("Weights:\n");
	print_integer_buffer(weight, N);
	printf("Values:\n");
	print_integer_buffer(value, N);
	printf("Weight limit = %d\n", WeightLimit);

	printf("\nstarting calculations....\n");
	calculateMatrix();
	printf("\nRESULT:\n");
	solve(N, WeightLimit);
	while (interrupt_flag) solve(k_mem, s_mem);
	printf("\n");

	clean_up();
	return;
}
