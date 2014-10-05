/********************************************************
* ADS HW Assignment 2
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

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
    ERROR_KEY_CANT_BE_RECOGNIZED = 3,
    ERROR_CANNT_OPEN_THE_FILE = 4
};

// Traffic light color
enum color 
{
    GREEN = 0,
    RED = 1
};

// Process errors and exit in case of exception
void process_error(error err);

// Prases input file to fill a params structure
error parse_params(FILE* infile, Params* params);

// Prints params structure into outfile
void print_params(FILE *outfile, Params* params);

// Prints reqults structure into outfile
void print_results(FILE *outfile, Results* results);

// Runs simulation of road intersection with traffic 
// lights, arriving and departing cars
void simulation(Params* params, Results* results);

// Poisson destribution is used to simulate a number 
// of arriving cars
int sample_poisson(double mean);
