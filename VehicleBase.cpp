#ifndef __VEHICLE_BASE_CPP__
#define __VEHICLE_BASE_CPP__

#include "VehicleBase.h"

int VehicleBase::vehicleCount = 0;

VehicleBase::VehicleBase(VehicleType type, Direction direction, Turn turn)
    : vehicleID(VehicleBase::vehicleCount++), 
      vehicleType(type),
      vehicleDirection(direction),
      vehicleTurn(turn)
{}

VehicleBase::VehicleBase(VehicleType type, Direction direction)
    : vehicleID(VehicleBase::vehicleCount++), 
      vehicleType(type),
      vehicleDirection(direction)
{}

//copy constructor
VehicleBase::VehicleBase(const VehicleBase& other)
    : vehicleID(other.vehicleID),
      vehicleType(other.vehicleType),
      vehicleDirection(other.vehicleDirection),
      vehicleTurn(other.vehicleTurn)
{}

//copy assignment
VehicleBase& VehicleBase::operator=(const VehicleBase& other){
  if(this == &other){
    return *this;
    }
  this->vehicleID = other.vehicleID;
  this->vehicleType = other.vehicleType;
  this->vehicleDirection = other.vehicleDirection;
  this->vehicleTurn = other.vehicleTurn;
} 

//move constructor. It is important to note that Vehicle type truck and Turn left are being used as pseudo "null" values.
VehicleBase::VehicleBase( VehicleBase&& other) noexcept{
  this->vehicleID = other.vehicleID;
  this->vehicleType = other.vehicleType;
  this->vehicleDirection = other.vehicleDirection;
  this->vehicleTurn = other.vehicleTurn;

  other.setId(-1);
  other.setVehicleType(VehicleType::truck);
  other.setVehicleOriginalDirection(Direction::none);
  other.setTurn(Turn::left);
}

//move assignment
 VehicleBase& VehicleBase::operator=(VehicleBase&& other) noexcept{
  if(this == &other){
    return *this;
    }
  this->vehicleID = other.vehicleID;
  this->vehicleType = other.vehicleType;
  this->vehicleDirection = other.vehicleDirection;
  this->vehicleTurn = other.vehicleTurn;

  other.setId(-1);
  other.setVehicleType(VehicleType::truck);
  other.setVehicleOriginalDirection(Direction::none);
  other.setTurn(Turn::left);

 }

VehicleBase::~VehicleBase() {}

#endif
