#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "tools.h"
#include "custom_knapsack_problem_mode.h"
#include "test_knapsack_problem_openmp.h"
#include "stdio.h"
#include "stdlib.h"


/*main function contains basic steps of program*/
int main() {
	int mode = 0;
	char again[8];
	int main_loop = 1;


	while (main_loop) 
	{
		mode = start_menu_mode();
		switch (mode)
		{
		case 1:
		{
			custom_knapsack_problem_mode();
			break;
		}
		case 2:
		{
			test_knapsack_problem_open_mp_mode();
			break;
		}
		case 3:
		{
			//test_knapsack_problem_mpi_mode();
			break;
		}
		case 4:
		{
			printf("exit......\n");
			return 0;
			break;
		}
		default:
		{
			printf("exit......\n");
			return 0;
			break;
		}
		}

		printf("\nDo you want to solve something else? (y/n)\n");
		while (1) 
		{
			scanf("%s", again);
			getchar();
			if (validate_main_loop_feedback(again) == 0)
			{
				main_loop = 0;
				break;
			}
			else if (validate_main_loop_feedback(again) == -1)
				printf("Error: incorrect input\n");
			else
				break;
		}
	}

	printf("exit......\n");
	return 0;
}
