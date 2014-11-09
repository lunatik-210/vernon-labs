#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Task 
{
    int source_city;
    int destination_city;
    int tourists;
};

struct Road 
{
    int city1;
    int city2;
    int weight;
    bool explored;
};

struct City 
{
    int came_from_road;
    int came_from_city;
    vector<int> roads;
};

struct Graph 
{
    vector<Road> roads;
    vector<City> cities;
};

struct Route
{
    int city;
    int road;
};

struct Results
{
    vector<int> route;
    int trips_number;
};

bool parse_input(ifstream& in, Graph& g, Task& t);

void prepare_graph(Graph& g, Task& t);

void print_graph_status(const Graph& g);

void print_routes(const vector<Route>& routes);

void print_graph(const Graph& g);

void sort(vector<Route>& routes, const Graph& g);

int get_neighbor(Route route, const Graph& g);

int explore_route(vector<Route>& routes, Graph& g);

bool is_goal(int city, const Task& t);

void reverse(vector<int>& route);

void traverse_route(const Graph& g, const Task& t, Results& resulst);

void explore_city_roads(int city, vector<Route>& routes, Graph& g);

bool prcoess(Graph& g, Task& t, Results& resulst);


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

void print_routes(const vector<Route>& routes)
{
    cout << "Routes: ";
    for(int i = 0; i<routes.size(); ++i)
    {
        cout << " (" << routes[i].city << "," << routes[i].road << ")";
    }
    cout << endl; 
}

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

void sort(vector<Route>& routes, const Graph& g)
{
    for(int i = 0; i<routes.size(); ++i)
    {
        for(int j = 0; j<routes.size()-i-1; ++j)
        {
            if(g.roads[routes[j].road].weight < g.roads[routes[j+1].road].weight)
            {
                Route buff = routes[j];
                routes[j] = routes[j+1];
                routes[j+1] = buff;
            }
        }
    }
}

int get_neighbor(Route route, const Graph& g)
{
    if(g.roads[route.road].city1 == route.city)
    {
        return g.roads[route.road].city2;
    }
    return g.roads[route.road].city1;
}

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

bool is_goal(int city, const Task& t)
{
    if(city == t.destination_city)
    {
        return true;
    }
}

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

void traverse_route(const Graph& g, const Task& t, Results& resulst)
{
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
        }
    }
}

bool prcoess(Graph& g, Task& t, Results& resulst)
{
    vector<Route> routes;
    int came_from_city = t.source_city;

    bool is_found;

    while(false == (is_found = is_goal(came_from_city, t)))
    {
        explore_city_roads(came_from_city, routes, g);
        sort(routes, g);
        came_from_city = explore_route(routes, g);
    }

    if(is_found)
    {
        traverse_route(g, t, resulst);
        return true;
    }

    return false;
}

int main(int aegc, char** argv)
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    int scenario_number = 0;
    out << "Andrew Lapin" << endl;

    while(!in.eof())
    {
        Graph g;
        Task t;
        Results resulst;

        if(false == parse_input(in, g, t))
            return 1;

        prepare_graph(g, t);
        prcoess(g, t, resulst);

        ++scenario_number;
        out << "Scenario #" << scenario_number << endl;
        out << "Minimum Number of Trips = " << resulst.trips_number << endl;
        out << "Route =";
        for(int i = 0; i<resulst.route.size(); ++i)
        {
            out  << " " << resulst.route[i];
        }
        out << endl;
    }

    return 0;
}
