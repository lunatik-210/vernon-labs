/* 
    travellingSalesmanImplementation.cpp - application file for the solution of the travelling saleman problem by exhaustive search using backtracking

    CS-CO-412 Algorithms and Data Structures Assignment No. 4

    See travellingSalesman.h for details of the problem

    Andrew Lapin
*/

#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H
 
#include "travellingSalesman.h"

// fill solution with initial values
// path cost/weight = INT_MAX
// path = [ -1, ... ]
// n = -1
void init_solution(solution_type* solution)
{
    solution->weight = INT_MAX;
    solution->n = -1;
    for(int i = 0; i<NUMBER_OF_STOPS; ++i)
    {
        solution->path[i] = -1;
    }
}

// fill path with -1 values
void init_path(int* path)
{
    for(int i = 0; i<NUMBER_OF_STOPS; ++i)
    {
        path[i] = -1;
    }
}

// print solution to the file
void print_solution(FILE *fp_out, solution_type* solution, input_type* input)
{
    fprintf(fp_out, "%d\n", solution->weight);
    for(int i = 0; i<solution->n+1; ++i)
    {
        fprintf(fp_out, "%s\n", input->shop[solution->path[i]].name);
    }
    fprintf(fp_out, "\n");
}

// calculate cost/weight of the path
int calulate_weight(input_type* input, int* path, int n)
{
    int w = 0;
    for(int i = 0; i<n; ++i)
    {
        w += input->distances[path[i]][path[i+1]];
    }
    return w;
}

// update path and cost of best solution
void update_solution(solution_type* solution, int* path, int w)
{
    for(int i = 0; i<=solution->n; ++i)
    {
        solution->path[i] = path[i];
    }
    solution->weight = w;
}

// return true if we reach complete permutation
bool is_solution(int k, int n)
{
    return (k == n-2);
}

// backtracking algorythm itself with pruning 
void backtrack(int* path, int k, int n, input_type* input, solution_type* solution)
{
    // marking already visited shops
	bool *is_used = (bool*)malloc(n*sizeof(bool));

    for(int i = 0; i<n; ++i) is_used[i] = false;
    for(int i = 0; i<k+1; ++i) if(path[i]!=-1) is_used[path[i]] = true;

    if(is_solution(k, n) == true)
    {
        // completing permutation
        for(int i = 0; i<n; ++i) 
        {
            if(is_used[i] == false)
            {
                path[n-1] = i;
            }
        }
        path[n] = n-1;

        int w = calulate_weight(input, path, n);

        // updating best solution if new one has lower path cost
        if(w < solution->weight)
        {
            update_solution(solution, path, w);
        }
    }
    else
    {
        // generate new permutations using unvisited shops
        for(int i = 0; i<n; ++i) 
        {
            if(is_used[i] == false)
            {
                path[k+1] = i;

                // do pruning if cost of the subtree is greater than current best solution
                if(solution->weight > calulate_weight(input, path, k))
                {
                    backtrack(path, k+1, n, input, solution);
                }
            }
        }
    }

	free(is_used);
}


#endif // IMPLEMENTATION_H