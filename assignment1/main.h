#include <string>
#include <vector>
#include <fstream>

struct Table;
struct Tournament;
struct Game;
struct Result;

struct Table 
{
    int tournament_number;
    std::vector<Tournament> tournaments;
};

struct Tournament 
{
    std::string name;

    int team_number;
    std::vector<std::string> team_names;

    int game_number;
    std::vector<Game> game;
};

struct Game 
{
    std::string team_name1;
    int goal_number1;

    std::string team_name2;
    int goal_number2;
};

struct Result 
{
    std::string team_name;

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

// Transforms a string into integer variable
// Returns integer variable
int to_int(std::string str);

// Get next line from input stream
// Returns true if next line exists
bool get_string(std::ifstream& file, std::string& line);

// Fill Table structure with parsed information from input sttream 
// Returns error if occur
error parse_table(std::ifstream& file, Table& table);

// Fill Tournament structure with parsed information from input sttream
// Returns error if occur
error parse_tournament(std::ifstream& file, Tournament& table);

// Fill Game structure with parsed information from input sttream
// Returns error if occur
error parse_game(std::string& str, Game& game);

// Output data stored in Table structure to the screen (terminal)
void print_table(const Table& table);

// Output data stored in Tournament structure to the screen (terminal)
void print_tournament_results(const Tournament& tournament, const std::vector<Result>& results);

// Process data stored in Tournament structure ot calculate resulting table
void calculate_results(const Tournament& tournament, std::vector<Result>& results);

// Implementation of bubble sort (used to sort data stored in Result vector structure)
void bubble_sort(std::vector<Result>& v);

// Compare two results records with given i and j indexes
// Returns criterion condition
bool criterion(const std::vector<Result>& v, const int i, const int j);

// Process code error, print output and exit
void process_error(error err);

// It's a help function which calls _quick_sort
void quick_sort(std::vector<Result>& v);

// Implementation of quick sort (used to sort data stored in Result vector structure)
void _quick_sort(std::vector<Result>& v, int left, int right);

// Implementation of quick sort partition function to split array on two arrays with pivot element
// Returns pivot element
int _quick_sort_partition(std::vector<Result>& v, int left, int right);

// Swap two result records with given i and j indexes
void swap(std::vector<Result>& v, int i, int j);

// Helper function is used by calculate_results
void fill_result(Result& r, int goal_number1, int goal_number2);
