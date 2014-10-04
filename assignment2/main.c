#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"

#include "queue.h"

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

void process_error(error err)
{
    char* mesage = NULL;
    switch(err)
    {
        case SUCCESS:
            return;
        case ERROR_INCORRECT_CASE_NUMBER:
            mesage = "ERROR: Incorrect tournaments number (0 < N < 1000)";
            break;
        case ERROR_PARSED_INVALID_TOKEN:
            mesage = "ERROR: can't parse";
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

error parse_params(FILE* infile, Params* params)
{
    int value;
    char key[10];
    int i = 0;

    params->arrival = 0;
    params->departure = 0;
    params->runtime = 0;
    params->increment = 0;
    params->red = 0;
    params->green = 0;

    for(i=0; i<6; ++i)
    {
        if(fscanf(infile, "%s", key) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(fscanf(infile, "%d", &value) != 1)
        {
            return ERROR_PARSED_INVALID_TOKEN;
        }

        if(strcmp("Arrival", key) == 0)
        {
            params->arrival = value;
            continue;
        }

        if(strcmp("Departure", key) == 0)
        {
            params->departure = value;
            continue;
        }

        if(strcmp("Runtime", key) == 0)
        {
            params->runtime = value;
            continue;
        }

        if(strcmp("Increment", key) == 0)
        {
            params->increment = value;
            continue;
        }

        if(strcmp("Red", key) == 0)
        {
            params->red = value;
            continue;
        }

        if(strcmp("Green", key) == 0)
        {
            params->green = value;
            continue;
        }
    }

    return SUCCESS;
}

void print_params(Params* params)
{
    printf("Arrival rate:    %d cars per minute\n", params->arrival);
    printf("Departure:       %d seconds per car\n", params->departure);
    printf("Runtime:         %d minutes\n", params->runtime);
    printf("Time increment:  %d milliseconds\n", params->increment);
    printf("Light sequence:  Red %d seconds; Green %d seconds\n", params->red, params->green);
}

void print_results(Results* results)
{
    printf("Average length:  %d cars\n", results->avg_len);
    printf("Maximum length:  %d cars\n", results->max_len);
    printf("Average wait:    %d seconds\n", results->avg_wait);
    printf("Maximum wait:    %d seconds\n", results->max_wait);    
}

void simulation(Params* params, Results* results)
{
    int i = 0;
    int wait_time = 0;

    int departure_msec = params->departure * 1000;
    int red_msec = params->red * 1000;
    int green_msec = params->green * 1000;
    int runtime_msec = params->runtime * 60000;
    int delta = params->increment;
    int timer = 0;

    int light_timer = 0;
    color light_state = GREEN;

    int departure_timer = 0;
    int departured_cars_number = 0;

    double mean = ((double)(delta) / 1000.0) * (double)(params->arrival);

    int tick = 0;

    Queue *queue = qCreate();
    QCarElement car;

    results->avg_len = 0;
    results->max_len = 0;
    results->avg_wait = 0;
    results->max_wait = 0;

    srand( (unsigned)time( NULL ) );

    while(timer < runtime_msec)
    {
        switch(light_state)
        {
            case GREEN:
                departure_timer+= delta;
                if(light_timer >= red_msec)
                {
                    departure_timer = 0;
                    light_timer = 0;
                    light_state = RED;
                }
                break;
            case RED:
                if(light_timer >= green_msec)
                {
                    if(qCount(queue) > 0)
                    {
                        car = qPop(queue);

                        wait_time = timer-car.enter_time;
                        results->avg_wait += wait_time;

                        if(wait_time > results->max_wait)
                        {
                            results->max_wait = wait_time;
                        }

                        departured_cars_number += 1;
                    }

                    light_timer = 0;
                    light_state = GREEN;
                }
                break;
        }
        
        timer += delta;
        light_timer += delta;
        tick += 1;

        for(i = 0; i<sample_poisson(mean); ++i)
        {
            car.enter_time = timer;
            qPush(queue, car);
        }

        if(light_state == GREEN && departure_timer >= departure_msec && qCount(queue) > 0)
        {
            car = qPop(queue);

            wait_time = timer-car.enter_time;
            results->avg_wait += wait_time;

            if(wait_time > results->max_wait)
            {
                results->max_wait = wait_time;
            }

            departure_timer = 0;

            departured_cars_number += 1;
        }

        results->avg_len += qCount(queue);

        if(qCount(queue) > results->max_len)
        {
            results->max_len = qCount(queue);
        }
    }

    results->avg_len /= tick;
    results->avg_wait /= departured_cars_number;
    
    results->avg_wait /= 1000;
    results->max_wait /= 1000;

    qDestroy(queue);
}

int sample_poisson(double mean)
{
    int count;
    double product;
    double zero_probability;

    count = 0;
    product = (double) rand() / (double) RAND_MAX;
    zero_probability = exp(-mean);

    while (product > zero_probability) 
    {
        count++;
        product = product * ((double) rand() / (double) RAND_MAX);
    }

    return count;
}


int main(int argc, char** argv)
{
    int N, i;

    Params params;
    Results results;

    FILE *infile = NULL;
    FILE *outfile = NULL;

    infile = fopen("input.txt", "r");
    outfile = fopen("output.txt", "w");

    if(infile == NULL)
    {
        process_error(ERROR_CANNT_OPEN_THE_FILE);
    }
    if(outfile == NULL)
    {
        process_error(ERROR_CANNT_OPEN_THE_FILE);
    }
    
    if(fscanf(infile, "%d", &N) != 1)
    {
        process_error(ERROR_PARSED_INVALID_TOKEN);
    }
    if(N <= 0 || N >= 1000)
    {
        process_error(ERROR_INCORRECT_CASE_NUMBER);
    }

    printf("Andrew Lapin\n\n");
    for(i = 0; i<N; ++i)
    {
        process_error(parse_params(infile, &params));
        simulation(&params, &results);
        print_params(&params);
        print_results(&results);
        printf("\n");
    }

    fclose(infile);
    fclose(outfile);    

    return 0;
}
