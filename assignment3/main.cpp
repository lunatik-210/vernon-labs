/********************************************************
* ADS HW Assignment 3 - Tourist Guide
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

#include "touristguide.h"


using namespace std;


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
        out << endl << endl;
    }

    return 0;
}
