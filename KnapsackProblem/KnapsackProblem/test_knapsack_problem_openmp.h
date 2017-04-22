#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "tools.h"
#include "time.h"
#include "omp.h"
#define MAX_STACK_DEPTH 4000

#define MAX_WEIGHT 10
#define MAX_VALUE 10
#define ATTEMPTS 10

#define MIN_N 1000
#define MAX_N 10000
#define STEP_N 1000

#define MIN_W 5000
#define MAX_W 40000
#define STEP_W 1000

char threads_count_input[16];
int N, WeightLimit, errcode;
int * weight;
int * value;
int x, j;
int threads_numb;
clock_t start, stop;
FILE * report_openmp;

// to avoid stack overflow
int depth_test_omp = 0;
int k_mem_test_omp, s_mem_test_omp;
int interrupt_flag_test_omp = 0;

int generate_weights(int amount)
{
	srand(time(NULL));

	weight = (int*)malloc(amount * sizeof(int));
	if (!weight) {
		printf("Fatal error!\n");
		return 1;
	}

	for (int i = 0; i < amount; i++) 
	{
		weight[i] = 1 + rand() % MAX_WEIGHT;
	}
	return 0;
}

int generate_values(int amount)
{
	srand(time(NULL));

	value = (int*)malloc(amount * sizeof(int));
	if (!value) {
		printf("Fatal error!\n");
		return 1;
	}

	for (int i = 0; i < amount; i++)
	{
		value[i] = 1 + rand() % MAX_VALUE;
	}
	return 0;
}

int calculateMatrix_openmp() {
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

#pragma omp parallel for private(x)
	for (x = 0; x <= WeightLimit; x++) A[0][x] = 0;

#pragma omp parallel for private(x)
	for (x = 0; x <= N; x++) A[x][0] = 0;

	for (int i = 1; i <= N; i++)
#pragma omp parallel for private(j)
		for (j = 1; j <= WeightLimit; j++)
		{
			if (j >= weight[i - 1])
				A[i][j] = max(A[i - 1][j], A[i - 1][j - weight[i - 1]] + value[i - 1]);
			else
				A[i][j] = A[i - 1][j];
		}
}

void solve_openmp(int k, int s) {
	depth_test_omp++;
	if (depth_test_omp > MAX_STACK_DEPTH) {
		k_mem_test_omp = k;
		s_mem_test_omp = s;
		interrupt_flag_test_omp = 1;
		depth_test_omp = 0;
		return;
	}

	if (A[k][s] == 0)
	{
		interrupt_flag_test_omp = 0;
		return;
	}
	if (A[k - 1][s] == A[k][s])
		solve_openmp(k - 1, s);
	else
	{
		solve_openmp(k - 1, s - weight[k - 1]);
	}
}

void clean_up_openmp()
{
	free(value);
	free(weight);

	for (int j = 0; j <= N; j++)
		free(A[j]);
	free(A);
}

int enter_threads_numb_openmp()
{
	printf("Please, enter number of threads (integer value less than 9):\n");

	while (1)
	{
		printf("Threads Numb = ");
		scanf("%s", threads_count_input);

		if (!validate_amount(threads_count_input, strlen(threads_count_input)))
			printf("Error: value must be integer\n");
		else
		{
			threads_numb = convert_to_integer(threads_count_input, strlen(threads_count_input));
			if (threads_numb == 0 || threads_numb > 8)
				printf("Error: value must be greater than 0 and less than 9\n");
			else
				break;
		}
	}

	return 0;
}

void test_knapsack_problem_open_mp_mode() 
{
	enter_threads_numb_openmp();

	omp_set_num_threads(threads_numb);

	printf("testing openmp parallel algorithm.....\n");

	report_openmp = fopen("open_mp_report.txt", "w");

	fprintf(report_openmp, "Threads count = %d\n\n", threads_numb);
	fprintf(report_openmp, "N\\W	");
	for (int limit = MIN_W; limit <= MAX_W; limit += STEP_W)
		fprintf(report_openmp, "%d	", limit);
	fprintf(report_openmp, "\n");

	for (int n = MIN_N; n <= MAX_N; n += STEP_N)
	{
		printf("%.1f %% completed...\n", (double)(n - STEP_N) * 100 / MAX_N);
		fprintf(report_openmp, "%d	", n);

		for (int limit = MIN_W; limit <= MAX_W; limit += STEP_W)
		{
			N = n;
			WeightLimit = limit;

			double time = 0;
			for (int a = 0; a < ATTEMPTS; a++)
			{
				errcode = generate_weights(N);
				if (errcode)
				{
					printf("Fatal error\n");
					return;
				}
				errcode = generate_values(N);
				if (errcode)
				{
					printf("Fatal error\n");
					return;
				}


				start = clock();

				calculateMatrix_openmp();
				solve_openmp(N, WeightLimit);
				while (interrupt_flag_test_omp) solve_openmp(k_mem_test_omp, s_mem_test_omp);

				stop = clock();
				double time_elapsed = ((double)stop - (double)start) / CLOCKS_PER_SEC;
				time += time_elapsed;

				clean_up_openmp();
			}

			fprintf(report_openmp, "%.3f	", time / ATTEMPTS);

		}

		fprintf(report_openmp, "\n");
	}
		
	fclose(report_openmp);
	printf("Done! The report has been generated. File: 'open_mp_report.txt'\n");
}
