#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <cmath>

using namespace std;

struct Task {
    int source_city;
    int destination_city;
    int tourists;
};

struct Edge {
    int node1;
    int node2;
    int weight;
    bool explored;
};

struct Node {
    int edge_id;
    set<int> edges;
};

struct Graph {
    vector<Edge> edges;
    vector<Node> nodes;
};

bool parse_input(ifstream& in, Graph& g, Task& t)
{
    int ncities, nroads;
    if(!in.eof())
    {
        in >> ncities >> nroads;
        if(ncities == 0 && nroads==0) return false;
        g.edges.resize(nroads);
        g.nodes.resize(ncities);
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
        Edge e = {city1, city2, npassengers, false};
        g.edges[i] = e;
        g.nodes[city1-1].edges.insert(i);
        g.nodes[city2-1].edges.insert(i);
    }

    in >> t.source_city >> t.destination_city >> t.tourists;
    return true;
}

void prepare_graph(Graph& g, Task& t)
{
    for(int i = 0; i<g.edges.size(); ++i)
    {
        if( g.edges[i].weight > 0 )
        {
            g.edges[i].weight = int(ceil(float(t.tourists+1)/float(g.edges[i].weight)));
        }
        else
        {
            g.edges[i].weight = 0;
        }
    }

    for(int i = 0; i<g.nodes.size(); ++i)
    {
        g.nodes[i].edge_id = -1;
    }
}

void print_graph_status(const Graph& g)
{
    cout << "Unexplored edges:";
    for(int i = 0; i<g.edges.size(); ++i)
    {
        if(g.edges[i].explored == false)
        {
            cout << " " << i;
        }
    }
    cout << endl;
    cout << "Explored edges:";
    for(int i = 0; i<g.edges.size(); ++i)
    {
        if(g.edges[i].explored == true)
        {
            cout << " " << i;
        }
    }
    cout << endl;
}

void print_v(const vector<int>& v)
{
    for(int i = 0; i<v.size(); ++i)
    {
        cout << " " << v[i];
    }
    cout << endl;    
}

void print_to_explore(const vector<int>& to_explore)
{
    cout << "To explore:";
    print_v(to_explore);
}

void print_route(const vector<int>& route)
{
    cout << "Route: ";
    print_v(route);
    cout << "Size: " << route.size() << endl;
}

void print_graph(const Graph& g)
{
    for(int i = 0; i<g.edges.size(); ++i)
    {
        cout << "Edge [" << i << "]: " 
             << g.edges[i].node1 << " "
             << g.edges[i].node2 << " "
             << g.edges[i].weight << endl;
    }

    cout << endl;

    for(int i = 0; i<g.nodes.size(); ++i)
    {
        cout << "Node [" << i+1 << "]: ";

        for (std::set<int>::iterator it = g.nodes[i].edges.begin(); it != g.nodes[i].edges.end(); ++it)
        {
            cout << *it << " ";
        }

        cout << endl;
    }

    cout << endl;
}

void sort(vector<int>& to_explore, const Graph& g)
{
    for(int i = 0; i<to_explore.size(); ++i)
    {
        for(int j = 0; j<to_explore.size()-i-1; ++j)
        {
            if(g.edges[to_explore[j]].weight < g.edges[to_explore[j+1]].weight)
            {
                int buff = to_explore[j];
                to_explore[j] = to_explore[j+1];
                to_explore[j+1] = buff;
            }
        }
    }
}

void add_to_explore(int source_city, vector<int>& to_explore, Graph& g)
{
    for (std::set<int>::iterator it = g.nodes[source_city-1].edges.begin(); it != g.nodes[source_city-1].edges.end(); ++it)
    {
        int edge_id = *it;
        if(g.edges[edge_id].explored == false)
        {
            g.edges[edge_id].explored = true;
            to_explore.push_back(edge_id);
        }
    }    
}

int get_neighbor(int city, int edge_id, const Graph& g)
{
    if(g.edges[edge_id].node1 == city)
    {
        return g.edges[edge_id].node2;
    }

    return g.edges[edge_id].node1;
}

int explore_route(int came_from_city, vector<int>& to_explore, Graph& g)
{
    if(to_explore.empty())
        return -1;

    int edge_id = to_explore.back();
    to_explore.pop_back();

    int edge_id_old = g.nodes[came_from_city-1].edge_id;

    if(edge_id_old == -1)
    {
        g.nodes[came_from_city-1].edge_id = edge_id;
    }
    else
    {
        Edge edge_new = g.edges[edge_id];
        Edge edge_old = g.edges[edge_id_old];
        if(edge_new.weight < edge_old.weight)
        {
            g.nodes[came_from_city-1].edge_id = edge_id;
        }
    }

    return get_neighbor(came_from_city, edge_id, g);
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

void traverse_route(const Graph& g, const Task& t, vector<int>& route)
{
    int city = t.destination_city;

    while(city!=t.source_city)
    {
        cout << city << endl;
        route.push_back(city);
        city = get_neighbor(city, g.nodes[city-1].edge_id, g);   
    }
}

void prcoess(Graph& g, Task& t)
{
    vector<int> to_explore;
    int came_from_city = t.source_city;

    /*
    print_graph_status(g);
    print_to_explore(to_explore);
    cout << "---" << endl;
    */
    
    bool is_found;

    while(false == (is_found = is_goal(came_from_city, t)))
    {
        add_to_explore(came_from_city, to_explore, g);
        sort(to_explore, g);
        print_to_explore(to_explore);
        came_from_city = explore_route(came_from_city, to_explore, g);
        if(came_from_city == -1) break;
        print_graph_status(g);
        cout << "---" << endl;
    }

    vector<int> route;

    if(is_found)
    {
        cout << "--RESULT--" << endl;
        //traverse_route(g, t, route);
        print_route(route);
    }

    /*
    print_graph_status(g);
    print_to_explore(to_explore);
    cout << "---" << endl;

    cout << to_explore.back() << endl;
    to_explore.pop_back();
    print_to_explore(to_explore);
    */
}

int main(int aegc, char** argv)
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    while(!in.eof())
    {
        Graph g;
        Task t;

        if( false == parse_input(in, g, t) )
            return 1;

        prepare_graph(g, t);
        //print_graph(g);
        prcoess(g, t);
    }

    return 0;
}
