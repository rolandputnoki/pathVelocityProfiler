//
// Created by osboxes on 02/04/17.
//

#include "PointMassDragModel.h"


#include <cmath>

//Kiszámolja az adott görbülethez tartozó maximális megengedhető sebességet
// a_cp = v^2/r = v^2*curv  ---> v = gyök(a_cp/curv)
double PointMassDragModel::getCriticalVelocity(double curvature) const
{
    double maxVel = std::sqrt(normalAccelMax / std::fabs(curvature));
    return (maxVel > maxVelocity) ? maxVelocity : maxVel;
}

//Kiszámolja a maximális tangenciális gyorsulást
double PointMassDragModel::getMaxTangentialAccel(double u) const
{
    double velocitySquare =  currentState.velocity * currentState.velocity;
    if(u > 0) {
        return tangAccelMax - dragCoef * velocitySquare; //doksiból képlet 167. oldal
    } else {
        return tangAccelMin - dragCoef * velocitySquare;
    }
}


void PointMassDragModel::stepForward(double u, double curvature)
{
    auto delta = calculateDelta(u, curvature);
    currentState.distance += delta.distance;
    currentState.velocity += delta.velocity;
    if(currentState.velocity > maxVelocity){
        currentState.velocity = maxVelocity;
    }
}

void PointMassDragModel::stepBackward(double u, double curvature)
{
    auto delta = calculateDelta(u, curvature);
    currentState.distance -= delta.distance;
    currentState.velocity -= delta.velocity;
    if(currentState.velocity > maxVelocity){
        currentState.velocity = maxVelocity;
    }
}


//VelocityProfilePoint visszatérési értékkel
VelocityProfilePoint PointMassDragModel::calculateDelta(double u, double curvature) const
{
    VelocityProfilePoint delta;
    delta.distance = currentState.velocity * timeStep;

    double atMaxSq = getMaxTangentialAccel(u);
    atMaxSq *= atMaxSq;

    double anMaxSq = normalAccelMax * normalAccelMax;

    double anSq = currentState.velocity * currentState.velocity * curvature;
    anSq *= anSq;

    if(anMaxSq > anSq) {
        delta.velocity = u * std::sqrt(atMaxSq - (atMaxSq / anMaxSq) * anSq) * timeStep;
    } else {
        delta.velocity = 0;
    }
    return delta;
}

/*
std::pair<double, double> PointMassDragModel::calculateDelta(double u, double curvature) const
{
    std::pair<double, double> delta;
    delta.first = currentState.velocity * timeStep;

    double atMaxSq = getMaxTangentialAccel(u);
    atMaxSq *= atMaxSq;

    double anMaxSq = normalAccelMax * normalAccelMax;

    double anSq = currentState.velocity * currentState.velocity * curvature;
    anSq *= anSq;

    if(anMaxSq > anSq) {
        delta.second = u * std::sqrt(atMaxSq - (atMaxSq / anMaxSq) * anSq) * timeStep;
    } else {
        delta.second = 0;
    }
    return delta;
}
 */