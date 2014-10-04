#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

typedef struct Params Params;

typedef struct Results Results;

typedef enum error error;

typedef enum color color;

struct Params
{
    int arrival;
    int departure;
    int runtime;
    int increment;
    int red;
    int green;
};

struct Results
{
    int avg_len;
    int max_len;
    int avg_wait;
    int max_wait;
};

enum error 
{
    SUCCESS = 0,
    ERROR_INCORRECT_CASE_NUMBER = 1,
    ERROR_PARSED_INVALID_TOKEN = 2,
    ERROR_CANNT_OPEN_THE_FILE = 3
};

enum color 
{
    GREEN = 0,
    RED = 1
};

void process_error(error err);

int sample_poisson(double mean);

error parse_params(FILE* infile, Params* params);

void print_params(Params* params);

void print_results(Results* results);
