#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "main1.h"

struct Table 
{
    int tournaments_number;
    Tournament** tournaments;
};

struct Tournament 
{
    char* name;

    int teams_number;
    char** team_names;

    int games_number;
    Game** games;
};

struct Game 
{
    char* team_name1;
    int goals_number1;

    char* team_name2;
    int goals_number2;
};

struct Result 
{
    char* team_name;

    int earned_points;
    int played_game_number;
    int win_number;
    int tie_number;
    int loose_number;
    int goal_difference;
    int scored_goal_number;
    int against_goal_number;
};

enum error 
{
    SUCCESS = 0,
    ERROR_INCORRECT_TOURNAMENT_NUMBER = 1,
    ERROR_TOO_LARGE_TOURNAMENT_NAME_LENGTH = 2,
    ERROR_INCORRECT_TEAM_NUMBER = 3,
    ERROR_TOO_LARGE_TEAM_NAME_LENGTH = 4,
    ERROR_FORBIDDEN_CHARACTER = 5,
    ERROR_INCORRECT_GAME_NUMBER = 6,
    ERROR_TOO_LARGE_GOAL_NUMBER = 7,
    ERROR_PARSED_INVALID_TOKEN = 8,
    ERROR_INCORRECT_INPUT_FILE_NAME = 9,
    ERROR_CANNT_OPEN_THE_FILE = 10
};

void process_error(error err)
{
    char* mesage = NULL;
    switch(err)
    {
        case SUCCESS:
            return;
        case ERROR_INCORRECT_TOURNAMENT_NUMBER:
            mesage = "ERROR: Incorrect tournaments number (0 < N < 1000)";
            break;
        case ERROR_TOO_LARGE_TOURNAMENT_NAME_LENGTH:
            mesage = "ERROR: Too large tournament name length (maximum 100 letters)";
            break;
        case ERROR_INCORRECT_TEAM_NUMBER:
            mesage = "ERROR: Incorrect team number (1 < T <= 30)";
            break;
        case ERROR_TOO_LARGE_TEAM_NAME_LENGTH:
            mesage = "ERROR: Too large team name length (maximum 30 letters)";
            break;
        case ERROR_FORBIDDEN_CHARACTER:
            mesage = "ERROR: there are forbidden letters in team name (avoid using @ or #)";
            break;
        case ERROR_INCORRECT_GAME_NUMBER:
            mesage = "ERROR: Incorrect game number (0 <= G <= 1000)";
            break;
        case ERROR_TOO_LARGE_GOAL_NUMBER:
            mesage = "ERROR: Too large goal number (0 < GN < 20)";
            break;
        case ERROR_PARSED_INVALID_TOKEN:
            mesage = "ERROR: can't parse";
            break;
        case ERROR_INCORRECT_INPUT_FILE_NAME:
            mesage = "ERROR: Input file isn't specified";
            break;
        case ERROR_CANNT_OPEN_THE_FILE:
            mesage = "ERROR: File with the given input name cann't be oppened";
            break;
        default:
            mesage = "ERROR: unknown";
    }

    printf("%s\n", mesage);
    exit(err);
}

error parse_file(FILE* infile, Table** table)
{
    int i, j;

    *table = calloc(1, sizeof(Table));
    // Reads number of tournaments
    int tournaments_number;
    if(fscanf(infile, "%d\n", &tournaments_number) != 1)
    {
        return ERROR_PARSED_INVALID_TOKEN;
    }

    if(tournaments_number <=0 || tournaments_number>=1000)
    {
        return ERROR_INCORRECT_TOURNAMENT_NUMBER;
    }

    (*table)->tournaments_number = tournaments_number;
    (*table)->tournaments = calloc(tournaments_number, sizeof(Tournament*));

    // Reads each tournament one by one
    for(i = 0; i<tournaments_number; ++i)
    {
        (*table)->tournaments[i] = calloc(1, sizeof(Tournament));
        (*table)->tournaments[i]->name = calloc(101, sizeof(char));

        if (fscanf(infile, " %100[^\n]\n", (*table)->tournaments[i]->name) != 1) 
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        // Reads teams data
        int teams_number = 0;
        if(fscanf(infile, "%d\n", &teams_number) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(teams_number<=1 || teams_number>30)
        {
            return ERROR_INCORRECT_TEAM_NUMBER;
        }

        (*table)->tournaments[i]->teams_number = teams_number;
        (*table)->tournaments[i]->team_names = calloc(teams_number, sizeof(char*));

        for(j = 0; j<teams_number; ++j)
        {
            (*table)->tournaments[i]->team_names[j] = calloc(31, sizeof(char));
            if (fscanf(infile, " %30[^\n]\n", (*table)->tournaments[i]->team_names[j]) != 1) 
            {
                return ERROR_PARSED_INVALID_TOKEN;
            }
        }

        // Reads games data
        int games_number = 0;
        if(fscanf(infile, "%d\n", &games_number) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(games_number<0 || games_number>1000)
        {
            return ERROR_INCORRECT_GAME_NUMBER;
        }

        (*table)->tournaments[i]->games_number = games_number;
        (*table)->tournaments[i]->games = calloc(games_number, sizeof(Game*));

        for(j = 0; j<games_number; ++j)
        {
            (*table)->tournaments[i]->games[j] = calloc(1, sizeof(Game));
            (*table)->tournaments[i]->games[j]->team_name1 = calloc(31, sizeof(char));
            (*table)->tournaments[i]->games[j]->team_name2 = calloc(31, sizeof(char));
            
            char str[67];
            char goals[5];
            
            int goals_number1 = 0;
            int goals_number2 = 0;

            if (fscanf(infile, "%67[^\n]\n", str) != 1) 
            {
                return ERROR_PARSED_INVALID_TOKEN;
            }

            strcpy((*table)->tournaments[i]->games[j]->team_name1, strtok(str, "#"));
            strcpy(goals, strtok(NULL, "#"));
            strcpy((*table)->tournaments[i]->games[j]->team_name2, strtok(NULL, "#"));

            goals_number1 = atoi(strtok(goals, "@"));
            goals_number2 = atoi(strtok(NULL, "@"));

            if(goals_number1 < 0 || goals_number1 >= 20 || goals_number2 < 0 || goals_number2 >= 20)
            {
                return ERROR_TOO_LARGE_GOAL_NUMBER;
            }

            (*table)->tournaments[i]->games[j]->goals_number1 = goals_number1;
            (*table)->tournaments[i]->games[j]->goals_number2 = goals_number2;
        }
    }

    return SUCCESS;
}

void free_table(Table** table)
{
    int i, j, k;

    for(i = 0; i<(*table)->tournaments_number; ++i)
    {
        for(j = 0; j<(*table)->tournaments[i]->games_number; ++j)
        {
            free((*table)->tournaments[i]->games[j]->team_name1);
            free((*table)->tournaments[i]->games[j]->team_name2);
            free((*table)->tournaments[i]->games[j]);
            (*table)->tournaments[i]->games[j]->team_name1 = NULL;
            (*table)->tournaments[i]->games[j]->team_name2 = NULL;
            (*table)->tournaments[i]->games[j] = NULL;
        }
     
        free((*table)->tournaments[i]->games);
        (*table)->tournaments[i]->games = NULL;

        for(j = 0; j<(*table)->tournaments[i]->teams_number; ++j)
        {
            free((*table)->tournaments[i]->team_names[j]);
            (*table)->tournaments[i]->team_names[j] = NULL;
        }

        free((*table)->tournaments[i]->team_names);
        free((*table)->tournaments[i]->name);
        free((*table)->tournaments[i]);

        (*table)->tournaments[i]->team_names = NULL;
        (*table)->tournaments[i]->name = NULL;
        (*table)->tournaments[i] = NULL;
    }

    free((*table)->tournaments);
    free(*table);

    (*table)->tournaments = NULL;
    *table = NULL;
}

void print_table(Table* table)
{
    int i, j;

    printf("%d\n", table->tournaments_number);
    for(i = 0; i<table->tournaments_number; ++i)
    {
        printf("%s\n", table->tournaments[i]->name);
        printf("%d\n", table->tournaments[i]->teams_number);
        for(j = 0; j<table->tournaments[i]->teams_number; ++j)
        {
            printf("%s\n", table->tournaments[i]->team_names[j]);
        }
        
        printf("%d\n", table->tournaments[i]->games_number);
        for(j = 0; j<table->tournaments[i]->games_number; ++j)
        {
            printf("%s#", table->tournaments[i]->games[j]->team_name1);
            printf("%d@", table->tournaments[i]->games[j]->goals_number1);
            printf("%d#", table->tournaments[i]->games[j]->goals_number2);
            printf("%s\n", table->tournaments[i]->games[j]->team_name2);
        }
    }
}

void fill_result(Result* results, int index, int goal_number1, int goal_number2)
{
    results[index].scored_goal_number += goal_number1;
    results[index].against_goal_number += goal_number2;
    results[index].goal_difference = results[index].scored_goal_number - results[index].against_goal_number;
    results[index].played_game_number += 1;
    if(goal_number1>goal_number2)
    {
        results[index].win_number += 1;
        results[index].earned_points += 3;
        return;
    }
    if (goal_number1<goal_number2)
    {
        results[index].loose_number += 1;
        return;
    }
    results[index].tie_number += 1;
    results[index].earned_points += 1;
}


void print_tournament_results(Tournament* tournament, Result* results)
{
    int j;
    printf("%s\n", tournament->name);

    for(j = 0; j < tournament->teams_number; ++j )
    {
        printf("%d) %s %d p, %dg (%d-%d-%d), %dgd (%d-%d)\n",
            j+1,
            results[j].team_name,
            results[j].earned_points,
            results[j].played_game_number,
            results[j].win_number,
            results[j].tie_number,
            results[j].loose_number,
            results[j].goal_difference,
            results[j].scored_goal_number,
            results[j].against_goal_number);
    }
    printf("\n");
}


void calculate_results(Tournament* tournament, Result** results)
{
    int i, j;

    *results = calloc(tournament->teams_number, sizeof(Result));

    for(i = 0; i<tournament->teams_number; ++i)
    {
        (*results)[i].team_name = calloc(31, sizeof(char));
        strcpy((*results)[i].team_name, tournament->team_names[i]);

        for(j = 0; j<tournament->games_number; ++j)
        {
            if(strcmp((*results)[i].team_name, tournament->games[j]->team_name1)==0)
            {
                fill_result((*results), i, tournament->games[j]->goals_number1, tournament->games[j]->goals_number2);
            }
            if(strcmp((*results)[i].team_name, tournament->games[j]->team_name2)==0)
            {
                fill_result((*results), i, tournament->games[j]->goals_number2, tournament->games[j]->goals_number1);
            }
        }
    }
}

void free_results(Result** results, int size)
{
    int i;
    for(i = 0; i<size; ++i)
    {
        free((*results)[i].team_name);
        (*results)[i].team_name = NULL;
    }
    free(*results);
    *results = NULL;
}


int criterion(Result* v, const int i, const int j)
{
    if(v[i].earned_points != v[j].earned_points)
    {
        return (v[i].earned_points < v[j].earned_points);
    }
    if(v[i].win_number != v[j].win_number)
    {
        return (v[i].win_number < v[j].win_number);
    }
    if(v[i].goal_difference != v[j].goal_difference)
    {
        return (v[i].goal_difference < v[j].goal_difference);
    }
    if(v[i].scored_goal_number != v[j].scored_goal_number)
    {
        return (v[i].scored_goal_number < v[j].scored_goal_number);
    }
    if(v[i].played_game_number != v[j].played_game_number)
    {
        return (v[i].played_game_number > v[j].played_game_number);
    }

    int length1 = strlen(v[i].team_name); 
    int length2 = strlen(v[j].team_name);

    char* name1 = calloc(length1, sizeof(char));
    char* name2 = calloc(length2, sizeof(char));;

    strcpy(name1, v[i].team_name);
    strcpy(name2, v[j].team_name);

    int min_length = (length1<length2)?length1:length2;

    int l;
    for(l = 0; l < length1; ++l)
    {
        name1[l] = tolower(name1[l]);
    }
    
    for(l = 0; l < length2; ++l)
    {
        name2[l] = tolower(name2[l]);
    }

    int k = 0;
    while(name1[k]==name2[k] && k<min_length) ++k;

    int ret = (name1[k] > name2[k]);
    
    free(name1);
    free(name2);

    return ret;
}

void swap(Result* v, int i, int j)
{
    Result saved = v[i];
    v[i] = v[j];
    v[j] = saved;    
}

void _quick_sort(Result* v, int left, int right)
{
    if(left>=right)
        return;

    int m = _quick_sort_partition(v, left, right);
    _quick_sort(v, left, m-1);
    _quick_sort(v, m+1, right);
}

int _quick_sort_partition(Result* v, int left, int right)
{
    int i = left-1;
    int j;
    for(j = left; j<right+1; ++j)
    {
        if(!criterion(v, j, right))
        {
            i+=1;
            swap(v, i, j);
        }
    }
    return i;
}

void quick_sort(Result* v, int size)
{
    _quick_sort(v, 0, size-1);
}

void bubble_sort(Result* v, int size)
{
    int i, j;
    for(i = 0; i < size; ++i)
    {
        for(j = 0; j < size-i-1; ++j)
        {
            if(criterion(v, j, j+1))
            {
                swap(v, j, j+1);
            }
        }
    }
}

int main(int argc, char** argv)
{
    FILE *infile = fopen("in.txt", "r");
    
    if(infile == NULL)
    {
        process_error(ERROR_CANNT_OPEN_THE_FILE);
    }

    Table* table;

    process_error(parse_file(infile, &table));

    int i = 0;
    for(i = 0; i<table->tournaments_number; ++i)
    {

        Result* results;
        calculate_results(table->tournaments[i], &results);
        //bubble_sort(results, table->tournaments[i]->teams_number);
        quick_sort(results, table->tournaments[i]->teams_number);
        print_tournament_results(table->tournaments[i], results);
        free_results(&results, table->tournaments[i]->teams_number);
    }

    free_table(&table);
    fclose(infile);

    return 0;
}
