//
// Created by osboxes on 02/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_VELOCITYPROFILE_H
#define PATH_VELOCITY_PROFILER_VELOCITYPROFILE_H

#include "VelocityProfilePoint.h"
#include "ProfileCrossSection.h"
#include <deque>

class VelocityProfile {
    std::vector<VelocityProfilePoint> profilePoints;
public:
	//Nem használt
    int getLastPointIndex();
    std::vector<VelocityProfilePoint> getPointsVector();
	//Egy pontot szúr a vektor végére
    void pushBackPoint(VelocityProfilePoint p);
	//Megadja a maxVelProfileból a sebességet az adott indexedik pontnál
    double getMaxVelAtIndex(int index);
	//
    double getPosAtIndex(int index) const;
	//
    void writePointsToFile(size_t i);
	//
    size_t getLength();
	//
    VelocityProfilePoint getVelPoint(int index);
	//
    double getLastDistance() const;
	//
    double getFirstDistance() const;
	//
    int isLocalMinimum(size_t index);
	
	//
	int isLocalMinimum2(size_t index);
	//
    void addProfilePoint(const VelocityProfilePoint & point);
	//
    double getCurvature(double distance);
	//
    bool isDoublePointProblem(size_t index, double position) const;
	//
	double interpolateCurvature(size_t index, double position) const;
	//
	double getVelocity2(double distance) const;
	//
    bool isDoublePointProblemVel(size_t index, double position) const;
	//
    double interpolateVelocity(size_t index, double position) const;
	//
    void invert();
	//
    void appendProfileByMin2(const VelocityProfile & other);
	//
    ProfileCrossSection getCrossSection2(const VelocityProfile &other) const;
	//
    size_t getStartIndex2(double startPos) const;
	//
    void replaceEndFrom2(const VelocityProfile& other, double from);
	//
    void calculateTime();
	//
    double getVelAt(size_t index) const;
	//
    double getTimeAt(size_t index) const;
	
	
	
	//Még kérdéses, hogy kell-e
    void changeVelAt(size_t index,double vel);
private:
    

};


#endif //PATH_VELOCITY_PROFILER_VELOCITYPROFILE_H
