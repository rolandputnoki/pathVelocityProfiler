//
// Created by osboxes on 22/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_PROFILECROSSSECTION_H
#define PATH_VELOCITY_PROFILER_PROFILECROSSSECTION_H


class ProfileCrossSection {
    double position;
    double value;
    bool thisLowerFirst;
    bool crossSection;
public:
    ProfileCrossSection(bool thisLowerFirst);
    ProfileCrossSection(double position, double value, bool thisLowerFrist);

    double getPosition() const;
    double getValue() const;
    bool isThisLowerFirst() const;
    bool isCrossSection() const;
};


#endif //PATH_VELOCITY_PROFILER_PROFILECROSSSECTION_H
