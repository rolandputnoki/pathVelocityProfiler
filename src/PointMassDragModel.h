//
// Created by osboxes on 02/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_POINTMASSDRAGMODEL_H
#define PATH_VELOCITY_PROFILER_POINTMASSDRAGMODEL_H

#include "conf.h"
#include "Model.h"


class PointMassDragModel : public Model
{
    double tangAccelMax = TANG_ACCEL_MAX;
    double tangAccelMin = TANG_ACCEL_MIN;
    double normalAccelMax = NORMAL_ACCEL_MAX;
    double dragCoef = DRAG_COEF;
    double maxVelocity = MAX_VELOCITY;
public:
    double getCriticalVelocity(double curvature) const;
    void stepForward(double u, double curvature);
    void stepBackward(double u, double curvature);
private:
    double getMaxTangentialAccel(double u) const;
    VelocityProfilePoint calculateDelta(double u, double curvature) const;
//    std::pair<double, double> calculateDelta(double u, double curvature) const;
};


#endif //PATH_VELOCITY_PROFILER_POINTMASSDRAGMODEL_H
