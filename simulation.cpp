#include <iostream>
#include "Animator.h"
#include "VehicleBase.h"
#include <map>
#include <random>
#include "Ingest.h"
#include <typeinfo>


int main(int argc, char* argv[]){
    //Check to make sure that the correct amount of args were provided
    if(argc < 2){
        std::cerr << "not enough arguments were provided! Enter arguments using the following template: ./simulation [input file] [initial seed]" << std::endl;
        return 0;
    }
    //setting up the random number generator to be used with the Randomizer class
    mt19937 rng;
    uniform_real_distribution<double> rand_double(0.0,1.0);

    string tempSeed = argv[2];

    for(int i = 0; i < tempSeed.size(); i++){
        if(tempSeed.at(i) != '0' && tempSeed.at(i) != '1' && tempSeed.at(i) != '2' && tempSeed.at(i) != '3' && tempSeed.at(i) != '4' && tempSeed.at(i) != '5'
        && tempSeed.at(i) != '6'  && tempSeed.at(i) != '7' && tempSeed.at(i) != '8' && tempSeed.at(i) != '9'){

        std::cerr << "Please enter an integer in the seed position. Use the following format: ./simulation [input file] [initial seed] for running the program" << std::endl;
        return 0;

        }
    } 

    int seed = stoi(argv[2]);
    rng.seed(seed);
    double randNum = rand_double(rng);

    //initializing the parameters from the input file, checking to make sure the file is set up properly 
    Ingest param(argv[1]);
    if(param.getInit() == false){
        return 0;
    }


    Animator::MAX_VEHICLE_COUNT = 999;
    char next; 

    int halfSize = param.getNumSectsBeforeIntersect();
    int clock_tick = param.getMaxSimTime();
    Animator anim(halfSize);

    std::vector<VehicleBase*> westbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> eastbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> southbound(halfSize * 2 + 2, nullptr);
    std::vector<VehicleBase*> northbound(halfSize * 2 + 2, nullptr);

    //Initialize the parameters needed to run the simulation.

    int number_of_sections_before_intersection = param.getNumSectsBeforeIntersect();
    int green_north_south = param.getGreenNSTime();
    int yellow_north_south = param.getYellowNSTime();
    int green_east_west = param.getGreenEWTime();
    int yellow_east_west = param.getYellowEWTime();
    double prob_new_vehicle_northbound = param.getNewVehicleProbN();
    double prob_new_vehicle_southbound = param.getNewVehicleProbS();
    double prob_new_vehicle_eastbound = param.getNewVehicleProbE();
    double prob_new_vehicle_westbound = param.getNewVehicleProbW();;
    double proportion_of_cars = param.getProbCars();
    double proportion_of_SUVs = param.getProbSUV();
    double proportion_right_turn_cars = param.getProbCRightTurn(); 
    double proportion_left_turn_cars = param.getProbCLeftTurn(); 
    double proportion_right_turn_SUVs = param.getProbSRightTurn();
    double proportion_left_turn_SUVs = param.getProbSLeftTurn();
    double proportion_right_turn_trucks = param.getProbTRightTurn();
    double proportion_left_turn_trucks = param.getProbTLeftTurn();

    // Initialize the start state of the NS EW lights
    anim.setLightNorthSouth(LightColor::red);
    anim.setLightEastWest(LightColor::green);

    LightColor currState_North_South = LightColor::red;
    LightColor currState_East_West = LightColor::green;

    // Initialize temporary records for current light time of duration
    int currGreenNS_Count = 0;
    int currYellowNS_Count = 0;
    int currGreenEW_Count = 0;
    int currYellowEW_Count = 0;

    // Record the section occupied by vehicles
    bool sectionOccupied_NS[halfSize * 2 + 2] = {false};
    bool sectionOccupied_SN[halfSize * 2 + 2] = {false};
    bool sectionOccupied_EW[halfSize * 2 + 2] = {false};
    bool sectionOccupied_WE[halfSize * 2 + 2] = {false};

    // Iterate the vector in ascending order so that the cars 
    // in the front will be displayed first
    std::map<int, VehicleBase*, std::greater<int>> southBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> northBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> eastBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> westBoundVehicles;

    std::map<int, VehicleBase*, std::greater<int>> dummy_southBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> dummy_northBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> dummy_eastBoundVehicles;
    std::map<int, VehicleBase*, std::greater<int>> dummy_westBoundVehicles;

    int currGreenNS_dummy = 0;
    int currYellowNS_dummy = 0;
    int currGreenEW_dummy = 0;
    int currYellowEW_dummy = 0;
    
    // Start sim
    // vehicle spawning.
    for(int i = 0; i < clock_tick + 1; i++){
    double randNum = rand_double(rng); //rerolls the rand number
        //northbound lane spawning 
        if (randNum < prob_new_vehicle_northbound && sectionOccupied_SN[0] == false){
            randNum = rand_double(rng);

            //car spawning
            if(randNum < proportion_of_cars){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_cars){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::north, Turn::right);
                }
                else if(randNum >= proportion_right_turn_cars && randNum < (1-proportion_left_turn_cars)){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::north, Turn::straight);
                }
            }

            //suv spawning
            else if(randNum >= proportion_of_cars && randNum < (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_SUVs){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::north, Turn::right);
                }
                else if(randNum >= proportion_right_turn_SUVs && randNum < (1-proportion_left_turn_SUVs)){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::north, Turn::straight);
                }
            }

            //truck spawning
            else if(randNum >= (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_trucks){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::north, Turn::right);
                }
                else if(randNum >= proportion_right_turn_trucks && randNum < (1-proportion_left_turn_trucks)){
                    northBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::north, Turn::straight);
                }
            }
        }

        //southbound lane spawning
        if ((randNum >= prob_new_vehicle_northbound && randNum < prob_new_vehicle_northbound + prob_new_vehicle_southbound) && sectionOccupied_NS[0] == false){
            randNum = rand_double(rng);

            //car spawning 
            if(randNum < proportion_of_cars){

                //calculate turn
                randNum = rand_double(rng);
                if(randNum < proportion_right_turn_cars){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::south, Turn::right);
                }
                else if(randNum >= proportion_right_turn_cars && randNum < (1-proportion_left_turn_cars)){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::south, Turn::straight);
                }
            }

            //suv spawning
            else if(randNum >= proportion_of_cars && randNum < (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_SUVs){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::south, Turn::right);
                }
                else if(randNum >= proportion_right_turn_SUVs && randNum < (1-proportion_left_turn_SUVs)){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::south, Turn::straight);
                }
            }

            //truck spawning
            else if(randNum >= (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_trucks){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::south, Turn::right);
                }
                else if(randNum >= proportion_right_turn_trucks && randNum < (1-proportion_left_turn_trucks)){
                    southBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::south, Turn::straight);
                }
            }
        } 

        //eastbound lane spawning
        if ((randNum >= prob_new_vehicle_northbound + prob_new_vehicle_southbound) && (randNum < prob_new_vehicle_northbound + prob_new_vehicle_southbound + prob_new_vehicle_eastbound) && sectionOccupied_WE[0] == false){
            randNum = rand_double(rng);

            //car spawning
            if(randNum < proportion_of_cars){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_cars){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::east, Turn::right);
                }
                else if(randNum >= proportion_right_turn_cars && randNum < (1-proportion_left_turn_cars)){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::east, Turn::straight);
                }
            }

            //SUV spawning
            else if(randNum >= proportion_of_cars && randNum < (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_SUVs){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::east, Turn::right);
                }
                else if(randNum >= proportion_right_turn_SUVs && randNum < (1-proportion_left_turn_SUVs)){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::east, Turn::straight);
                }
            }

            //truck spawning
            else if(randNum >= (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_trucks){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::east, Turn::right);
                }
                else if(randNum >= proportion_right_turn_trucks && randNum < (1-proportion_left_turn_trucks)){
                    eastBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::east, Turn::straight);
                }
            }
        }

        //westbound lane spawning
        if ((randNum >= prob_new_vehicle_northbound + prob_new_vehicle_southbound + prob_new_vehicle_eastbound) && (randNum < prob_new_vehicle_northbound + prob_new_vehicle_southbound + prob_new_vehicle_eastbound + prob_new_vehicle_westbound) && sectionOccupied_EW[0] == false){
            randNum = rand_double(rng);

            //car spawning
            if(randNum < proportion_of_cars){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_cars){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::west, Turn::right);
                }
                else if(randNum >= proportion_right_turn_cars && randNum < (1-proportion_left_turn_cars)){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::car, Direction::west, Turn::straight);
                }
            }

            //suv spawning
            else if(randNum >= proportion_of_cars && randNum < (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);

                //calculate turn
                if(randNum < proportion_right_turn_SUVs){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::west, Turn::right);
                }
                else if(randNum >= proportion_right_turn_SUVs && randNum < (1-proportion_left_turn_SUVs)){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::suv, Direction::west, Turn::straight);
                }
            }

            //truck spawning
            else if(randNum >= (proportion_of_cars + proportion_of_SUVs)){
                randNum = rand_double(rng);
                
                //calculate turn
                if(randNum < proportion_right_turn_trucks){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::west, Turn::right);
                }
                else if(randNum >= proportion_right_turn_trucks && randNum < (1-proportion_left_turn_trucks)){
                    westBoundVehicles[-1] = new VehicleBase (VehicleType::truck, Direction::west, Turn::straight);
                }
            }
        }

        


        // Traffic Light simulation
        // Everything stays still when time is 0
        if(i == 0){
            anim.setVehiclesNorthbound(northbound);
            anim.setVehiclesWestbound(westbound);
            anim.setVehiclesSouthbound(southbound);
            anim.setVehiclesEastbound(eastbound);

            anim.draw(i);
            std::cin.get(next);

            continue;
        }

        std::map<int, VehicleBase*, std::greater<int>> dummy_southBoundVehicles;
        std::map<int, VehicleBase*, std::greater<int>> dummy_northBoundVehicles;
        std::map<int, VehicleBase*, std::greater<int>> dummy_eastBoundVehicles;
        std::map<int, VehicleBase*, std::greater<int>> dummy_westBoundVehicles;


        // If green light of NS count reach the limit, reset the count and
        // change the light to yellow
        if(currGreenNS_Count == green_north_south){
            anim.setLightNorthSouth(LightColor::yellow);
            currState_North_South = LightColor::yellow;
            currGreenNS_Count = 0;

        // If yellow light of NS count reach the limit, reset the count and
        // change the NS light to red and change the EW light to light
        }else if(currYellowNS_Count == yellow_north_south){
            anim.setLightNorthSouth(LightColor::red);
            currState_North_South = LightColor::red;

            anim.setLightEastWest(LightColor::green);
            currState_East_West = LightColor::green;
            currYellowNS_Count = 0;
        }

        // If green light of EW count reach the limit, reset the count and
        // change the light to yellow
        if(currGreenEW_Count == green_east_west){
            anim.setLightEastWest(LightColor::yellow);
            currState_East_West = LightColor::yellow;
            currGreenEW_Count = 0;

        // If yellow light of EW count reach the limit, reset the count and
        // change the EW light to red and change the NS light to light
        }else if(currYellowEW_Count == yellow_east_west){
            anim.setLightEastWest(LightColor::red);
            currState_East_West = LightColor::red;

            anim.setLightNorthSouth(LightColor::green);
            currState_North_South = LightColor::green;
            currYellowEW_Count = 0;
        }

        // Increase the light time count if it is in the specific light time period
        if(currState_North_South == LightColor::green){
            currYellowEW_dummy = 0;
            currGreenEW_dummy = 0;
            currGreenNS_Count += 1;
            currGreenNS_dummy += 1;
        }else if(currState_North_South == LightColor::yellow){
            currYellowNS_Count += 1;
            currYellowNS_dummy += 1;
        }

        if(currState_East_West == LightColor::green){
            currYellowNS_dummy = 0;
            currGreenNS_dummy = 0;
            currGreenEW_Count += 1;
            currGreenEW_dummy += 1;
        }else if(currState_East_West == LightColor::yellow){
            currYellowEW_Count += 1;
            currYellowEW_dummy += 1;
        }





        

        // Consider the circumstances where the East_West light is either green or yellow

        if(currState_East_West == LightColor::green || currState_East_West == LightColor::yellow){

            // Place the cars on the second half of South - North vector
            for (auto it = northBoundVehicles.begin(); it != northBoundVehicles.end(); ++it){

                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        northbound[it -> first] = it -> second;

                        sectionOccupied_SN[halfSize * 2] = false;
                        

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        northbound[it -> first] = northbound[it -> first - 1] = it -> second;
                        sectionOccupied_SN[halfSize * 2 - 1] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        northbound[it -> first - 1] = it -> second;
                        sectionOccupied_SN[halfSize * 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        northbound[it -> first] = northbound[it -> first - 1] = northbound[it -> first - 2] = it -> second;
                        sectionOccupied_SN[halfSize * 2 - 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        northbound[it -> first - 1] = northbound[it -> first - 2] = it -> second;

                        sectionOccupied_SN[halfSize * 2 - 1] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        northbound[it -> first - 2] = it -> second;

                        sectionOccupied_SN[halfSize * 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_SN[0] == false){
                            
                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_SN[0] == false){

                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[0] == false){

                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_SN[1] == false){
                            
                            northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[1] == false){

                            northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[2] == false){

                            northbound[it -> first - 1] = northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = sectionOccupied_SN[it->first - 1] = true;
                            
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }

                    }

                    // Move every cars by 1 for other Vehicles in northbound lane
                    // Handled the cars that is still turning

                    if(it -> first <= halfSize || it -> first > halfSize){

                        int size = (it -> second) -> getSize();
                        
                        // Handle the previous half of the northbound lane
                        if(it -> first < halfSize){

                            if(sectionOccupied_SN[it -> first + 1] == true || it -> first == halfSize - 1){

                                for(int i = 0; i < size; i++){
                                    northbound[it -> first - i] = it -> second;
                                    sectionOccupied_SN[it -> first - i] = true;
                                }
                                
                                dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                                continue;

                            }
                            if(sectionOccupied_SN[it -> first + 1] == false && it -> first < halfSize - 1){
                                for(int i = 0; i < size; i++){
                                    northbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_SN[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_SN[it -> first - size + 1] = false;

                                dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                

                                continue;
                            }
                            

                        }
                        
                        // Handle the last half of the northbound lane
                        if(it -> first > halfSize){

                            // If the car is from west bound lane run this
                            if((it -> second) -> getTurn() == Turn::right){

                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize + 1){
                                    northbound[halfSize + 1] = northbound[halfSize + 2] = it -> second;
                                    sectionOccupied_EW[halfSize - 1] = sectionOccupied_EW[halfSize] = false;
                                    
                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    

                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first > halfSize + 1){
                                    for(int i = 0; i < size; i++){
                                        northbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_EW[it -> first - size + 1] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(it -> first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 1){
                                    northbound[halfSize + 2] = northbound[halfSize + 1] = westbound[halfSize - 1] = it -> second;
                                    sectionOccupied_EW[halfSize - 2] = false;
                                    
                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 2){
                                    northbound[halfSize + 3] = northbound[halfSize + 2] = northbound[halfSize + 1] = it -> second;
                                    sectionOccupied_EW[halfSize - 1] = sectionOccupied_EW[halfSize] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first > halfSize + 2){
                                    for(int i = 0; i < size; i++){
                                        northbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_EW[it -> first - size + 1] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 1){
                                    northbound[halfSize + 2] = northbound[halfSize + 1] = westbound[halfSize - 1] = westbound[halfSize - 2] = it -> second;
                                    sectionOccupied_EW[halfSize - 3] = false;
                                    
                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 2){
                                    northbound[halfSize + 3] = northbound[halfSize + 2] = northbound[halfSize + 1] = westbound[halfSize - 1] = it -> second;
                                    sectionOccupied_EW[halfSize - 2] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 3){
                                    northbound[halfSize + 4] = northbound[halfSize + 3] = northbound[halfSize + 2] = northbound[halfSize + 1] = it -> second;
                                    sectionOccupied_EW[halfSize - 1] = sectionOccupied_EW[halfSize] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(halfSize + 4, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first > halfSize + 3){
                                    for(int i = 0; i < size; i++){
                                        northbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_EW[it -> first - size + 1] = false;

                                    dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }
                            }else{

                                for(int i = 0; i < size; i++){
                                    northbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_SN[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_SN[it -> first - size + 1] = false;

                                dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            }
                        }
                    } 
                }
            }

            // Place the cars on the second half of North - South vector
            for (auto it = southBoundVehicles.begin(); it != southBoundVehicles.end(); ++it){

                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        southbound[it -> first] = it -> second;

                        sectionOccupied_NS[halfSize * 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        southbound[it -> first] = southbound[it -> first - 1] = it -> second;
                        sectionOccupied_NS[halfSize * 2 - 1] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        southbound[it -> first - 1] = it -> second;
                        sectionOccupied_NS[halfSize * 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        southbound[it -> first] = southbound[it -> first - 1] = southbound[it -> first - 2] = it -> second;
                        sectionOccupied_NS[halfSize * 2 - 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        southbound[it -> first - 1] = southbound[it -> first - 2] = it -> second;

                        sectionOccupied_NS[halfSize * 2 - 1] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        southbound[it -> first - 2] = it -> second;

                        sectionOccupied_NS[halfSize * 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_NS[0] == false){
                            
                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_NS[0] == false){

                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[0] == false){

                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_NS[1] == false){
                            
                            southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[1] == false){

                            southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[2] == false){

                            southbound[it -> first - 1] = southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = sectionOccupied_NS[it->first - 1] = true;
                            
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }

                    }

                    // Move every cars by 1 for other Vehicles in northbound lane
                    // Handled the cars that is still turning

                    if(it -> first <= halfSize || it -> first > halfSize){

                        int size = (it -> second) -> getSize();
                        
                        // Handle the previous half of the northbound lane
                        if(it -> first < halfSize){

                            if(sectionOccupied_NS[it -> first + 1] == true || it -> first == halfSize - 1){

                                for(int i = 0; i < size; i++){
                                    southbound[it -> first - i] = it -> second;
                                    sectionOccupied_NS[it -> first - i] = true;
                                }
                                
                                dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                                continue;

                            }
                            if(sectionOccupied_NS[it -> first + 1] == false && it -> first < halfSize - 1){
                                for(int i = 0; i < size; i++){
                                    southbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_NS[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_NS[it -> first - size + 1] = false;

                                dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                

                                continue;
                            }
                            

                        }
                        
                        // Handle the last half of the northbound lane
                        if(it -> first > halfSize){
                            
                            // If the car is from east bound lane run this
                            if((it -> second) -> getTurn() == Turn::right){

                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize + 1 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 1] = southbound[halfSize + 2] = it -> second;
                                    sectionOccupied_WE[halfSize - 1] = sectionOccupied_WE[halfSize] = false;
                                    
                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    

                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first > halfSize + 1){
                                    for(int i = 0; i < size; i++){
                                        southbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_WE[it -> first - size + 1] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(it -> first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 1 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 2] = southbound[halfSize + 1] = eastbound[halfSize - 1] = it -> second;
                                    sectionOccupied_WE[halfSize - 2] = false;
                                    
                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 2 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 3] = southbound[halfSize + 2] = southbound[halfSize + 1] = it -> second;
                                    sectionOccupied_WE[halfSize - 1] = sectionOccupied_WE[halfSize] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first > halfSize + 2){
                                    for(int i = 0; i < size; i++){
                                        southbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_WE[it -> first - size + 1] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 1 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 2] = southbound[halfSize + 1] = eastbound[halfSize - 1] = eastbound[halfSize - 2] = it -> second;
                                    sectionOccupied_WE[halfSize - 3] = false;
                                    
                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 2 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 3] = southbound[halfSize + 2] = southbound[halfSize + 1] = eastbound[halfSize - 1] = it -> second;
                                    sectionOccupied_WE[halfSize - 2] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 3 && (it -> second) -> getTurn() == Turn::right){
                                    southbound[halfSize + 4] = southbound[halfSize + 3] = southbound[halfSize + 2] = southbound[halfSize + 1] = it -> second;
                                    sectionOccupied_WE[halfSize - 1] = sectionOccupied_WE[halfSize] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(halfSize + 4, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first > halfSize + 3){
                                    for(int i = 0; i < size; i++){
                                        southbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_WE[it -> first - size + 1] = false;

                                    dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }
                            }else{

                                for(int i = 0; i < size; i++){
                                    southbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_NS[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_NS[it -> first - size + 1] = false;

                                dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            }
                        }
                    } 
                }
            }




































            for (auto it = westBoundVehicles.begin(); it != westBoundVehicles.end(); ++it){
                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        westbound[it -> first] = it -> second;

                        sectionOccupied_EW[halfSize * 2] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        westbound[it -> first] = westbound[it -> first - 1] = it -> second;

                        sectionOccupied_EW[halfSize * 2 - 1] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        westbound[it -> first - 1] = it -> second;

                        sectionOccupied_EW[halfSize * 2] = false;
                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                    
                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        westbound[it -> first] = westbound[it -> first - 1] = westbound[it -> first - 2] = it -> second;

                        sectionOccupied_EW[halfSize * 2 - 2] = false;
                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        westbound[it -> first - 1] = westbound[it -> first - 2] = it -> second;

                        sectionOccupied_EW[halfSize * 2 - 1] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        westbound[it -> first - 2] = it -> second;

                        sectionOccupied_EW[halfSize * 2] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_EW[0] == false){
                            
                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_EW[0] == false){

                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[0] == false){

                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_EW[1] == false){
                            
                            westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[1] == false){

                            westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[2] == false){

                            westbound[it -> first - 1] = westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = sectionOccupied_EW[it->first - 1] = true;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }

                    }

                    // Handle the right turn
                    if((it -> second) -> getTurn() == Turn::right){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_EW[it -> first + 1] == false && it -> first != halfSize - 1){

                            for(int i = 0; i < size; i++){
                                westbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_EW[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_EW[it -> first - size + 1] = false;
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }
                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_EW[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 1 && it -> first == halfSize - 1){

                            northbound[halfSize + 1] = westbound[halfSize - 1] = it->second;
                            sectionOccupied_EW[halfSize - 1] = true;
                            sectionOccupied_EW[halfSize - 2] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));

                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 1){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 2 && it -> first == halfSize - 1){
                            
                            northbound[halfSize + 1] = westbound[halfSize - 1] = westbound[halfSize - 2] = it -> second;
                            sectionOccupied_EW[halfSize - 2] = sectionOccupied_EW[halfSize - 1] = true;
                            sectionOccupied_EW[halfSize - 3] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                        
                        if((it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 3 && it -> first == halfSize - 1){
                            
                            northbound[halfSize + 1] = westbound[halfSize - 1]
                            = westbound[halfSize - 2] = westbound[halfSize - 3] = it -> second;
                            sectionOccupied_EW[halfSize - 3] =  sectionOccupied_EW[halfSize - 1] = sectionOccupied_EW[halfSize - 2] = true;
                            sectionOccupied_EW[halfSize - 4] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                    }

                    // Move every cars by 1 for straight going cars

                    if((it -> second) ->getTurn() == Turn::straight){
                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_EW[it -> first + 1] == false && it -> first != halfSize - 1 ){

                            for(int i = 0; i < size; i++){
                                westbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_EW[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_EW[it -> first - size + 1] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }

                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_EW[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        // Check for whether the Vehicles could make it to go across the northbound when at the enter point
                        // if yes, move up by one, if not, do nonthing
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 2){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_EW[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_EW[it -> first - size + 1] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 3){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_EW[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_EW[it -> first - size + 1] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 4){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_EW[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_EW[it -> first - size + 1] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 4){
                            for(int i = 0; i < size; i++){
                                westbound[it -> first - i] = it -> second;
                                sectionOccupied_EW[it -> first - i] = true;
                            }
                            dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));


                            continue;
                        }

                    }

                }

            }





            for (auto it = eastBoundVehicles.begin(); it != eastBoundVehicles.end(); ++it){
                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        eastbound[it -> first] = it -> second;

                        sectionOccupied_WE[halfSize * 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        eastbound[it -> first] = eastbound[it -> first - 1] = it -> second;

                        sectionOccupied_WE[halfSize * 2 - 1] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        eastbound[it -> first - 1] = it -> second;

                        sectionOccupied_WE[halfSize * 2] = false;
                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        eastbound[it -> first] = eastbound[it -> first - 1] = eastbound[it -> first - 2] = it -> second;

                        sectionOccupied_WE[halfSize * 2 - 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        eastbound[it -> first - 1] = eastbound[it -> first - 2] = it -> second;

                        sectionOccupied_WE[halfSize * 2 - 1] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        eastbound[it -> first - 2] = it -> second;

                        sectionOccupied_WE[halfSize * 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_WE[0] == false){
                            
                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_WE[0] == false){

                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[0] == false){

                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_WE[1] == false){
                            
                            eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[1] == false){

                            eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[2] == false){

                            eastbound[it -> first - 1] = eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = sectionOccupied_WE[it->first - 1] = true;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }

                    }

                    // Handle the right turn
                    if((it -> second) -> getTurn() == Turn::right){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_WE[it -> first + 1] == false && it -> first != halfSize - 1){

                            for(int i = 0; i < size; i++){
                                eastbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_WE[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_WE[it -> first - size + 1] = false;
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }
                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_WE[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 1 && it -> first == halfSize - 1){

                            southbound[halfSize + 1] = eastbound[halfSize - 1] = it->second;
                            sectionOccupied_WE[halfSize - 1] = true;
                            sectionOccupied_WE[halfSize - 2] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));

                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 1){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 2 && it -> first == halfSize - 1){
                            
                            southbound[halfSize + 1] = eastbound[halfSize - 1] = eastbound[halfSize - 2] = it -> second;
                            sectionOccupied_WE[halfSize - 1] = sectionOccupied_WE[halfSize - 2] = true;
                            sectionOccupied_WE[halfSize - 3] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                        
                        if((it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 3 && it -> first == halfSize - 1){
                            
                            southbound[halfSize + 1] = eastbound[halfSize - 1]
                            = eastbound[halfSize - 2] = eastbound[halfSize - 3] = it -> second;
                            sectionOccupied_WE[halfSize - 3] =  sectionOccupied_WE[halfSize - 1] = sectionOccupied_WE[halfSize - 2] = true;
                            sectionOccupied_WE[halfSize - 4] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 3){

                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                    }

                    // Move every cars by 1 for straight going cars

                    if((it -> second) ->getTurn() == Turn::straight){
                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_WE[it -> first + 1] == false && it -> first != halfSize - 1 ){

                            for(int i = 0; i < size; i++){
                                eastbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_WE[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_WE[it -> first - size + 1] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }

                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_WE[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        // Check for whether the Vehicles could make it to go across the northbound when at the enter point
                        // if yes, move up by one, if not, do nonthing
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 2){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_WE[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_WE[it -> first - size + 1] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 3){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_WE[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_WE[it -> first - size + 1] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) >= 4){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_WE[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_WE[it -> first - size + 1] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_east_west - currGreenEW_dummy + yellow_east_west - currYellowEW_dummy) < 4){
                            for(int i = 0; i < size; i++){
                                eastbound[it -> first - i] = it -> second;
                                sectionOccupied_WE[it -> first - i] = true;
                            }
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));


                            continue;
                        }

                    }

                }

            }

        }





        // Consider the circumstances where the North South light is either green or yellow

        if(currState_North_South == LightColor::green || currState_North_South == LightColor::yellow){

            // Place the cars on the second half of South - North vector
            for (auto it = westBoundVehicles.begin(); it != westBoundVehicles.end(); ++it){

                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        westbound[it -> first] = it -> second;

                        sectionOccupied_EW[halfSize * 2] = false;
                        
                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        westbound[it -> first] = westbound[it -> first - 1] = it -> second;
                        sectionOccupied_EW[halfSize * 2 - 1] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        westbound[it -> first - 1] = it -> second;
                        sectionOccupied_EW[halfSize * 2] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        westbound[it -> first] = westbound[it -> first - 1] = westbound[it -> first - 2] = it -> second;
                        sectionOccupied_EW[halfSize * 2 - 2] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        westbound[it -> first - 1] = westbound[it -> first - 2] = it -> second;

                        sectionOccupied_EW[halfSize * 2 - 1] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        westbound[it -> first - 2] = it -> second;

                        sectionOccupied_EW[halfSize * 2] = false;

                        dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_EW[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_EW[0] == false){
                            
                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_EW[0] == false){

                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[0] == false){

                            westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first + 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_EW[1] == false){
                            
                            westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[1] == false){

                            westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_EW[2] == false){

                            westbound[it -> first - 1] = westbound[it -> first] = westbound[it -> first + 1] = it -> second;
                            sectionOccupied_EW[it -> first] = sectionOccupied_EW[it -> first + 1] = sectionOccupied_EW[it->first - 1] = true;
                            
                            dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }

                    }

                    // Move every cars by 1 for other Vehicles in northbound lane
                    // Handled the cars that is still turning

                    if(it -> first <= halfSize || it -> first > halfSize){

                        int size = (it -> second) -> getSize();
                        
                        // Handle the previous half of the northbound lane
                        if(it -> first < halfSize){

                            if(sectionOccupied_EW[it -> first + 1] == true || it -> first == halfSize - 1){

                                for(int i = 0; i < size; i++){
                                    westbound[it -> first - i] = it -> second;
                                    sectionOccupied_EW[it -> first - i] = true;
                                }
                                
                                dummy_westBoundVehicles.insert(std::make_pair(it->first, it -> second));

                                continue;

                            }
                            if(sectionOccupied_EW[it -> first + 1] == false && it -> first < halfSize - 1){
                                for(int i = 0; i < size; i++){
                                    westbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_EW[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_EW[it -> first - size + 1] = false;

                                dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                

                                continue;
                            }
                            

                        }
                        
                        // Handle the last half of the southbound lane
                        if(it -> first > halfSize){

                            // If the car is from south bound lane run this
                            if((it -> second) -> getTurn() == Turn::right){

                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize + 1){
                                    westbound[halfSize + 1] = westbound[halfSize + 2] = it -> second;
                                    sectionOccupied_NS[halfSize - 1] = false;
                                    
                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    

                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first > halfSize + 1){
                                    for(int i = 0; i < size; i++){
                                        westbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_NS[it -> first - size + 1] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(it -> first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 1){
                                    westbound[halfSize + 2] = westbound[halfSize + 1] = southbound[halfSize - 1] = it -> second;
                                    sectionOccupied_NS[halfSize - 2] = false;
                                    
                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 2){
                                    westbound[halfSize + 3] = westbound[halfSize + 2] = westbound[halfSize + 1] = it -> second;
                                    sectionOccupied_NS[halfSize - 1] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first > halfSize + 2){
                                    for(int i = 0; i < size; i++){
                                        westbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_NS[it -> first - size + 1] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 1){
                                    westbound[halfSize + 2] = westbound[halfSize + 1] = southbound[halfSize - 1] = southbound[halfSize - 2] = it -> second;
                                    sectionOccupied_NS[halfSize - 3] = false;
                                    
                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 2){
                                    westbound[halfSize + 3] = westbound[halfSize + 2] = westbound[halfSize + 1] = southbound[halfSize - 1] = it -> second;
                                    sectionOccupied_NS[halfSize - 2] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 3){
                                    westbound[halfSize + 4] = westbound[halfSize + 3] = westbound[halfSize + 2] = westbound[halfSize + 1] = it -> second;
                                    sectionOccupied_NS[halfSize - 1] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(halfSize + 4, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first > halfSize + 3){
                                    for(int i = 0; i < size; i++){
                                        westbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_NS[it -> first - size + 1] = false;

                                    dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }

                            }else{
                                for(int i = 0; i < size; i++){
                                    westbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_EW[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_EW[it -> first - size + 1] = false;

                                dummy_westBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            }
                        }
                    } 
                }
            }







            // Place the cars on the second half of North - South vector
            for (auto it = eastBoundVehicles.begin(); it != eastBoundVehicles.end(); ++it){

                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        eastbound[it -> first] = it -> second;

                        sectionOccupied_WE[halfSize * 2] = false;
                        
                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        eastbound[it -> first] = eastbound[it -> first - 1] = it -> second;
                        sectionOccupied_WE[halfSize * 2 - 1] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        eastbound[it -> first - 1] = it -> second;
                        sectionOccupied_WE[halfSize * 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        eastbound[it -> first] = eastbound[it -> first - 1] = eastbound[it -> first - 2] = it -> second;
                        sectionOccupied_WE[halfSize * 2 - 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        eastbound[it -> first - 1] = eastbound[it -> first - 2] = it -> second;

                        sectionOccupied_WE[halfSize * 2 - 1] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        eastbound[it -> first - 2] = it -> second;

                        sectionOccupied_WE[halfSize * 2] = false;

                        dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_WE[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_WE[0] == false){
                            
                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_WE[0] == false){

                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[0] == false){

                            eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first + 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_WE[1] == false){
                            
                            eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[1] == false){

                            eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_WE[2] == false){

                            eastbound[it -> first - 1] = eastbound[it -> first] = eastbound[it -> first + 1] = it -> second;
                            sectionOccupied_WE[it -> first] = sectionOccupied_WE[it -> first + 1] = sectionOccupied_WE[it->first - 1] = true;
                            
                            dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            continue;
                        }

                    }

                    // Move every cars by 1 for other Vehicles in northbound lane
                    // Handled the cars that is still turning

                    if(it -> first <= halfSize || it -> first > halfSize){

                        int size = (it -> second) -> getSize();
                        
                        // Handle the previous half of the northbound lane
                        if(it -> first < halfSize){

                            if(sectionOccupied_WE[it -> first + 1] == true || it -> first == halfSize - 1){

                                for(int i = 0; i < size; i++){
                                    eastbound[it -> first - i] = it -> second;
                                    sectionOccupied_WE[it -> first - i] = true;
                                }
                                
                                dummy_eastBoundVehicles.insert(std::make_pair(it->first, it -> second));

                                continue;

                            }
                            if(sectionOccupied_WE[it -> first + 1] == false && it -> first < halfSize - 1){
                                for(int i = 0; i < size; i++){
                                    eastbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_WE[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_WE[it -> first - size + 1] = false;

                                dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                

                                continue;
                            }
                            

                        }
                        
                        // Handle the last half of the northbound lane
                        if(it -> first > halfSize){
                            
                            // If the car is from north bound lane run this
                            if((it -> second) -> getTurn() == Turn::right){

                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize + 1){
                                    eastbound[halfSize + 1] = eastbound[halfSize + 2] = it -> second;
                                    sectionOccupied_SN[halfSize - 1] = sectionOccupied_SN[halfSize] = false;
                                    
                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    

                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::car && it -> first > halfSize + 1){
                                    for(int i = 0; i < size; i++){
                                        eastbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_SN[it -> first - size + 1] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(it -> first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 1){
                                    eastbound[halfSize + 2] = eastbound[halfSize + 1] = northbound[halfSize - 1] = it -> second;
                                    sectionOccupied_SN[halfSize - 2] = false;
                                    
                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize + 2){
                                    eastbound[halfSize + 3] = eastbound[halfSize + 2] = eastbound[halfSize + 1] = it -> second;
                                    sectionOccupied_SN[halfSize - 1] = sectionOccupied_SN[halfSize] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::suv && it -> first > halfSize + 2){
                                    for(int i = 0; i < size; i++){
                                        eastbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_SN[it -> first - size + 1] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 1){
                                    eastbound[halfSize + 2] = eastbound[halfSize + 1] = northbound[halfSize - 1] = northbound[halfSize - 2] = it -> second;
                                    sectionOccupied_SN[halfSize - 3] = false;
                                    
                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 2, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 2){
                                    eastbound[halfSize + 3] = eastbound[halfSize + 2] = eastbound[halfSize + 1] = northbound[halfSize - 1] = it -> second;
                                    sectionOccupied_SN[halfSize - 2] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 3, it -> second));
                                    
                                    continue;
                                }
                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize + 3){
                                    eastbound[halfSize + 4] = eastbound[halfSize + 3] = eastbound[halfSize + 2] = eastbound[halfSize + 1] = it -> second;
                                    sectionOccupied_SN[halfSize - 1] = sectionOccupied_SN[halfSize] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 4, it -> second));
                                    
                                    continue;
                                }

                                if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first > halfSize + 3){
                                    for(int i = 0; i < size; i++){
                                        eastbound[it -> first + 1 - i] = it -> second;
                                    }
                                    sectionOccupied_SN[it -> first - size + 1] = false;

                                    dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                                    
                                    continue;
                                }
                            }else{

                                for(int i = 0; i < size; i++){
                                    eastbound[it -> first + 1 - i] = it -> second;
                                    sectionOccupied_WE[it -> first + 1 - i] = true;
                                }
                                sectionOccupied_WE[it -> first - size + 1] = false;

                                dummy_eastBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            }
                        }
                    } 
                }
            }





            for (auto it = northBoundVehicles.begin(); it != northBoundVehicles.end(); ++it){
                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        northbound[it -> first] = it -> second;

                        sectionOccupied_SN[halfSize * 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        northbound[it -> first] = northbound[it -> first - 1] = it -> second;

                        sectionOccupied_SN[halfSize * 2 - 1] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        northbound[it -> first - 1] = it -> second;

                        sectionOccupied_SN[halfSize * 2] = false;
                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        northbound[it -> first] = northbound[it -> first - 1] = northbound[it -> first - 2] = it -> second;

                        sectionOccupied_SN[halfSize * 2 - 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        northbound[it -> first - 1] = northbound[it -> first - 2] = it -> second;

                        sectionOccupied_SN[halfSize * 2 - 1] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        northbound[it -> first - 2] = it -> second;

                        sectionOccupied_SN[halfSize * 2] = false;

                        dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_SN[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_SN[0] == false){
                            
                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_SN[0] == false){

                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[0] == false){

                            northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first + 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_SN[1] == false){
                            
                            northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[1] == false){

                            northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_SN[2] == false){

                            northbound[it -> first - 1] = northbound[it -> first] = northbound[it -> first + 1] = it -> second;
                            sectionOccupied_SN[it -> first] = sectionOccupied_SN[it -> first + 1] = sectionOccupied_SN[it->first - 1] = true;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }

                    }

                    // Handle the right turn
                    if((it -> second) -> getTurn() == Turn::right){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_SN[it -> first + 1] == false && it -> first != halfSize - 1){

                            for(int i = 0; i < size; i++){
                                northbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_SN[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_SN[it -> first - size + 1] = false;
                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }
                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_SN[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 1 && it -> first == halfSize - 1){

                            eastbound[halfSize + 1] = northbound[halfSize - 1] = it->second;
                            sectionOccupied_SN[halfSize - 1] = true;
                            sectionOccupied_SN[halfSize - 2] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));

                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 1){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 2 && it -> first == halfSize - 1){
                            
                            eastbound[halfSize + 1] = northbound[halfSize - 1] = northbound[halfSize - 2] = it -> second;
                            sectionOccupied_SN[halfSize - 1] = sectionOccupied_SN[halfSize - 2] = true;
                            sectionOccupied_SN[halfSize - 3] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                        
                        if((it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 3 && it -> first == halfSize - 1){
                            
                            eastbound[halfSize + 1] = northbound[halfSize - 1]
                            = northbound[halfSize - 2] = northbound[halfSize - 3] = it -> second;
                            sectionOccupied_SN[halfSize - 3] = sectionOccupied_SN[halfSize - 2] = sectionOccupied_SN[halfSize - 1] = true;
                            sectionOccupied_SN[halfSize - 4] = false;

                            dummy_eastBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                    }

                    // Move every cars by 1 for straight going cars

                    if((it -> second) ->getTurn() == Turn::straight){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_SN[it -> first + 1] == false && it -> first != halfSize - 1 ){

                            for(int i = 0; i < size; i++){
                                northbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_SN[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_SN[it -> first - size + 1] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }

                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_SN[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        // Check for whether the Vehicles could make it to go across the northbound when at the enter point
                        // if yes, move up by one, if not, do nonthing
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 2){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_SN[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_SN[it -> first - size + 1] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 3){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_SN[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_SN[it -> first - size + 1] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 4){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_SN[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_SN[it -> first - size + 1] = false;

                            dummy_northBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 4){
                            for(int i = 0; i < size; i++){
                                northbound[it -> first - i] = it -> second;
                                sectionOccupied_SN[it -> first - i] = true;
                            }
                            dummy_northBoundVehicles.insert(std::make_pair(it->first, it -> second));


                            continue;
                        }

                    }

                }

            }





            for (auto it = southBoundVehicles.begin(); it != southBoundVehicles.end(); ++it){
                // Put the cars on the road on the second half % check if they are on the second half

                // Deal with the edge cases
                if(it -> first >= halfSize * 2 + 1){

                    // Check if car is starting to leave the road
                    if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 1)){
                        southbound[it -> first] = it -> second;

                        sectionOccupied_NS[halfSize * 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    // Check if the car already left the road, delete it if it is
                    }else if(((it -> second) -> getVehicleType() == VehicleType::car && it -> first == halfSize * 2 + 2)){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;

                    // Check if the SUV is starting to leave 1 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 1)){
                        southbound[it -> first] = southbound[it -> first - 1] = it -> second;

                        sectionOccupied_NS[halfSize * 2 - 1] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    // Check if the SUV is starting to leave 2 section
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 2)){
                        southbound[it -> first - 1] = it -> second;

                        sectionOccupied_NS[halfSize * 2] = false;
                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;
                    
                    // Check if the SUV already left
                    }else if(((it -> second) -> getVehicleType() == VehicleType::suv && it -> first == halfSize * 2 + 3)){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;
                        
                    // Start to check Truck
                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 1){
                        southbound[it -> first] = southbound[it -> first - 1] = southbound[it -> first - 2] = it -> second;

                        sectionOccupied_NS[halfSize * 2 - 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 2){
                        southbound[it -> first - 1] = southbound[it -> first - 2] = it -> second;

                        sectionOccupied_NS[halfSize * 2 - 1] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                        

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 3){
                        southbound[it -> first - 2] = it -> second;

                        sectionOccupied_NS[halfSize * 2] = false;

                        dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));

                        continue;

                    }else if((it -> second) -> getVehicleType() == VehicleType::truck && it -> first == halfSize * 2 + 4){
                        sectionOccupied_NS[halfSize * 2 + 1] = false;

                        continue;
                    }

                }else{

                    // Handle the scenarios where Vehicles go into the road
                    if(it -> first == -1){
                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_NS[0] == false){
                            
                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::car && sectionOccupied_NS[0] == false){

                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[0] == false){

                            southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first + 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }
                       
                    }
                    if(it -> first == 0){

                        if((it -> second) -> getVehicleType() == VehicleType::suv && sectionOccupied_NS[1] == false){
                            
                            southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                            
                        }else if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[1] == false){

                            southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }
                    

                    }

                    if(it -> first == 1){
                        if((it -> second) -> getVehicleType() == VehicleType::truck && sectionOccupied_NS[2] == false){

                            southbound[it -> first - 1] = southbound[it -> first] = southbound[it -> first + 1] = it -> second;
                            sectionOccupied_NS[it -> first] = sectionOccupied_NS[it -> first + 1] = sectionOccupied_NS[it->first - 1] = true;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;
                        }

                    }

                    // Handle the right turn
                    if((it -> second) -> getTurn() == Turn::right){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_NS[it -> first + 1] == false && it -> first != halfSize - 1){

                            for(int i = 0; i < size; i++){
                                southbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_NS[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_NS[it -> first - size + 1] = false;
                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }
                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_NS[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 1 && it -> first == halfSize - 1){

                            westbound[halfSize + 1] = southbound[halfSize - 1] = it->second;
                            sectionOccupied_NS[halfSize - 1] = true;
                            sectionOccupied_NS[halfSize - 2] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));

                            continue;
                        }
                        
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 1){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if((it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 2 && it -> first == halfSize - 1){
                            
                            westbound[halfSize + 1] = southbound[halfSize - 1] = southbound[halfSize - 2] = it -> second;
                            sectionOccupied_NS[halfSize - 1] = sectionOccupied_NS[halfSize - 2] = true;
                            sectionOccupied_NS[halfSize - 3] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }
                        
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                        
                        if((it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 3 && it -> first == halfSize - 1){
                            
                            westbound[halfSize + 1] = southbound[halfSize - 1]
                            = southbound[halfSize - 2] = southbound[halfSize - 3] = it -> second;
                            sectionOccupied_NS[halfSize - 3] =  sectionOccupied_NS[halfSize - 1] = sectionOccupied_NS[halfSize - 2] = true;
                            sectionOccupied_NS[halfSize - 4] = false;

                            dummy_westBoundVehicles.insert(std::make_pair(halfSize + 1, it->second));
                            
                            continue;
                        }
                        
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }
                    }

                    // Move every cars by 1 for straight going cars

                    if((it -> second) ->getTurn() == Turn::straight){

                        int size = (it -> second) -> getSize();
                        
                        // Check if there is any car in front of it and it is not entering the cross section
                        if(sectionOccupied_NS[it -> first + 1] == false && it -> first != halfSize - 1 ){

                            for(int i = 0; i < size; i++){
                                southbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_NS[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_NS[it -> first - size + 1] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;
                        }

                        // Check if there is any car in front of it -- reprint the car
                        if(sectionOccupied_NS[it -> first + 1] == true){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        // Check for whether the Vehicles could make it to go across the northbound when at the enter point
                        // if yes, move up by one, if not, do nonthing
                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 2){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_NS[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_NS[it -> first - size + 1] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::car && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 2){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 3){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_NS[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_NS[it -> first - size + 1] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            

                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::suv && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 3){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));

                            continue;
                        }

                        if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) >= 4){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first + 1 - i] = it -> second;
                                sectionOccupied_NS[it -> first + 1 - i] = true;
                            }
                            sectionOccupied_NS[it -> first - size + 1] = false;

                            dummy_southBoundVehicles.insert(std::make_pair(it->first + 1, it -> second));
                            
                            continue;

                        }else if(it -> first == halfSize - 1 && (it -> second) -> getVehicleType() == VehicleType::truck && 
                        (green_north_south - currGreenNS_dummy + yellow_north_south - currYellowNS_dummy) < 4){
                            for(int i = 0; i < size; i++){
                                southbound[it -> first - i] = it -> second;
                                sectionOccupied_NS[it -> first - i] = true;
                            }
                            dummy_southBoundVehicles.insert(std::make_pair(it->first, it -> second));


                            continue;
                        }

                    }

                }

            }

        }


        

        northBoundVehicles = dummy_northBoundVehicles;
        westBoundVehicles = dummy_westBoundVehicles;
        southBoundVehicles = dummy_southBoundVehicles;
        eastBoundVehicles = dummy_eastBoundVehicles;


        anim.setVehiclesNorthbound(northbound);
        anim.setVehiclesWestbound(westbound);
        anim.setVehiclesSouthbound(southbound);
        anim.setVehiclesEastbound(eastbound);

        southbound.assign(halfSize * 2 + 2, nullptr); // reset vector
        northbound.assign(halfSize * 2 + 2, nullptr); // reset
        eastbound.assign(halfSize * 2 + 2, nullptr); // reset
        westbound.assign(halfSize * 2 + 2, nullptr); // reset

        anim.draw(i);

        std::cin.get(next);
    }
}