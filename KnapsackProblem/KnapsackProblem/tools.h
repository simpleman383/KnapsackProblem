#pragma once
#include "string.h"

int validate_menu_mode_input(char*input_data, int len) 
{
	if (len > 1) return 0;

	for (int i = 0; i < len; i++) {
		if (input_data[i] < 48 || input_data[i] > 51) return 0;
	}

	return 1;
}

int validate_help_mode_input(char*input_data, int len)
{
	if (len > 1) return 0;

	for (int i = 0; i < len; i++) {
		if (input_data[i] != 48 ) return 0;
	}

	return 1;
}

int validate_menu_1_mode_input(char*input_data, int len)
{
	if (len > 1) return 0;

	for (int i = 0; i < len; i++) {
		if (input_data[i] <= 48 || input_data[i] > 51) return 0;
	}

	return 1;
}

int validate_main_loop_feedback(char*input_data)
{
	if (!(strcmp(input_data, "Yes") * strcmp(input_data, "y") * strcmp(input_data, "Y") * strcmp(input_data, "yes"))) return 1;
	else if (!(strcmp(input_data, "No") * strcmp(input_data, "n") * strcmp(input_data, "N") * strcmp(input_data, "no"))) return 0;
	else return -1;
}

int validate_amount(char*input, int len) 
{
	if (len == 0) return 0;
	for (int i = 0; i < len; i++)
	{
		if (input[i] < 48 || input[i] > 57) return 0;
	}
	return 1;
}

int convert_to_integer(char*input, int len) {
	int result = 0;
	int base = 1;

	for (int i = len - 1; i >= 0; i--) 
	{
		result += (input[i] - 48) * base;
		base *= 10;
	}

	return result;
}

void print_integer_buffer(int*buf, int size)
{
	for (int i = 0; i < size; i++) 
	{
		printf("%d	", buf[i]);
	}
	printf("\n");
}

void print_matrix(int**A, int N, int W)
{
	printf("k\\s	");

	for (int i = 0; i <= W; i++)
		printf("%d	", i);
	printf("\n");

	for (int i = 0; i <= N; i++) 
	{
		printf("%d	", i);
		for (int j = 0; j <= W; j++)
			printf("%d	", A[i][j]);
		printf("\n");
	}
}