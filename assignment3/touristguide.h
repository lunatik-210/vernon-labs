/********************************************************
* ADS HW Assignment 3 - Tourist Guide
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

#ifndef TOURISTGUIDE_H
#define TOURISTGUIDE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

// Represents the source and the destination cities
// as well as the amount of passengers/tourists to carry
struct Task 
{
    int source_city;
    int destination_city;
    int tourists;
};

// Represents an edge of the graph. It connects two cities 
// together and provides weight/bandwidth of road per trip.
// Explored flag helps to track if we have already explored 
// such a road (used for simplicity in the algorithm).
struct Road 
{
    int city1;
    int city2;
    int weight;
    bool explored;
};

// Represnet the node of the graph. It stores all the roads
// which are outging from the city as well as two helpers
// variables to store from which previos city and road we came
// (used for simplicity in the algorithm).
struct City 
{
    int came_from_road;
    int came_from_city;
    std::vector<int> roads;
};

// Stores list of roads and cities/connections between roads.
struct Graph 
{
    std::vector<Road> roads;
    std::vector<City> cities;
};

// Route structure is used to track roads to explore.
// For simplicity it also includes city id from which 
// we explored this road. (See prcoess method).
struct Route
{
    int city;
    int road;
};

// Uses to represent the results. The best route
// and minimal number of trips. 
struct Results
{
    std::vector<int> route;
    int trips_number;
};

// Parses input file and fills Graph structure
bool parse_input(std::ifstream& in, Graph& g, Task& t);

// Initializes some graph fields with values by default
void prepare_graph(Graph& g, Task& t);

// Prints explored and unexplored nodes (used for debug)
void print_graph_status(const Graph& g);

// Prints list of routes to explore (used for debug)
void print_routes(const std::vector<Route>& routes);

// Prints entire the Graph structure (used for debug)
void print_graph(const Graph& g);

// Implements bubble sort algorithm for routes
// It's used for the reason that the last element of
// the list should always be with the higher bandwidth
void sort(std::vector<Route>& routes, const Graph& g);

// Returs the id of neighbor city by given route
int get_neighbor(Route route, const Graph& g);

// Chooses the route with the higher bandwidth from routes,
// marks the city and road from where we came (used later for traversing)
int explore_route(std::vector<Route>& routes, Graph& g);

// Verfifies if we reach the goal
bool is_goal(int city, const Task& t);

// Reverses list
void reverse(std::vector<int>& route);

// Retrive the route which we should follow and fill Results structure
void traverse_route(const Graph& g, const Task& t, Results& resulst);

// Explores new city adding new roads to the routes
void explore_city_roads(int city, std::vector<Route>& routes, Graph& g);

// Runs the main process of finding the route
bool prcoess(Graph& g, Task& t, Results& resulst);


#endif // TOURISTGUIDE_H
