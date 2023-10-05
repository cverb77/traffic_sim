#include "Ingest.h"

//default constructor
Ingest::Ingest(){
    //constructor, initializing all of the variables.
    this->fileName = "";
    this->maxSimTime = 0;  
    this->SectionsBeforeIntersect = 0;
    this->GreenNSTime = 0;
    this->YellowNSTime = 0; 
    this->GreenEWTime = 0;
    this->YellowEWTime = 0;
    this->newVehicleProbN = 0.0;
    this->newVehicleProbE = 0.0;
    this->newVehicleProbS = 0.0;
    this->newVehicleProbW = 0.0;
    this->probCars = 0.0;
    this->probSUV = 0.0;
    this->probTruck = 0.0;
    this->probCRightTurn = 0.0;
    this->probCStraightTurn = 0.0;
    this->probCLeftTurn = 0.0;
    this->probSRightTurn = 0.0;
    this->probSStraightTurn = 0.0;
    this->probSLeftTurn = 0.0;
    this->probTRightTurn = 0.0;
    this->probTStraightTurn = 0.0;
    this->probTLeftTurn = 0.0;
    this->init = true;
}

//destructor
Ingest::~Ingest(){

}

//constructor with string arugment to initizialize input file
Ingest::Ingest(string fName){
    //same variable initialization
    this->probTruck = 0.0;
    this->probCStraightTurn = 0;
    this->probSStraightTurn = 0;
    this->probTStraightTurn = 0;
    this->maxSimTime = 0;  
    this->SectionsBeforeIntersect = 0;
    this->GreenNSTime = 0;
    this->YellowNSTime = 0; 
    this->GreenEWTime = 0;
    this->YellowEWTime = 0;
    this->newVehicleProbN = 0.0;
    this->newVehicleProbE = 0.0;
    this->newVehicleProbS = 0.0;
    this->newVehicleProbW = 0.0;
    this->probCars = 0.0;
    this->probSUV = 0.0;
    this->probTruck = 0.0;
    this->probCRightTurn = 0.0;
    this->probCStraightTurn = 0.0;
    this->probCLeftTurn = 0.0;
    this->probSRightTurn = 0.0;
    this->probSStraightTurn = 0.0;
    this->probSLeftTurn = 0.0;
    this->probTRightTurn = 0.0;
    this->probTStraightTurn = 0.0;
    this->probTLeftTurn = 0.0;
    this->init = true;

    //setting up the file, throw an exception if the file is not set up properly
    ifstream file(fName);
    if(!file){
        cerr << "The file " <<  "\"" << fName << "\"" << " could not be opened. Ensure .txt is included in the command line arguments and the file specified is present in the same directoy as the executable!" << endl;
        this->init = false;
    }
    string test; 

    //input for the max time the sim can run for
    while(file >> test){
        if(test.find("maximum_simulated_time") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){ //handles no space between : and variable name
                if(test == "maximum_simulated_time:"){
                    file >> test;
                    input = stoi(test);
                    this->maxSimTime = input;       
                }
                else if(test.find(":") != string::npos){ //handles space between the : and the number
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->maxSimTime = input; 
                }
                else{
                    file >> test; //handles space between : and the variable name
                    input = stoi(test);
                    this->maxSimTime = input;
                }
            }
            else{ //handles space between : and variable name and : and the number
                file >> test;
                file >> test;
                input = stoi(test);
                this->maxSimTime = input;
            }
            input = 0;
        }
        //this method of input is repeated for all of the input variables that are integers.

        //input for the number of sections there are before the intersection.
        if(test.find("number_of_sections_before_intersection") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "number_of_sections_before_intersection:"){
                    file >> test;
                    input = stoi(test);
                    this->SectionsBeforeIntersect = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->SectionsBeforeIntersect = input; 
                }
                else{
                    file >> test;
                    input = stoi(test);
                    this->SectionsBeforeIntersect = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stoi(test);
                this->SectionsBeforeIntersect = input;
            }
            input = 0;
        }

        //input for the number of time the green light peresists on the NS lane. 654
        if(test.find("green_north_south") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "green_north_south:"){
                    file >> test;
                    input = stoi(test);
                    this->GreenNSTime = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->GreenNSTime = input; 
                }
                else{
                    file >> test;
                    input = stoi(test);
                    this->GreenNSTime = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stoi(test);
                this->GreenNSTime = input;
            }
            input = 0;
        }

        //input for the duration of a yellow light on the North-South lane
        if(test.find("yellow_north_south") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "yellow_north_south:"){
                    file >> test;
                    input = stoi(test);
                    this->YellowNSTime = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->YellowNSTime = input; 
                }
                else{
                    file >> test;
                    input = stoi(test);
                    this->YellowNSTime = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stoi(test);
                this->YellowNSTime = input;
            }
            input = 0;
        }

        //input for the duration of a green light on the East-West lane
        if(test.find("green_east_west") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "green_east_west:"){
                    file >> test;
                    input = stoi(test);
                    this->GreenEWTime = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->GreenEWTime = input; 
                }
                else{
                    file >> test;
                    input = stoi(test);
                    this->GreenEWTime = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stoi(test);
                this->GreenEWTime = input;
            }
            input = 0;
        }
        
        //input for the duration of a yellow light on the East-West lane
        if(test.find("yellow_east_west") != string::npos){
            int input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "yellow_east_west:"){
                    file >> test;
                    input = stoi(test);
                    this->YellowEWTime = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stoi(out);
                    this->YellowEWTime = input; 
                }
                else{
                    file >> test;
                    input = stoi(test);
                    this->YellowEWTime = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stoi(test);
                this->YellowEWTime = input;
            }
            input = 0;
        }
        
        //All of the values from this point on are floats. The intake works mostly the same, but with change to handle a float 
        //intakes the probability of a vehicle spawning on the nothbound lane (between 0 and 1)
        if(test.find("prob_new_vehicle_northbound") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "prob_new_vehicle_northbound:"){
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbN = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->newVehicleProbN = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbN = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->newVehicleProbN = input;
            }
            input = 0;
        }

        //intakes the probability of a vehicle spawning on the nothbound lane (between 0 and 1)
        if(test.find("prob_new_vehicle_southbound") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "prob_new_vehicle_southbound:"){
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbS = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->newVehicleProbS = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbS = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->newVehicleProbS = input;
            }
            input = 0;
        }
        
        //takes in the probability of spawning a vehcile in the eastbount lane
        if(test.find("prob_new_vehicle_eastbound") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "prob_new_vehicle_eastbound:"){
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbE = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->newVehicleProbE = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbE = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->newVehicleProbE = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a vehicle in the westbound lane
        if(test.find("prob_new_vehicle_westbound") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "prob_new_vehicle_westbound:"){
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbW = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->newVehicleProbW = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->newVehicleProbW = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->newVehicleProbW = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a car
        if(test.find("proportion_of_cars") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_of_cars:"){
                    file >> test;
                    input = stof(test);
                    this->probCars = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probCars = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probCars = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probCars = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a SUV
        if(test.find("proportion_of_SUVs") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_of_SUVs:"){
                    file >> test;
                    input = stof(test);
                    this->probSUV = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probSUV = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probSUV = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probSUV = input;
            }
            input = 0;
        }
        
        //takes in the probability of spawning a car that turns right at the intersection
        if(test.find("proportion_right_turn_cars") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_right_turn_cars:"){
                    file >> test;
                    input = stof(test);
                    this->probCRightTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probCRightTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probCRightTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probCRightTurn = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a car that turns left at the intersection
        if(test.find("proportion_left_turn_cars") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_left_turn_cars:"){
                    file >> test;
                    input = stof(test);
                    this->probCLeftTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probCLeftTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probCLeftTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probCLeftTurn = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a SUV that turns right at the intersection
        if(test.find("proportion_right_turn_SUVs") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_right_turn_SUVs:"){
                    file >> test;
                    input = stof(test);
                    this->probSRightTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probSRightTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probSRightTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probSRightTurn = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a SUV that turns left at the intersection
        if(test.find("proportion_left_turn_SUVs") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_left_turn_SUVs:"){
                    file >> test;
                    input = stof(test);
                    this->probSLeftTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probSLeftTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probSLeftTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probSLeftTurn = input;
            }
            input = 0;
        }

        //takes in the probability of spawning a truck that turns right at the intersection
        if(test.find("proportion_right_turn_trucks") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_right_turn_trucks:"){
                    file >> test;
                    input = stof(test);
                    this->probTRightTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probTRightTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probTRightTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probTRightTurn = input;
            }
            input = 0;
        }
        //takes in the probability of spawning a truck that turns left at the intersection
        if(test.find("proportion_left_turn_trucks") != string::npos){
            float input = 0; 
            string out = "";
            if(test.find(":") != string::npos){
                if(test == "proportion_left_turn_trucks:"){
                    file >> test;
                    input = stof(test);
                    this->probTLeftTurn = input;       
                }
                else if(test.find(":") != string::npos){
                    size_t index = test.find(":");
                    for(int i = (index+1); i < test.length(); i++){
                        if(test[i] )
                        out = out + test[i];
                    }
                    input = stof(out);
                    this->probTLeftTurn = input; 
                }
                else{
                    file >> test;
                    input = stof(test);
                    this->probTLeftTurn = input;
                }
            }
            else{
                file >> test;
                file >> test;
                input = stof(test);
                this->probTLeftTurn = input;
            }
            input = 0;
        }
        

    }
    file.close();

}

//returns that max amount of time the sim can run for.
int Ingest::getMaxSimTime(){
    return this->maxSimTime;
}

//returns the number sections before the intersection 
int Ingest::getNumSectsBeforeIntersect(){
    return this->SectionsBeforeIntersect;
}

//returns the amount of time the green light will be displayed on the northbound/southbound lane every cycle.
int Ingest::getGreenNSTime(){
    return this->GreenNSTime;
}

//returns the amount of time the yellow light will be displayed on the northbound/southbound lane every cycle.
int Ingest::getYellowNSTime(){
    return this->YellowNSTime;
}

//returns the amount of time the green light will be displayed on the eastbound/westbound lane every cycle.
int Ingest::getGreenEWTime(){
    return this->GreenEWTime;
}

//returns the amount of time the yellow light will be displayed on the eastbound/westbound lane every cycle.
int Ingest::getYellowEWTime(){
    return this->YellowEWTime;
}

//returns the probability of a new vehicle spwaning on the northbound lane
float Ingest::getNewVehicleProbN(){
    return this->newVehicleProbN;
}

//returns the probability of a new vehicle spwaning on the eastbound lane
float Ingest::getNewVehicleProbE(){
    return this->newVehicleProbE;
}

//returns the probability of a new vehicle spwaning on the southbound lane
float Ingest::getNewVehicleProbS(){
    return this->newVehicleProbS;
}

//returns the probability of a new vehicle spwaning on the westbound lane
float Ingest::getNewVehicleProbW(){
    return this->newVehicleProbW;
}

//returns the probability of spawning a car 
float Ingest::getProbCars(){
    return this->probCars;
}

//returns the probability of spawning a SUV 
float Ingest::getProbSUV(){
    return this->probSUV; 
}

//returns the probability of spawning a truck 
float Ingest::getProbTruck(){
    float result;
    result = 1 - (this->probSUV + this->probCars);
    return result;
}

//returns the probability of a car turning right at the intersection
float Ingest::getProbCRightTurn(){
    return this->probCRightTurn;
}

//returns the probability of a car going straight at the intersection
float Ingest::getProbCStraight(){
    float result;
    result = 1 - (this->probCRightTurn + this->probCLeftTurn);
    return result;    
}

//returns the probability of a car turning left at the intersection
float Ingest::getProbCLeftTurn(){
    return this->probCLeftTurn;
}

//returns the probability of a SUV turning right at the intersection
float Ingest::getProbSRightTurn(){
    return this->probSRightTurn;
}

//returns the probability of a SUV going straight at the intersection
float Ingest::getProbSStraight(){
    float result;
    result = 1 - (this->probSRightTurn + this->probSLeftTurn);
    return result;
}

//returns the probability of a SUV turning left at the intersection
float Ingest::getProbSLeftTurn(){
    return this->probSLeftTurn;
}

//returns the probability of a truck turning right at the intersection
float Ingest::getProbTRightTurn(){
    return this->probTRightTurn;
}

//returns the probability of a truck going straight at the intersection
float Ingest::getProbTStraight(){
    float result;
    result = 1 - (this->probTRightTurn + this->probTLeftTurn);
    return result;
}

//returns the probability of a truck turning left at the intersection
float Ingest::getProbTLeftTurn(){
    return this->probTLeftTurn;
}

//returns a boolean value indicating whether the file was initialized properly
bool Ingest::getInit(){
    return this->init;
}