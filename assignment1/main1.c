#include "string.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct Table Table;
typedef struct Tournament Tournament;
typedef struct Game Game;
typedef struct Result Result;
typedef enum error error;

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

void process_error(error err);
error parse_file(FILE* infile, Table** table);


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

        if (fscanf(infile, " %100[^\n]", (*table)->tournaments[i]->name) != 1) 
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        // Reads teams data
        int teams_number;
        if(fscanf(infile, "%d\n", &teams_number) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(teams_number<=1 || teams_number>30)
        {
            return ERROR_INCORRECT_TEAM_NUMBER;
        }

        (*table)->tournaments[i]->team_names = calloc(teams_number, sizeof(char*));

        for(j = 0; j<teams_number; ++j)
        {
            (*table)->tournaments[i]->team_names[j] = calloc(31, sizeof(char));
            if (fscanf(infile, " %30[^\n]", (*table)->tournaments[i]->team_names[j]) != 1) 
            {
                return ERROR_PARSED_INVALID_TOKEN;
            }
        }

        // Reads games data
        int games_number;
        if(fscanf(infile, "%d\n", &games_number) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(games_number<0 || games_number>1000)
        {
            return ERROR_INCORRECT_GAME_NUMBER;
        }

        (*table)->tournaments[i]->games = calloc(games_number, sizeof(Game*));

        for(j = 0; j<games_number; ++j)
        {
            (*table)->tournaments[i]->games[j] = calloc(1, sizeof(Game));
            (*table)->tournaments[i]->games[j]->team_name1 = calloc(31, sizeof(char));
            (*table)->tournaments[i]->games[j]->team_name2 = calloc(31, sizeof(char));
            int goals_number1;
            int goals_number2;
            if (fscanf(infile, "%s#%d@%s#%d\n", (*table)->tournaments[i]->games[j]->team_name1, 
                                             &goals_number1, 
                                             (*table)->tournaments[i]->games[j]->team_name2,
                                             &goals_number2) != 1) 
            {
                return ERROR_PARSED_INVALID_TOKEN;
            }

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
            //free((*table)->tournaments[i]->games[j]->team_name1);
            //free((*table)->tournaments[i]->games[j]->team_name2);
            //free((*table)->tournaments[i]->games[j]);
        }
        //free((*table)->tournaments[i]->games);

        for(j = 0; j<(*table)->tournaments[i]->teams_number; ++j)
        {
            //free((*table)->tournaments[i]->team_names[j]);
        }
        //free((*table)->tournaments[i]->team_names);
        //free((*table)->tournaments[i]->name);
        //free((*table)->tournaments[i]);
    }
    //free((*table)->tournaments);
    //free(*table);
}

int main(int argc, char** argv)
{
    FILE *infile = fopen("in.txt", "r");
    
    if(infile == NULL)
    {
        process_error(ERROR_CANNT_OPEN_THE_FILE);
    }

    Table* table;

    parse_file(infile, &table);



    free_table(&table);

    fclose(infile);

    return 0;
}