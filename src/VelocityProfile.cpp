//
// Created by osboxes on 02/04/17.
//

#include "VelocityProfile.h"
#include <fstream>
#include <iostream>
#include <cmath>


using namespace std;

//Nem használtak
std::vector<VelocityProfilePoint> VelocityProfile::getPointsVector() {
    return profilePoints;
}

int VelocityProfile::getLastPointIndex() {
    return this->profilePoints.size() -1 ;
}




//Használtak

//
void VelocityProfile::pushBackPoint(VelocityProfilePoint p) {
    this->profilePoints.push_back(p);
}

//
double VelocityProfile::getMaxVelAtIndex(int index) {
    return this->profilePoints[index].velocity;
}

//Az indexedik pont távolságát adja meg
double VelocityProfile::getPosAtIndex(int index) const{
    return this->profilePoints[index].distance;
}

//Egy VelocityProfile profilePoints vektorának értékeit
//írja ki egy csv mintájú fájlba
void VelocityProfile::writePointsToFile(size_t i) {
    VelocityProfilePoint v;
    ofstream outfile;
    std::string name = "points";
    std::string end = ".dat";
    std::string result = name + std::to_string(i) + end;
    outfile.open(result);
    int length = this->profilePoints.size();
    for(int j =0; j < length; j++){
        v = this->getVelPoint(j);
        auto data = to_string(v.distance) + "," + to_string(v.velocity) + "," + to_string(v.time) + "\n";
        outfile << data;
    }
    outfile.close();

}

//Az adott indexen elhelyezkedő VelocityProfilePoint-ot adja vissza
VelocityProfilePoint VelocityProfile::getVelPoint(int index) {
    return this->profilePoints[index];
}

//A profilePoints vektor hosszát adja vissza
size_t VelocityProfile::getLength() {
    return this->profilePoints.size();
}

//A legutolsó pont távolságát adja vissza
double VelocityProfile::getLastDistance() const {
    size_t length = this->profilePoints.size();
    return this->profilePoints[length - 1 ].distance;
}

//A legelső pont távolságát adja vissza
double VelocityProfile::getFirstDistance() const {
    return this->profilePoints[0].distance;
}

//A maxVelProfile-on dolgozik,
//megmondja, hogy az adott indexnél minimuma van-e a profilnak
//A negatív ugrást a profilban is minimumnak tekinti
int VelocityProfile::isLocalMinimum(size_t index) {
    double prevVel, nextVel, curVel;
    prevVel = getMaxVelAtIndex(index-1);
    nextVel = getMaxVelAtIndex(index+1);
    curVel  = getMaxVelAtIndex(index);

    double curDist, prevDist;
    curDist = getPosAtIndex(index);
    prevDist = getPosAtIndex(index-1);
    if(curDist == prevDist && curDist != getLastDistance()){
        if(curVel < prevVel){
            return 1;
        }
    }
    if(curVel < prevVel && curVel < nextVel){
        return 1;
    } else {
        return 0;
    }
}


//A profil végére szúr ey pontot
void VelocityProfile::addProfilePoint(const VelocityProfilePoint & point) {
    this->profilePoints.push_back(point);
}

//Egy adott távolságban adja vissza a görbületet,
//ha a távolság épp egy pontja a profilnak, akkor
//ahhoz a ponthoz tartozó görbületet ad
//Ha nem, akkor a két szomszédos pontból interpolálja
double VelocityProfile::getCurvature(double distance) {

    size_t i = 0;
    while(profilePoints[i].distance - distance <= 0){
        i++;
        if(i > profilePoints.size()){
            break;
        }
    }
    if(profilePoints[i-1].distance - distance == 0){
        return profilePoints[i-1].curvature;
    } else {
        if(i != 0 && isDoublePointProblem(i,distance)){
            if(std::fabs(profilePoints[i].curvature) < std::fabs(profilePoints[i-1].curvature)){
                return profilePoints[i].curvature;
            } else{
                return profilePoints[i].curvature;
            }
        } else {
            return interpolateCurvature(i-1,distance);
        }
    }
}

//Azt vizsgálja, hogy két pont (közel) azonos távolsággal rendelkezik-e
bool VelocityProfile::isDoublePointProblem(size_t index, double position) const {
    bool isEqual = profilePoints[index].distance == profilePoints[index-1].distance;
    bool isNear = std::fabs(position - profilePoints[index].distance) < 0.0001;
    return isEqual && isNear;
}

//Görbület értéket interpolál két pont között
double VelocityProfile::interpolateCurvature(size_t index, double position) const
{
    double x1 = profilePoints[index].distance;
    double x2 = profilePoints[index+1].distance;
    double f_x1 = profilePoints[index].curvature;
    double f_x2 = profilePoints[index+1].curvature;
    if(x1 == x2)
        return (f_x1 + f_x2)*0.5;

    return (f_x2 - f_x1)/(x2-x1)*(position-x1)+f_x1;
}

//A sebességet számolja adott távolságnál
double VelocityProfile::getVelocity2(double distance) const{

    size_t i = 0;
    while(profilePoints[i].distance - distance <= 0){
        i++;
        if(i > profilePoints.size()){
            break;
        }
    }
    if(profilePoints[i-1].distance - distance == 0){
        return profilePoints[i-1].velocity;
    } else {
        if(i != 0 && isDoublePointProblem(i,distance)){
            if(std::fabs(profilePoints[i].velocity) < std::fabs(profilePoints[i-1].velocity)){
                return profilePoints[i].velocity;
            } else{
                return profilePoints[i].velocity;
            }
        } else {
            return interpolateVelocity(i-1,distance);
        }
    }
}

//
bool VelocityProfile::isDoublePointProblemVel(size_t index, double position) const {
    bool isEqual = profilePoints[index].distance == profilePoints[index-1].distance;
    bool isNear = std::fabs(position - profilePoints[index].distance) < 0.0001;
    return isEqual && isNear;
}

//Sebességet interpolál két pont között
double VelocityProfile::interpolateVelocity(size_t index, double position) const
{
    double x1 = profilePoints[index].distance;
    double x2 = profilePoints[index+1].distance;
    double f_x1 = profilePoints[index].velocity;
    double f_x2 = profilePoints[index+1].velocity;
    if(x1 == x2)
        return (f_x1 + f_x2)*0.5;

    return (f_x2 - f_x1)/(x2-x1)*(position-x1)+f_x1;
}

//Megfordítja a profilePoints vektort
//A visszafelé terjesztett profiloknál használjuk
//Mert azoknál a pontok távolsága csökken
//így az összefűzhetőséghez meg kell fordítani őket
void VelocityProfile::invert() {
    profilePoints = std::vector<VelocityProfilePoint> (profilePoints.rbegin(),profilePoints.rend());
}

//Összefűzi a szubprofilokat
void VelocityProfile::appendProfileByMin2(const VelocityProfile &other)
{

    if(this->profilePoints.empty()) {
        this->profilePoints = other.profilePoints;
        return;
    }
    ProfileCrossSection crossSection = getCrossSection2(other);
    if(crossSection.isCrossSection()) {
        replaceEndFrom2(other, crossSection.getPosition());
    } else {
        if(!crossSection.isThisLowerFirst()) {
            replaceEndFrom2(other, other.getFirstDistance());
        } else {
            replaceEndFrom2(other, this->getLastDistance());
        }
    }
}

//Két profil metszéspontját számolja ki
ProfileCrossSection VelocityProfile::getCrossSection2(const VelocityProfile &other) const
{

    //A végső profilban a másik profil kezdőpozíciónál >= distance kezdőindexe
    size_t thisIndex = this->getStartIndex2(other.getFirstDistance());

    //A paraméter profilban adja meg azt az indexet, amire a distance nagyobb mint a végsőprofil kezdőpontjának távolsága
    size_t otherIndex = other.getStartIndex2(this->getFirstDistance());


    //Ha a paraméterül kapott profil csak a már meglévő profilpontoknál távolábbi pontot tartalmaz,
    // akkor nincs közös része a két profilnak

    if(thisIndex == this->profilePoints.size())
        return ProfileCrossSection(false);



    bool isThisHigher = this->getVelAt(thisIndex) > other.getVelAt(otherIndex);


    while(isThisHigher ^ (this->getVelAt(thisIndex) <= other.getVelAt(otherIndex))) {
        while(this->getPosAtIndex(thisIndex) <= other.getPosAtIndex(otherIndex)) {
            if(++thisIndex == this->profilePoints.size())
                return ProfileCrossSection(!isThisHigher);
        }
        while(other.getPosAtIndex(otherIndex) <= this->getPosAtIndex(thisIndex)) {
            if(++otherIndex == other.profilePoints.size())
                return ProfileCrossSection(!isThisHigher);
        }
    }

    //TODO lehetne okosítani egy metszéspont számítással
/*
    double v1 = getVelAt(thisIndex);
    double v2 = getVelAt(otherIndex);
    double x1 = getPosAtIndex(thisIndex);
    double x2 = getPosAtIndex(otherIndex);
    double delta_v1 = getVelAt(thisIndex +1 ) - getVelAt(thisIndex);
    double delta_v2 = getVelAt(otherIndex - 1 ) - getVelAt(otherIndex);
    double delta_x1 = getPosAtIndex(thisIndex + 1 ) - getPosAtIndex(thisIndex);
    double delta_x2 = getPosAtIndex(otherIndex) - getPosAtIndex(otherIndex -1);

    double delta_x = (v2 - v1 + (x2 - x1)*(delta_v2/delta_x2))/(delta_v1/delta_x1 + delta_v2/delta_x2);
    double position = x1 + delta_x;
    double value = v1 + delta_x*(delta_v1/delta_x1);
    VelocityProfilePoint v(position, value, 0);
    this->profilePoints.insert(profilePoints.begin()+ thisIndex + 1, v);

*/

    double position = std::min(this->getPosAtIndex(thisIndex), other.getPosAtIndex(otherIndex));
    double value = getVelocity2(position);

    return ProfileCrossSection(position, value, !isThisHigher);
}

//Visszaadja azt az indexet a ProfileContainerhez, ahonnan a distance már nagyobb mint a paraméterül kapott érték
size_t VelocityProfile::getStartIndex2(double startPos) const
{
    size_t index;
    for(index = 0; index < profilePoints.size(); index++) {
        if(profilePoints[index].distance >= startPos)
            break;
    }
    return index;
}

//Az indexedik pont sebességét adja vissza
double VelocityProfile::getVelAt(size_t index) const {
    return profilePoints[index].velocity;
}

//Két profil összefűzésénél használjuk,
//hogy a metszés pont után a kisebbiket vegyük figyelembe
void VelocityProfile::replaceEndFrom2(const VelocityProfile &other, double from) {
    size_t thisBeginIndex = this->getStartIndex2(from);
    size_t otherBeginIndex = other.getStartIndex2(from);
    profilePoints.erase(profilePoints.begin() + thisBeginIndex, profilePoints.end());
    profilePoints.insert(profilePoints.end(), other.profilePoints.begin() + otherBeginIndex, other.profilePoints.end());
}

//A már elkészült profil pontjaihoz számolja ki
//hogy mennyi idő telik el az adott pontig
void VelocityProfile::calculateTime() {
    profilePoints[0].time = 0;
    for(size_t i = 0; i<profilePoints.size() - 1; i++){
        double x1 = getPosAtIndex(i);
        double x2 = getPosAtIndex(i+1);
        double v1 = getVelAt(i);
        double v2 = getVelAt(i + 1);
        double delta_v = v2 - v1;
        double delta_x = x2 - x1;

        double delta_t = (delta_x/(v1 + delta_v/2));
        profilePoints[i+1].time = profilePoints[i].time + delta_t;
    }
}

//Az indexedik ponthoz tartozó időt adja meg,
//Fájlba írásnál használt
double VelocityProfile::getTimeAt(size_t index) const {
    return profilePoints[index].time;
}

//Még kérdéses a használata
void VelocityProfile::changeVelAt(size_t index, double vel) {
    profilePoints[index].velocity = vel;
}
