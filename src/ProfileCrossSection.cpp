//
// Created by osboxes on 22/04/17.
//

#include "ProfileCrossSection.h"

ProfileCrossSection::ProfileCrossSection(bool thisLowerFirst)
        : thisLowerFirst(thisLowerFirst), crossSection(false)
{}

ProfileCrossSection::ProfileCrossSection(double position, double value, bool thisLowerFirst)
        : position(position), value(value), thisLowerFirst(thisLowerFirst), crossSection(true)
{}

bool ProfileCrossSection::isCrossSection() const
{
    return crossSection;
}

double ProfileCrossSection::getPosition() const
{
    return position;
}

double ProfileCrossSection::getValue() const
{
    return value;
}

bool ProfileCrossSection::isThisLowerFirst() const
{
    return thisLowerFirst;
}