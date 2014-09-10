
typedef struct Table Table;

typedef struct Tournament Tournament;

typedef struct Game Game;

typedef struct Result Result;

typedef enum error error;

void process_error(error err);

error parse_file(FILE* infile, Table** table);

void free_table(Table** table);

void print_table(Table* table);

void fill_result(Result* results, int index, int goal_number1, int goal_number2);

void print_tournament_results(Tournament* tournament, Result* results);

void calculate_results(Tournament* tournament, Result** results);

void free_results(Result** results, int size);

int criterion(Result* v, const int i, const int j);

void swap(Result* v, int i, int j);

void _quick_sort(Result* v, int left, int right);

int _quick_sort_partition(Result* v, int left, int right);

void quick_sort(Result* v, int size);

void bubble_sort(Result* v, int size);