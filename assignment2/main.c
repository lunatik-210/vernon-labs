/********************************************************
* ADS HW Assignment 2
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/
/*
* Testing (input.txt)
* 1) First two simulations checks case sensitivity of keys name
* and sensitivity to the keys order. So it tests parse_params method.
*
* 2) Third simulation checks if green light = 0, so no cars 
* must be departed.
* As a consequence in the output:
* Maximum wait ~= Runtime = 5 minutes
*
* 3) Fourth simulation checks if an arrival rate = 0, so no single car
*  must be in a queue.
* As a consequence in the output:
* Average length:   0 cars
* Maximum length:   0 cars
* Average wait:     0 seconds
* Maximum wait:     0 seconds
*
* 4) The fifth one checks critical point so that all inputs are 0 
* numbers as well as outputs.
*/


#include "main.h"
#include "queue.h"

// Process errors and exit in case of exception
void process_error(error err)
{
    char* message = NULL;
    switch(err)
    {
        case SUCCESS:
            return;
        case ERROR_INCORRECT_CASE_NUMBER:
            message = "ERROR: Incorrect tournaments number (0 < N < 1000)";
            break;
        case ERROR_PARSED_INVALID_TOKEN:
            message = "ERROR: can't parse";
            break;
        case ERROR_KEY_CANT_BE_RECOGNIZED:
            message = "ERROR: can't recognize the key";
            break; 
        case ERROR_CANNT_OPEN_THE_FILE:
            message = "ERROR: File with the given input name cann't be oppened";
            break;
        default:
            message = "ERROR: unknown";
    }

    printf("%s\n", message);
    exit(err);
}

// Prases input file to fill a params structure
error parse_params(FILE* infile, Params* params)
{
    int value;
    char key[10];
    int i = 0;
    char *p;

    params->arrival = 0;
    params->departure = 0;
    params->runtime = 0;
    params->increment = 0;
    params->red = 0;
    params->green = 0;

    /*
    * Reads six key-value pairs from input file
    */

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

        // Corrects key to be non case sensitive

        for (p = key; *p != '\0'; ++p)
        {
            *p = tolower(*p);
        }

        // Saves key/value pairs

        if(strcmp("arrival", key) == 0)
        {
            params->arrival = value;
            continue;
        }

        if(strcmp("departure", key) == 0)
        {
            params->departure = value;
            continue;
        }

        if(strcmp("runtime", key) == 0)
        {
            params->runtime = value;
            continue;
        }

        if(strcmp("increment", key) == 0)
        {
            params->increment = value;
            continue;
        }

        if(strcmp("red", key) == 0)
        {
            params->red = value;
            continue;
        }

        if(strcmp("green", key) == 0)
        {
            params->green = value;
            continue;
        }

        return ERROR_KEY_CANT_BE_RECOGNIZED;
    }

    return SUCCESS;
}

// Prints params structure into outfile
void print_params(FILE *outfile, Params* params)
{
    fprintf(outfile, "Arrival rate:     %d cars per minute\n", params->arrival);
    fprintf(outfile, "Departure:        %d seconds per car\n", params->departure);
    fprintf(outfile, "Runtime:          %d minutes\n", params->runtime);
    fprintf(outfile, "Time increment:   %d milliseconds\n", params->increment);
    fprintf(outfile, "Light sequence:   Red %d seconds; Green %d seconds\n", params->red, params->green);
}

// Prints reqults structure into outfile
void print_results(FILE *outfile, Results* results)
{
    fprintf(outfile, "Average length:   %d cars\n", results->avg_len);
    fprintf(outfile, "Maximum length:   %d cars\n", results->max_len);
    fprintf(outfile, "Average wait:     %d seconds\n", results->avg_wait);
    fprintf(outfile, "Maximum wait:     %d seconds\n", results->max_wait);    
}

// Runs simulation of road intersection with traffic 
// lights, arriving and departing cars
void simulation(Params* params, Results* results)
{
    /*
    * Define all the necessary variables
    * (Translates all the inputs into milliseconds)
    */

    int i = 0;
    int wait_time = 0;

    int departure_msec = params->departure * 1000;
    int red_msec = params->red * 1000;
    int green_msec = params->green * 1000;
    int runtime_msec = params->runtime * 60000;
    int delta = params->increment;
    
    // Timer defines current time since the start of simulation
    // while tick defines the iteration number since the start
    int timer = 0;
    int tick = 0;

    // light_timer defines the time since the last traffic light state change
    int light_timer = 0;
    color light_state = RED;

    // departure_timer defines the time since the previous car was departured
    int departure_timer = 0;
    int departured_cars_number = 0;

    // mean is a variable for arrival car simulation 
    // (used by sample_poisson method )
    double mean = ((double)(delta) / 60000.0) * (double)(params->arrival);

    Queue *queue = qCreate();
    QCarElement car;

    results->avg_len = 0;
    results->max_len = 0;
    results->avg_wait = 0;
    results->max_wait = 0;

    /*
    * Starting simulation process
    */

    while(timer < runtime_msec)
    {
        /*
        * Switch the traffic light state if time has exceed
        */

        switch(light_state)
        {
            case GREEN:
                departure_timer+= delta;
                if(light_timer >= red_msec && red_msec > 0)
                {
                    departure_timer = 0;
                    light_timer = 0;
                    light_state = RED;
                }
                break;
            case RED:
                if(light_timer >= green_msec && green_msec > 0)
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

        /*
        * Increment all the time counters
        */
        
        timer += delta;
        light_timer += delta;
        tick += 1;

        /*
        * Simulation of arrival cars
        * (adding arrival cars to queue)
        */

        for(i = 0; i<sample_poisson(mean); ++i)
        {
            car.enter_time = timer;
            qPush(queue, car);
        }

        /*
        * Departure car if departure time has exceed 
        * and the traffic light state is GREEN
        */

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

        /*
        * Counting results
        */

        results->avg_len += qCount(queue);

        if(qCount(queue) > results->max_len)
        {
            results->max_len = qCount(queue);
        }
    }
    
    /*
    * Taking in account remaining cars in queue
    */

    while(0 != qCount(queue))
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

    if( tick > 0 )
    {
        results->avg_len /= tick;
    }
    
    if( departured_cars_number > 0 )
    {
        results->avg_wait /= departured_cars_number;
    }
    
    results->avg_wait /= 1000;
    results->max_wait /= 1000;

    qDestroy(queue);
}

// Poisson destribution is used to simulate a number 
// of arriving cars
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

    /*
    * Reads number of test simulations from the file
    */
    
    if(fscanf(infile, "%d", &N) != 1)
    {
        process_error(ERROR_PARSED_INVALID_TOKEN);
    }
    if(N <= 0 || N >= 1000)
    {
        process_error(ERROR_INCORRECT_CASE_NUMBER);
    }

    /*
    * Seed the randomizer
    */
    srand( (unsigned)time( NULL ) );

    /*
    * Runs N simulations and prints results into output file
    */
    fprintf(outfile, "Andrew Lapin\n");
    for(i = 0; i<N; ++i)
    {
        process_error(parse_params(infile, &params));
        simulation(&params, &results);
        print_params(outfile, &params);
        print_results(outfile, &results);
        fprintf(outfile, "\n");
    }

    fclose(infile);
    fclose(outfile);    

    return 0;
}
