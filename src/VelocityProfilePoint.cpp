//
// Created by osboxes on 03/04/17.
//

#include "VelocityProfilePoint.h"

VelocityProfilePoint::VelocityProfilePoint(double dist, double vel, double curv){
    this->distance = dist;
    this->velocity = vel;
    this->curvature = curv;
}

VelocityProfilePoint::VelocityProfilePoint(){
}

/*
void VelocityProfilePoint::pushVelocityValue(double vel) {
    velocityValues.push_back(vel);
}
*/

double VelocityProfilePoint::getVelocity() {
    return this->velocity;
}

VelocityProfilePoint::VelocityProfilePoint(double dist, double vel) {
    this->distance = dist;
    this->velocity = vel;
}