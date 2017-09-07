//
// Created by osboxes on 02/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_VELOCITYPROFILEBUILDER_H
#define PATH_VELOCITY_PROFILER_VELOCITYPROFILEBUILDER_H


#include <ros/ros.h>
#include "path_sampler/SampledPath.h"
#include "path_sampler/SampledSection.h"
#include "VelocityProfilePoint.h"
#include "VelocityProfile.h"
#include "PointMassDragModel.h"
#include <vector>
#include <memory>
#include "Model.h"


using namespace path_sampler;

class VelocityProfileBuilder {
    std::unique_ptr<Model> model;
//    PointMassDragModel model;
    double startSpeed;
    double finalSpeed;
    std::vector<VelocityProfilePoint> localMinima;
public:
    VelocityProfile maxVelProfile;
    VelocityProfile finalProfile;
    std::vector<VelocityProfile> subProfiles;
    VelocityProfileBuilder();
    VelocityProfileBuilder(std::string filename);
    VelocityProfileBuilder(SampledSection sampledSection);
    void setModel(std::unique_ptr<Model> model);
//    void setModel(PointMassDragModel pmd);
    void collectLocalMinima();
    double getStartSpeed();
    double getEndSpeed();
    int getLastPointIndex();
    void buildSubProfileForward(size_t index);
    void buildSubProfileBackward(size_t index);
    void buildSubProfile(size_t index);
    void build();

    void mergeWithMaxVelProfile();
};


#endif //PATH_VELOCITY_PROFILER_VELOCITYPROFILEBUILDER_H
