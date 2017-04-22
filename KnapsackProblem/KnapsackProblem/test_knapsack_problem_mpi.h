#pragma once
#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"

#define MAX_WEIGHT 10
#define MAX_VALUE 10

#define N 5
#define WeightLimit 13

int ** A;
int * weight;
int * value;
//int N, WeightLimit;

int * prev_row;
int * cur_row;


int process_id, process_amount;


void generate_weight_value()
{
	srand(NULL);
	for (int i = 0; i < N; i++)
	{
		weight[i] = 1 + rand() % MAX_WEIGHT;
		value[i] = 1 + rand() % MAX_VALUE;
	}
}

void test_knapsack_problem_mpi_mode(int argc, char*argv[]) 
{
	fflush(stdout);
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &process_amount);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

	weight = (int*)malloc(N * sizeof(int));
	if (!weight)
	{
		printf("Fatal error!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return;
	}
	value = (int*)malloc(N * sizeof(int));
	if (!value)
	{
		printf("Fatal error!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return;
	}

	if (process_id == 0) generate_weight_value();
	MPI_Bcast(weight, N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(value, N, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	cur_row = (int*)malloc((WeightLimit + 1) * sizeof(int));
	if (!cur_row)
	{
		printf("Fatal error!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return;
	}
	prev_row = (int*)malloc((WeightLimit + 1) * sizeof(int));
	if (!prev_row)
	{
		printf("Fatal error!\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
		return;
	}

	memset(cur_row, 0x00, sizeof(int) * ( WeightLimit + 1));
	memset(prev_row, 0x00, sizeof(int) * (WeightLimit + 1));



	if (process_id == 0)
	{
		A = (int**)(malloc(sizeof(int*) * (N + 1)));
		if (!A)
		{
			printf("Fatal Error!\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
			return;
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
				MPI_Abort(MPI_COMM_WORLD, 1);
				return;
			}
			memset(A[i], 0x00, sizeof(int) * (WeightLimit + 1));
		}
	}


	for (int row = 1; row <= N; row++)
	{
		memset(cur_row, 0x00, sizeof(int) * (WeightLimit + 1));

		for (int col = process_id; col <= WeightLimit; col += process_amount)
		{
			if (col == 0) cur_row[col] = 0;
			else
			{
				if (col >= weight[row - 1])
					cur_row[col] = max(prev_row[col], prev_row[col - weight[row - 1]] + value[row - 1]);
				else
					cur_row[col] = prev_row[col];
			}
		}

		MPI_Reduce(cur_row, A[row], WeightLimit + 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		prev_row = cur_row;

	}



	free(weight);
	free(value);
	free(cur_row);
	if (process_id == 0)
	{
		for (int j = 0; j <= N; j++)
			free(A[j]);
		free(A);
	}

	MPI_Finalize();
}
