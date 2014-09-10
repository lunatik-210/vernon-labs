/********************************************************
* ADS HW Assignment 1
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

typedef struct Table Table;

typedef struct Tournament Tournament;

typedef struct Game Game;

typedef struct Result Result;

typedef enum error error;

// Process code error, print output and exit
void process_error(error err);

// Fill Table structure with parsed information from input sttream 
// Returns error if occur
error parse_file(FILE* infile, Table** table);

// Аrees memory of table with tournaments
void free_table(Table** table);

// Аrees memory of results array
void free_results(Result** results, int size);

// Output data stored in Table structure to the screen (terminal)
void print_table(Table* table);

// Helper function is used by calculate_results
void fill_result(Result* results, int index, int goal_number1, int goal_number2);

// Output data stored in Tournament structure to the screen (terminal)
void print_tournament_results(FILE *outfile, Tournament* tournament, Result* results);

// Process data stored in Tournament structure ot calculate resulting table
void calculate_results(Tournament* tournament, Result** results);

// Compare two results records with given i and j indexes
// Returns criterion condition
int criterion(Result* v, const int i, const int j);

// Swap two result records with given i and j indexes
void swap(Result* v, int i, int j);

// Implementation of quick sort (used to sort data stored in Result vector structure)
void _quick_sort(Result* v, int left, int right);

// Implementation of quick sort partition function to split array on two arrays with pivot element
// Returns pivot element
int _quick_sort_partition(Result* v, int left, int right);

// It's a help function which calls _quick_sort
void quick_sort(Result* v, int size);

// Implementation of bubble sort (used to sort data stored in Result vector structure)
void bubble_sort(Result* v, int size);
