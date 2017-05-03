//
// Created by osboxes on 11/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_MODEL_H
#define PATH_VELOCITY_PROFILER_MODEL_H

#include "VelocityProfilePoint.h"
#include "conf.h"

class Model {
protected:
    VelocityProfilePoint currentState;
    double timeStep = TIME_STEP;

public:
    Model();

    VelocityProfilePoint getState() const;
    void setState(const VelocityProfilePoint & state);

    virtual double getCriticalVelocity(double curvature) const = 0;
    virtual void stepForward(double u, double curvature) = 0;
    virtual void stepBackward(double u, double curvature) = 0;

    void stepForwardOptimal(double curvature);
    void stepBackwardOptimal(double curvature);
};



#endif //PATH_VELOCITY_PROFILER_MODEL_H
