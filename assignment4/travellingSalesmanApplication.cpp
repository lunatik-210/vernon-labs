/* 
    travellingSalesmanApplication.cpp - application file for the solution of the travelling saleman problem by exhaustive search using backtracking

    CS-CO-412 Algorithms and Data Structures Assignment No. 4

    See travellingSalesman.h for details of the problem

    Andrew Lapin
*/
 
#include "travellingSalesman.h"


int main() 
{
    input_type input;

    int i, j, k;                                      // general purpose counters
    int n;                                            // number of shops
    int number_of_test_cases;
    int path[NUMBER_OF_STOPS];

    solution_type solution;

    FILE *fp_in, *fp_out;                              // input and output file pointers


    /* open input and output files */

    if ((fp_in = fopen("input.txt","r")) == 0) 
    {
        printf("Error can't open input input.txt\n");
        exit(0);
    }

    if ((fp_out = fopen("output.txt","w")) == 0) 
    {
        printf("Error can't open output output.txt\n");
        exit(0);
    }

    fprintf(fp_out, "%s\n", "Andrew Lapin");

    /* read the number of test cases */

    fscanf(fp_in, "%d\n", &number_of_test_cases);

    /* main processing loop, one iteration for each test case */

    for (k=0; k<number_of_test_cases; k++) 
    {
        /* read the data for each test case  */
        /* --------------------------------  */

        /* number of shops */

        fscanf(fp_in, "%d\n", &n);

        /* get the shop names and the car name */

        for (i = 0; i <n+1; i++) 
        {
            fscanf(fp_in, "%80[^\n]\n", input.shop[i].name);

            input.shop[i].key = i;
        }

        /* get the matrix of distances */

        for (i = 0; i < n+1; i++) 
        {
            for (j = 0; j < n+1; j++) 
            {
                fscanf(fp_in, "%d", &(input.distances[i][j]));
            }
        }

        /* main processing begins here */
        /* --------------------------- */

        init_solution(&solution);
        init_path(path);

        path[0] = n;
        solution.n = n+1;

        backtrack(path, 0, n+1, &input, &solution);

        fprintf(fp_out, "%d\n", k+1);
        print_solution(fp_out, &solution, &input);
    }

    fclose(fp_in);
    fclose(fp_out);
}
