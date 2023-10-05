#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

class Ingest {

private:
    string fileName; //holds the name of the file to opened 
    //initialize a variable to hold all of the parameters from the input file. 
    int maxSimTime;  
    int SectionsBeforeIntersect;
    int GreenNSTime;
    int YellowNSTime; 
    int GreenEWTime;
    int YellowEWTime;
    float newVehicleProbN;
    float newVehicleProbE;
    float newVehicleProbS;
    float newVehicleProbW;
    float probCars;
    float probSUV;
    float probTruck;
    float probCRightTurn;
    float probCStraightTurn;
    float probCLeftTurn;
    float probSRightTurn;
    float probSStraightTurn;
    float probSLeftTurn;
    float probTRightTurn;
    float probTStraightTurn;
    float probTLeftTurn;
    bool init; //boolean used to indicate whether the file has been initialized properly. 

public:

    ~Ingest(); //basic constructors and destructors.
    Ingest();
    Ingest(string fName);

    //getter methods in order to access the variables setup ealier. 
    int getMaxSimTime();
    int getNumSectsBeforeIntersect();
    int getGreenNSTime();
    int getYellowNSTime();
    int getGreenEWTime();
    int getYellowEWTime();
    float getNewVehicleProbN();
    float getNewVehicleProbE();
    float getNewVehicleProbS();
    float getNewVehicleProbW();
    float getProbCars();
    float getProbSUV();
    float getProbTruck();
    float getProbCRightTurn();
    float getProbCStraight();
    float getProbCLeftTurn();
    float getProbSRightTurn();
    float getProbSStraight();
    float getProbSLeftTurn();
    float getProbTRightTurn();
    float getProbTStraight();
    float getProbTLeftTurn();
    bool getInit();
};


