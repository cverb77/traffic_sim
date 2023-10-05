#ifndef __VEHICLE_BASE_H__
#define __VEHICLE_BASE_H__

// enum: see http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-enum
enum class Direction   {north, south, east, west, none};
enum class VehicleType {car, suv, truck};
enum class LightColor  {green, yellow, red};
enum class Turn {left, right, straight}; //turn 

class VehicleBase
{
   public:
      static int vehicleCount;
      

   private:
      int         vehicleID;
      VehicleType vehicleType;
      Direction   vehicleDirection;
      Turn        vehicleTurn;

   public:
      VehicleBase(VehicleType type, Direction originalDirection);
      VehicleBase(VehicleType type, Direction originalDirection, Turn turn);
      VehicleBase(const VehicleBase& other); //copy constructor
      VehicleBase& operator=(const VehicleBase& other); //copy assignment
      VehicleBase& operator=(VehicleBase&& other) noexcept; //move assignment
      VehicleBase( VehicleBase&& other) noexcept; //move constructor
      ~VehicleBase();


      inline int getVehicleID() const { return this->vehicleID; }
      inline int getSize() const {
         if(vehicleType == VehicleType::car){
            return 2;
         }else if(vehicleType == VehicleType::truck){
            return 4;
         }else{
            return 3;
         }
      }

      inline VehicleType getVehicleType() const { return this->vehicleType; }
      inline Direction   getVehicleOriginalDirection() const { return this->vehicleDirection;}
      inline Turn        getTurn() const {return this->vehicleTurn;}

      inline void        setVehicleType(VehicleType type) { this->vehicleType = type; }
      inline void        setVehicleOriginalDirection(Direction dir) { this->vehicleDirection = dir;}
      inline void        setTurn(Turn t) { this->vehicleTurn = t;}
      inline void        setId(int i) { this->vehicleID = i;}
};

#endif
