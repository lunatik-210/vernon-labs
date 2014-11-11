/********************************************************
* ADS HW Assignment 3 - Tourist Guide
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

#include "touristguide.h"


using namespace std;

// Parses input file and fills Graph structure
bool parse_input(ifstream& in, Graph& g, Task& t)
{
    int ncities, nroads;
    if(!in.eof())
    {
        in >> ncities >> nroads;
        if(ncities == 0 && nroads==0) return false;
        g.roads.resize(nroads);
        g.cities.resize(ncities);
    }
    else
    {
        return false;
    }

    int city1, city2, npassengers;
    for(int i = 0; i<nroads; ++i)
    {
        if(!in.eof())
        {
            in >> city1 >> city2 >> npassengers;
        }
        else
        {
            return false;
        }
        Road r = {city1, city2, npassengers, false};
        g.roads[i] = r;
        g.cities[city1-1].roads.push_back(i);
        g.cities[city2-1].roads.push_back(i);
    }

    in >> t.source_city >> t.destination_city >> t.tourists;
    return true;
}

// Initializes some graph fields with values by default
void prepare_graph(Graph& g, Task& t)
{
    for(int i = 0; i<g.roads.size(); ++i)
    {
        if( g.roads[i].weight > 0 )
        {
            g.roads[i].weight = int(ceil(float(t.tourists)/float(g.roads[i].weight-1)));
        }
        else
        {
            g.roads[i].weight = 0;
        }
    }

    for(int i = 0; i<g.cities.size(); ++i)
    {
        g.cities[i].came_from_road = -1;
        g.cities[i].came_from_city = -1;
    }
}

// Prints explored and unexplored nodes (used for debug)
void print_graph_status(const Graph& g)
{
    cout << "Unexplored roads:";
    for(int i = 0; i<g.roads.size(); ++i)
    {
        if(g.roads[i].explored == false)
        {
            cout << " " << i;
        }
    }
    cout << endl;
    cout << "Explored roads:";
    for(int i = 0; i<g.roads.size(); ++i)
    {
        if(g.roads[i].explored == true)
        {
            cout << " " << i;
        }
    }
    cout << endl;
}

// Prints list of routes to explore (used for debug)
void print_routes(const vector<Route>& routes)
{
    cout << "Routes: ";
    for(int i = 0; i<routes.size(); ++i)
    {
        cout << " (" << routes[i].city << "," << routes[i].road << ")";
    }
    cout << endl; 
}

// Prints entire the Graph structure (used for debug)
void print_graph(const Graph& g)
{
    for(int i = 0; i<g.roads.size(); ++i)
    {
        cout << "Road [" << i << "]: " 
             << g.roads[i].city1 << " "
             << g.roads[i].city2 << " "
             << g.roads[i].weight << endl;
    }

    cout << endl;

    for(int i = 0; i<g.cities.size(); ++i)
    {
        cout << "City [" << i+1 << "]: ";

        for(int j = 0; j<g.cities[i].roads.size(); ++j)
        {
            cout << g.cities[i].roads[j] << " ";
        }

        cout << "(" << g.cities[i].came_from_city << "|" << g.cities[i].came_from_road << "|" << g.roads[g.cities[i].came_from_road].weight << ")";

        cout << endl;
    }

    cout << endl;
}

// Performs one iteration of bubbling of an array element
// (is used to keep routes list sorted after adding new element)
void bubble(vector<Route>& routes, const Graph& g)
{
    if(routes.size()<2)
        return;

    for(int i = 0; i<routes.size()-1; ++i)
    {
        if(g.roads[routes[i].road].weight < g.roads[routes[i+1].road].weight)
        {
            Route buff = routes[i];
            routes[i] = routes[i+1];
            routes[i+1] = buff;
        }
    }
}

// Returs the id of neighbor city by given route
int get_neighbor(Route route, const Graph& g)
{
    if(g.roads[route.road].city1 == route.city)
    {
        return g.roads[route.road].city2;
    }
    return g.roads[route.road].city1;
}

// Chooses the route with the higher bandwidth from routes,
// marks the city and road from where we came (used later for traversing)
int explore_route(vector<Route>& routes, Graph& g)
{
    if(routes.empty())
        return -1;

    Route route = routes.back();
    routes.pop_back();

    int neighbor_city = get_neighbor(route, g);
    int road_id_old = g.cities[neighbor_city-1].came_from_road;

    if(road_id_old == -1)
    {
        g.cities[neighbor_city-1].came_from_road = route.road;
        g.cities[neighbor_city-1].came_from_city = route.city;
    }
    else
    {
        Road edge_new = g.roads[route.road];
        Road edge_old = g.roads[road_id_old];
        if(edge_new.weight < edge_old.weight)
        {
            g.cities[neighbor_city-1].came_from_road = route.road;
            g.cities[neighbor_city-1].came_from_city = route.city;
        }
    }

    return neighbor_city;
}

// Verfifies if we reach the goal
bool is_goal(int city, const Task& t)
{
    return (city == t.destination_city);
}

// Reverses list
void reverse(vector<int>& route)
{
    for(int i = 0; i<route.size()/2; ++i)
    {
        int j = route.size() - 1 - i;
        int buff = route[i];
        route[i] = route[j];
        route[j] = buff;
    }
}

// Retrive the route which we should follow and fill Results structure
void traverse_route(const Graph& g, const Task& t, Results& resulst)
{
    if(t.destination_city == t.source_city || g.cities.size() == 1 || g.roads.size() == 0)
    {
        resulst.route.push_back(t.destination_city);
        resulst.trips_number = 0;
        return;
    }

    int city = t.destination_city;
    resulst.trips_number = g.roads[g.cities[city-1].came_from_road].weight;

    while(city!=t.source_city && city!=-1)
    {
        resulst.route.push_back(city);
        city = g.cities[city-1].came_from_city;

        if(g.cities[city-1].came_from_road != -1 && resulst.trips_number < g.roads[g.cities[city-1].came_from_road].weight)
            resulst.trips_number = g.roads[g.cities[city-1].came_from_road].weight;
    }
    
    resulst.route.push_back(city);
    reverse(resulst.route);
}

// Explores new city adding new roads to the routes
void explore_city_roads(int city, vector<Route>& routes, Graph& g)
{
    for(int i = 0; i<g.cities[city-1].roads.size(); ++i)
    {
        int road_id = g.cities[city-1].roads[i];
        if(g.roads[road_id].explored == false)
        {
            g.roads[road_id].explored = true;
            Route r = {city, road_id};
            routes.push_back(r);
            bubble(routes, g);
        }
    }
}

// Runs the main process of finding the route
bool prcoess(Graph& g, Task& t, Results& resulst)
{
    // routes - used to track all to-explore roads
    vector<Route> routes;
    int came_from_city = t.source_city;

    bool is_found;

    while(false == (is_found = is_goal(came_from_city, t)))
    {
        // adding new roads to routes list
        explore_city_roads(came_from_city, routes, g);

        // moving to the next city
        came_from_city = explore_route(routes, g);
    }

    if(is_found)
    {
        traverse_route(g, t, resulst);
        return true;
    }

    return false;
}
