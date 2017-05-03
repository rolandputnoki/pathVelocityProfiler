//
// Created by osboxes on 11/04/17.
//

#include "Model.h"
Model::Model()
        : currentState(0.0, 0.0)
{}

void Model::stepForwardOptimal(double curvature)
{
    stepForward(1, curvature);
}

void Model::stepBackwardOptimal(double curvature)
{
    stepBackward(-1, curvature);
}

VelocityProfilePoint Model::getState() const
{
    return currentState;
}

void Model::setState(const VelocityProfilePoint &state) {
    this->currentState = state;
}

