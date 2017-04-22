#pragma once
#include "stdio.h"
#include "stdlib.h"
#include "tools.h"
#include "string.h"


void print_menu_level_0() {
	system("cls");
	printf("KNAPSACK PROBLEM v1.0\n");
	printf("created by Kurnev A., Babkin S., NRNU MEPhI, 2017\n\n");
	printf("MENU:\n");
	printf("0) Help\n");
	printf("1) Solve custom problem\n");
	printf("2) Test mode\n");
	printf("3) Exit\n");
	printf("Please, choose a mode from this list: ");
	return;
}


void print_menu_level_1(int prev_mode) {
	system("cls");
	printf("KNAPSACK PROBLEM v1.0\n");
	printf("created by Kurnev A., Babkin S., NRNU MEPhI, 2017\n\n");

	switch (prev_mode)
	{
		case 1:
		{
			printf("Solve custom problem\n");
			break;
		}
		case 2:
		{
			printf("Test mode\n");
			printf("1) Open_MP\n");
			printf("2) MPI\n");
			printf("3) Back\n");
			printf("Please, choose a mode from this list: ");
			break;
		}
		default:
			break;
		
	}

	return;
}


void start_help_mode() {
	system("cls");
	printf("KNAPSACK PROBLEM v1.0\n");
	printf("created by Kurnev A., Babkin S., NRNU MEPhI, 2017\n\n");

	printf("HELP:\n");
	printf("The knapsack problem or rucksack problem is a problem in combinatorial optimization: Given a set of items, each with a weight and a value, determine the number of each item to include in a collection so that the total weight is less than or equal to a given limit and the total value is as large as possible.\n\n");

	printf("Controls:\n");
	printf("To select concrete menu item you should just type its number (integer value)\n\n");


	printf("Options:\n");
	printf("Help\n");
	printf("Contains some information about this program\n\n");

	printf("Solve custom problem\n");
	printf("This option allows user to solve custom knapsack problem. User should enter amount of items (integer value), then enter weights (integer) and values (integer). Finally, user should enter limit of weight. The program calcultes optimal set of items and displays numbers of items in the set.\n\n");

	printf("Test mode\n");
	printf("This option allows user to test knapsack problem algorithm time complexity. First, user should choose parallelizing technology (Open_MP or MPI). Then the calculation will start and the program will generate a report. \n\n");

	printf("type '0' to go to main menu...\n");

	char cmd[2];
	while (1) 
	{
		fflush(stdout);
		scanf("%s", cmd);
		if (!validate_help_mode_input(cmd, strlen(cmd)))
			printf("Error: incorrect input\n");
		else
			break;
	}

	return;
}


int start_menu_mode() 
{
	char select_input[8];
	int level_0_input, level_1_input;
	int prev_mode = -1;
	int can_continue = 1;

	while (prev_mode == 0 || prev_mode == -1)
	{

		print_menu_level_0();

		while (1)
		{
			fflush(stdout);
			scanf("%s", select_input);
			if (!validate_menu_mode_input(select_input, strlen(select_input)))
				printf("Error: incorrect input\n");
			else
				break;
		}

		can_continue = 1;
		level_0_input = select_input[0] - 48;

		switch (level_0_input)
		{
		case 0:
		{
			start_help_mode();
			can_continue = 0;
			prev_mode = 0;
			break;
		}
		case 1:
		{
			print_menu_level_1(level_0_input);
			return 1;
			break;
		}
		case 2:
		{
			print_menu_level_1(level_0_input);
			break;
		}
		case 3:
		{
			return 4;
			break;
		}

		default:
			return 4;
			break;
		}

		if (can_continue)
		{
			print_menu_level_1(level_0_input);
			while (1)
			{
				fflush(stdout);
				scanf("%s", select_input);
				if (!validate_menu_1_mode_input(select_input, strlen(select_input)))
					printf("Error: incorrect input\n");
				else
					break;
			}

			level_1_input = select_input[0] - 48;

			switch (level_1_input)
			{
			case 1:
			{
				return 2;
				break;
			}
			case 2:
			{
				return 3;
				break;
			}
			case 3:
			{
				prev_mode = -1;
				break;
			}

			default:
				return 4;
				break;
			}

		}
	}

}


