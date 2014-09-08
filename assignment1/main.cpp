#include <iostream>
#include <sstream>
#include "stdlib.h"

#include "main.h"

using namespace std;

void process_error(error err)
{
    switch(err)
    {
        case SUCCESS:
            return;
        case ERROR_INCORRECT_TOURNAMENT_NUMBER:
            cerr << "ERROR: Incorrect tournaments number (0 < N < 1000)" << endl;
            break;
        case ERROR_TOO_LARGE_TOURNAMENT_NAME_LENGTH:
            cerr << "ERROR: Too large tournament name length (maximum 100 letters)" << endl;
            break;
        case ERROR_INCORRECT_TEAM_NUMBER:
            cerr << "ERROR: Incorrect team number (1 < T <= 30)" << endl;
            break;
        case ERROR_TOO_LARGE_TEAM_NAME_LENGTH:
            cerr << "ERROR: Too large team name length (maximum 30 letters)" << endl;
            break;
        case ERROR_FORBIDDEN_CHARACTER:
            cerr << "ERROR: there are forbidden letters in team name (avoid using @ or #)" << endl;
            break;
        case ERROR_INCORRECT_GAME_NUMBER:
            cerr << "ERROR: Incorrect game number (0 <= G <= 1000)" << endl;
            break;
        case ERROR_TOO_LARGE_GOAL_NUMBER:
            cerr << "ERROR: Too large goal number (0 < GN < 20)" << endl;
            break;
        case ERROR_PARSED_INVALID_TOKEN:
            cerr << "ERROR: can't parse" << endl;
            break;
        case ERROR_INCORRECT_INPUT_FILE_NAME:
            cerr << "ERROR: Input file isn't specified" << endl;
            break;
        case ERROR_CANNT_OPEN_THE_FILE:
            cerr << "ERROR: File with the given input name cann't be oppened" << endl;
            break;
        default:
            cerr << "ERROR: unknown" << endl;
    }

    exit(err);
}

int to_int(string str)
{
    int numb;
    istringstream ( str ) >> numb;
    return numb;
}

bool get_string(ifstream& file, string& line)
{
    if(!file.eof())
    {
        getline(file, line);
        return true;
    }

    return false;
}

error parse_table(ifstream& file, Table& table)
{
    string str;
    if(get_string(file, str))
    {
        table.tournament_number = to_int(str);
        
        if(table.tournament_number<=0 or table.tournament_number>=1000)
        {
            return ERROR_INCORRECT_TOURNAMENT_NUMBER;
        }
    }

    for(int i = 0; i<table.tournament_number; ++i)
    {
        Tournament t;
        error err = parse_tournament(file, t);
        if(err != SUCCESS)
            return err;
        table.tournaments.push_back(t);
    }

    return SUCCESS;
}

error parse_tournament(ifstream& file, Tournament& t)
{
    string str;

    if(get_string(file, str))
    {
        t.name = str;
        if(str.length()>100)
        {
            return ERROR_TOO_LARGE_TOURNAMENT_NAME_LENGTH;
        }
    }
    else
    {
        return ERROR_PARSED_INVALID_TOKEN;
    }

    if(get_string(file, str))
    {
        t.team_number = to_int(str);
        if(t.team_number<=1 or t.team_number>30)
        {
            return ERROR_INCORRECT_TEAM_NUMBER;
        }
    }
    else
    {
        return ERROR_PARSED_INVALID_TOKEN;
    }

    for(int i = 0; i < t.team_number; ++i)
    {
        if(get_string(file, str))
        {
            if(str.length()>30)
            {
                return ERROR_TOO_LARGE_TEAM_NAME_LENGTH;
            }
            if(str.find("#")!=string::npos or str.find("@")!=string::npos)
            {
                return ERROR_FORBIDDEN_CHARACTER;
            }
            t.team_names.push_back(str);
            continue;
        }
        return ERROR_PARSED_INVALID_TOKEN;
    }

    if(get_string(file, str))
    {
        t.game_number = to_int(str);
        if(t.game_number<0 or t.game_number>1000)
        {
            return ERROR_INCORRECT_GAME_NUMBER;
        }
    }
    else
    {
        return ERROR_PARSED_INVALID_TOKEN;
    }

    for(int i = 0; i < t.game_number; ++i)
    {
        if(get_string(file, str))
        {
            Game game;
            error err = parse_game(str, game);
            if(SUCCESS != err)
                return err;
            t.game.push_back(game);
            continue;
        }
        return ERROR_PARSED_INVALID_TOKEN;
    }

    return SUCCESS;
}

error parse_game(string& str, Game& game)
{
    string left = str.substr(0, str.find("@"));
    string right = str.substr(str.find("@")+1);

    game.team_name1 = left.substr(0, left.find("#"));
    game.goal_number1 = to_int(left.substr(left.find("#")+1));
    game.team_name2 = right.substr(right.find("#")+1);
    game.goal_number2 = to_int(right.substr(0, right.find("#")));

    if(game.goal_number1 < 0 or game.goal_number1 >= 20 or game.goal_number2 < 0 or game.goal_number2 >= 20)
    {
        return ERROR_TOO_LARGE_GOAL_NUMBER;
    }

    return SUCCESS;
}

void print_table(const Table& table)
{
    cout << table.tournament_number << endl;

    for(int i = 0; i < table.tournament_number; ++i)
    {
        cout << table.tournaments[i].name << endl;

        cout << table.tournaments[i].team_number << endl;

        for(int j = 0; j<table.tournaments[j].team_number; ++j)
        {
            cout << table.tournaments[i].team_names[j] << endl;
        }
        
        cout << table.tournaments[i].game_number << endl;

        for(int j = 0; j<table.tournaments[i].game_number; ++j)
        {
            cout << table.tournaments[i].game[j].team_name1 << "#" 
                 << table.tournaments[i].game[j].goal_number1 << "@" 
                 << table.tournaments[i].game[j].goal_number2 << "#" 
                 << table.tournaments[i].game[j].team_name2 << endl;
        }

        cout << table.tournaments[i].game_number << endl;
    }
}

void print_tournament_results(const Tournament& tournament, const vector<Result>& results)
{
    cout << tournament.name << endl;

    for(int j = 0; j < results.size(); ++j )
    {
        cout << j+1 << ") " << results[j].team_name << " "
             << results[j].earned_points << "p, "
             << results[j].played_game_number << "g "
             << "(" << results[j].win_number 
             << "-" << results[j].tie_number
             << "-" << results[j].loose_number << "), "
             << results[j].goal_difference << "gd "
             << "(" << results[j].scored_goal_number
             << "-" << results[j].against_goal_number << ")"
             << endl;
    }

    cout << endl;
}

void fill_result(Result& r, int goal_number1, int goal_number2)
{
    r.scored_goal_number += goal_number1;
    r.against_goal_number += goal_number2;
    r.goal_difference = r.scored_goal_number - r.against_goal_number;
    r.played_game_number += 1;
    if(goal_number1>goal_number2)
    {
        r.win_number += 1;
        r.earned_points += 3;
        return;
    }
    if (goal_number1<goal_number2)
    {
        r.loose_number += 1;
        return;
    }
    r.tie_number += 1;
    r.earned_points += 1;
}

void calculate_results(const Tournament& tournament, vector<Result>& results)
{
    for(int i = 0; i<tournament.team_number; ++i)
    {
        Result r = { tournament.team_names[i], 0, 0, 0, 0, 0, 0, 0, 0 };

        for(int j = 0; j<tournament.game_number; ++j)
        {
            if(r.team_name == tournament.game[j].team_name1)
            {
                fill_result(r, tournament.game[j].goal_number1, tournament.game[j].goal_number2);
            }
            if(r.team_name == tournament.game[j].team_name2)
            {
                fill_result(r, tournament.game[j].goal_number2, tournament.game[j].goal_number1);
            }
        }

        results.push_back(r);
    }
}

bool criterion(const vector<Result>& v, const int i, const int j)
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

    string name1 = v[i].team_name;
    string name2 = v[j].team_name;

    int length1 = name1.length(); 
    int length2 = name2.length();
    int min_length = (length1<length2)?length1:length2;

    for(int l = 0; l < length1; ++l)
    {
        name1[l] = tolower(name1[l]);
    }
    
    for(int l = 0; l < length2; ++l)
    {
        name2[l] = tolower(name2[l]);
    }

    int k = 0;
    while(name1[k]==name2[k] and k<min_length) ++k;

    return (name1[k] > name2[k]);
}

void swap(vector<Result>& v, int i, int j)
{
    Result saved = v[i];
    v[i] = v[j];
    v[j] = saved;    
}

void bubble_sort(vector<Result>& v)
{
    for(int i = 0; i < v.size(); ++i)
    {
        for(int j = 0; j < v.size()-i-1; ++j)
        {
            if(criterion(v, j, j+1))
            {
                swap(v, j, j+1);
            }
        }
    }
}

void quick_sort(vector<Result>& v)
{
    _quick_sort(v, 0, v.size()-1);
}

void _quick_sort(vector<Result>& v, int left, int right)
{
    if(left>=right)
        return;

    int m = _quick_sort_partition(v, left, right);
    _quick_sort(v, left, m-1);
    _quick_sort(v, m+1, right);
}

int _quick_sort_partition(vector<Result>& v, int left, int right)
{
    int i = left-1;
    for(int j = left; j<right+1; ++j)
    {
        if(!criterion(v, j, right))
        {
            i+=1;
            swap(v, i, j);
        }
    }
    return i;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        process_error(ERROR_INCORRECT_INPUT_FILE_NAME);
    }

    ifstream file(argv[1]);

    if (!file.is_open())
    {
        process_error(ERROR_CANNT_OPEN_THE_FILE);
    }

    Table table;

    process_error(parse_table(file, table));

    for(int i = 0; i < table.tournament_number; ++i)
    {
        vector<Result> results;
        calculate_results(table.tournaments[i], results);
        //bubble_sort(results);
        quick_sort(results);
        print_tournament_results(table.tournaments[i], results);
    }

    return SUCCESS;
}
