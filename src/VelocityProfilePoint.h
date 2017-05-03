//
// Created by osboxes on 03/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_VELOCITYPROFILEPOINT_H
#define PATH_VELOCITY_PROFILER_VELOCITYPROFILEPOINT_H

#include <vector>
#include <sstream>

class VelocityProfilePoint {

public:
    double distance;
    double velocity;
    double curvature;
    double time;
    VelocityProfilePoint();
    VelocityProfilePoint(double dist, double vel, double curv);
    VelocityProfilePoint(double dist, double vel);
    double getDistance();
//    void pushVelocityValue(double vel);
    double getVelocity();


};


#endif //PATH_VELOCITY_PROFILER_VELOCITYPROFILEPOINT_H
