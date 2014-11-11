/********************************************************
* ADS HW Assignment 3 - Tourist Guide
* Andrew Lapin
* andrew.d.lapin@gmail.com
********************************************************/

/********************************************************
* Project include all the structures definitions and 
* methods declaration in touristguide.h as well as the
* implementation of those methods in touristguide.cpp.
********************************************************/

/********************************************************
/ Graphs which were used for testing
/********************************************************
*                         Graph 1
*
*      5     10     15
*  [1]---[2]----[3]----[4]
*
/********************************************************
*                         Graph 2
*
*      10     15     20
*  [1]----[2]----[3]----[4]
*   |                    |
*   |--------[5]---------|
*       5          25
*
/********************************************************
*                         Graph 3
*
*       25     30
*   [1]----[2]-----[3]
*    |              |
* 25 |              | 50
*    |  20     25   |
*   [8]----[9]-----[4]
*    |              |
* 30 |              | 10
*    |  35     70   |
*   [7]----[6]-----[5]
*
/********************************************************
* Testing
/********************************************************
* [Case 1] I just checked the critical condition when tere is
* only one town.
*
* [Case 2] Is based on "Graph 1". Here we check that for 
* one trip it would be possible to pass 4 passengers from 
* city 1 to 4 visiting 2 and 3.
*
* [Case 3] Is based on "Graph 2". The same as with previous
* one but also added cycle. So the algorithm chooses route
* 1-5-4 (in fact 1-5-4 and 1-2-3-4 have the same minimum
* number of trips)
*
* [Case 3] Is based on "Graph 2". Again extends previous
* case with increasing number of passengers = 15. In that
* case algorithm cann't go with the previous route it would  
* require more trips so it would choose route 1-2-3-4.
*
* [Case 4] Is based on "Graph 4". Here we have many possible
* routes to go but the most optimal one is 1-2-3-4-9-6-5.
*
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

        // fill grpah structure from file
        if(false == parse_input(in, g, t))
            return 1;

        // do some additional initializations
        prepare_graph(g, t);

        // calculating the results
        prcoess(g, t, resulst);

        // Output
        ++scenario_number;
        out << "Scenario #" << scenario_number << endl;
        out << "Minimum Number of Trips = " << resulst.trips_number << endl;
        out << "Route =";
        for(int i = 0; i<resulst.route.size(); ++i)
        {
            out  << " " << resulst.route[i];
        }
        out << endl << endl;
        ////////////////////
    }

    return 0;
}
