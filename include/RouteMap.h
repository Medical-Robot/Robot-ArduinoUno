#pragma once
#include <StandardCplusplus.h>
#include <vector>

struct RouteMapWaypoint{
    int ID;
    int ID_next_front_waypoint;
    int ID_next_right_waypoint;
    int ID_next_left_waypoint;
    int ID_next_back_waypoint;
};

class RouteMap
{
private:
    std::vector<RouteMapWaypoint> route;
    /* data */
public:
    RouteMap(/* args */);
    ~RouteMap();
};


