#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Table;
struct Tournament;
struct Game;

struct Table {
    int tournament_number;
    vector<Tournament> tournaments;
};

struct Tournament {
    string name;

    int team_number;
    vector<string> team_names;

    int game_number;
    vector<Game> game;
};

struct Game {
    string team_name1;
    int goal_number1;

    string team_name2;
    int goal_number2;
};

int to_int(string str);
bool get_string(ifstream& file, string& line);
bool parse_table(ifstream& file, Table& table);
bool parse_tournament(ifstream& file, Tournament& table);
bool parse_game(string& str, Game& game);

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

bool parse_table(ifstream& file, Table& table)
{
    string str;
    if(get_string(file, str))
    {
        table.tournament_number = to_int(str);
        
        if(table.tournament_number<=0 or table.tournament_number>=1000)
        {
            cerr << "ERROR: Incorrect tournaments number (0 < N < 1000)" << endl;
            return false;
        }
    }

    while(!file.eof())
    {
        Tournament t;
        if(!parse_tournament(file, t))
            return false;
        table.tournaments.push_back(t);
    }

    return true;
}

bool parse_tournament(ifstream& file, Tournament& t)
{
    string str;

    if(get_string(file, str))
    {
        t.name = str;
        if(str.length()>100)
        {
            cerr << "ERROR: Too large tournament name (maximum 100 letters)" << endl;
            return false;
        }
    }
    else
    {
        cerr << "ERROR: can't parse" << endl;
        return false;
    }

    if(get_string(file, str))
    {
        t.team_number = to_int(str);
        if(t.team_number<=1 or t.team_number>30)
        {
            cerr << "ERROR: Incorrect teams number (1 < T <= 30)" << endl;
            return false;
        }
    }
    else
    {
        cerr << "ERROR: can't parse" << endl;
        return false;
    }

    for(int i = 0; i < t.team_number; ++i)
    {
        if(get_string(file, str))
        {
            if(str.length()>30)
            {
                cerr << "ERROR: Too team name (maximum 30 letters)" << endl;
                return false;
            }
            if(str.find("#")!=string::npos or str.find("@")!=string::npos)
            {
                cerr << "ERROR: there are forbidden letters in team name (avoid using @ or #)" << endl;
                return false;
            }
            t.team_names.push_back(str);
            continue;
        }
        cerr << "ERROR: can't parse" << endl;
        return false;
    }

    if(get_string(file, str))
    {
        t.game_number = to_int(str);
        if(t.game_number<0 or t.game_number>1000)
        {
            cerr << "ERROR: Incorrect game number (0 <= G <= 1000)" << endl;
            return false;
        }
    }
    else
    {
        cerr << "ERROR: can't parse" << endl;
        return false;
    }

    for(int i = 0; i < t.game_number; ++i)
    {
        if(get_string(file, str))
        {
            Game game;
            if(!parse_game(str, game))
                return false;
            t.game.push_back(game);
            continue;
        }
        cerr << "ERROR: can't parse" << endl;
        return false;
    }

    return true;
}

bool parse_game(string& str, Game& game)
{
    string left = str.substr(0, str.find("@"));
    string right = str.substr(str.find("@")+1);

    game.team_name1 = left.substr(0, left.find("#"));
    game.goal_number1 = to_int(left.substr(left.find("#")+1));
    game.team_name2 = right.substr(right.find("#")+1);
    game.goal_number2 = to_int(right.substr(0, right.find("#")));

    if(game.goal_number1 < 0 or game.goal_number1 >= 20 or game.goal_number2 < 0 or game.goal_number2 >= 20)
    {
        cerr << "ERROR: Too large goal number (0 < GN < 20)" << endl;
        return false;
    }

    return true;
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

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cout << "ERROR: Input file isn't specified" << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.is_open())
        return 1;

    Table table;

    if(!parse_table(file, table))
        return 1;

    print_table(table);

    return 0;
}
