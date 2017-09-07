//
// Created by osboxes on 02/04/17.
//

#include "VelocityProfileBuilder.h"


using namespace path_sampler;




void VelocityProfileBuilder::setModel(std::unique_ptr<Model> model){
    this->model = std::move(model);
}



double VelocityProfileBuilder::getEndSpeed() {
    return this->finalSpeed;
}


VelocityProfileBuilder::VelocityProfileBuilder(SampledSection sampledSection) {
    int path_lenght = sampledSection.sample_points.size();
//    PointMassDragModel pmd;
//    this->setModel(pmd);
    this->setModel(std::make_unique<PointMassDragModel>());
    this->finalSpeed = 0;
    this->startSpeed = 0;
    for(int i=0; i< path_lenght; i++ ){
        double curv = sampledSection.sample_points[i].curvature;
        double max_vel = this->model->getCriticalVelocity(curv);
        double distance = sampledSection.sample_points[i].position;
        VelocityProfilePoint vpp(distance,max_vel,curv);
        this->maxVelProfile.pushBackPoint(vpp);
    }
}

/*
void VelocityProfileBuilder::setModel(PointMassDragModel pmd) {
    this->model = pmd;
}
*/


VelocityProfileBuilder::VelocityProfileBuilder(std::string filename) {
    startSpeed = 0;
    finalSpeed = 0;
    FILE *fp;
    fp = fopen(filename.c_str(),"r");
    double distance, radius,velocity;
    while( fscanf(fp, "%lf,%lf\n", &distance, &radius) != EOF){
        velocity = this->model->getCriticalVelocity(1/radius);
        VelocityProfilePoint v(distance, velocity, (1/radius));
//        VelocityProfilePoint c(distance, 1/radius);
//        this->curvature.pushBackPoint(c);
        this->maxVelProfile.pushBackPoint(v);
    }
}




/*
//Beállítja a kezdőpont beli sebességet
void VelocityProfileBuilder::setStartVelocity(double startSpeed)
{
    this->startSpeed = startSpeed;
}

//Beállítja a végpont beli sebességet
void VelocityProfileBuilder::setEndVelocity(double finalSpeed)
{
    this->finalSpeed = finalSpeed;
}

//a_cp + a_t = 1, most legyen a_cp maximális, tehát 1
//Ezért v = gyök(1/görbület)
double VelocityProfileBuilder::getCriticalSpeed(double position)
{
    return std::sqrt(std::fabs(1/curvature.getCurvature(position)));
}*/

void VelocityProfileBuilder::collectLocalMinima() {
    localMinima.clear();
    localMinima.emplace_back(maxVelProfile.getFirstDistance(), startSpeed);
    for(int i = 0; i<maxVelProfile.getLength(); i++){
        if(maxVelProfile.isLocalMinimum(i)){
            localMinima.emplace_back(maxVelProfile.getPosAtIndex(i),maxVelProfile.getMaxVelAtIndex(i));
        }
    }
    localMinima.emplace_back(maxVelProfile.getLastDistance(), finalSpeed);
}

void VelocityProfileBuilder::buildSubProfileForward(size_t index)
{
    VelocityProfile& subProfile = subProfiles.back();
    model->setState(localMinima[index]);
    for(size_t i = index; i < localMinima.size()-1; i++) {
        while(model->getState().distance < localMinima[i+1].distance ) {
            subProfile.addProfilePoint(model->getState());
            model->stepForwardOptimal(maxVelProfile.getCurvature(model->getState().distance));
        }

        if(model->getState().velocity >= localMinima[i+1].velocity)
            break;
    }
}

void VelocityProfileBuilder::buildSubProfileBackward(size_t index)
{
    VelocityProfile& subProfile = subProfiles.back();
    model->setState(localMinima[index]);
    for(size_t i = index; i > 0; i--) {
        while(model->getState().distance > localMinima[i-1].distance ) {
            subProfile.addProfilePoint(model->getState());
            model->stepBackwardOptimal(maxVelProfile.getCurvature(model->getState().distance));
        }

        if(model->getState().velocity >= localMinima[i-1].velocity)
            break;
    }
    subProfile.invert();
}

void VelocityProfileBuilder::buildSubProfile(size_t index)
{
    subProfiles.emplace_back();
    buildSubProfileForward(index);
    subProfiles.emplace_back();
    buildSubProfileBackward(index);
}


void VelocityProfileBuilder::build()
{
    VelocityProfile profile;
    collectLocalMinima();
    for(size_t i = 0; i < localMinima.size(); i++) {
//        buildSubProfile(i);
        if(i == 0){
            subProfiles.emplace_back();
            buildSubProfileForward(i);
//            subProfiles.back().copyPointsToContainer();
//            profile.appendProfileByMin(subProfiles.back());
            profile.appendProfileByMin2(subProfiles.back());
        } else if(i == ((size_t)localMinima.size() - 1)){
            subProfiles.emplace_back();
            buildSubProfileBackward(i);
 //           subProfiles.back().copyPointsToContainer();
 //           profile.appendProfileByMin(subProfiles.back());
            profile.appendProfileByMin2(subProfiles.back());
        } else {
            subProfiles.emplace_back();
            buildSubProfileBackward(i);
//            subProfiles.back().copyPointsToContainer();
//            profile.appendProfileByMin(subProfiles.back());
            profile.appendProfileByMin2(subProfiles.back());

//            profile.writePointsToFile(i+5000);
            subProfiles.emplace_back();
            buildSubProfileForward(i);
 //           subProfiles.back().copyPointsToContainer();
//            profile.appendProfileByMin(subProfiles.back());
            profile.appendProfileByMin2(subProfiles.back());
        }

    }

    for(size_t i = 0; i< subProfiles.size(); i++){
        subProfiles[i].writePointsToFile(i);
    }
    finalProfile = profile;
    finalProfile.calculateTime();

}


void VelocityProfileBuilder::mergeWithMaxVelProfile() {
    double curVel,maxVel;
    for(size_t i = 0; i<finalProfile.getLength(); i++){
        maxVel = maxVelProfile.getVelocity2(finalProfile.getPosAtIndex(i));
        curVel = finalProfile.getVelAt(i);
        if(curVel > maxVel){
            finalProfile.changeVelAt(i,maxVel);
        }

    }
}