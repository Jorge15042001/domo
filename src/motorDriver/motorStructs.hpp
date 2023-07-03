#pragma once
// #include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <string_view>
#include <charconv>

#include "util.hpp"

enum MotorMode{
  HOME_MODE = 1,
  RELATIVE = 2,
  ABSOLUTE = 3,
  };
enum MovementUnit{
  mm = 1,
  step = 2,
  rev = 3
  };
// enum MovementType{
//   relative= 1,
//   absolute = 2
// };

struct MotorMessage{
  MotorMode movement_mode;
  MovementUnit movement_unit;
  double speed;//mm/s or step/s depending on movement_unit
  double mag;

  MotorMessage()=default;
  std::string serailize()const{
    return fmt::format("{};{};{};{}\n", movement_mode,movement_unit, speed,mag );
  }
  MotorMessage(const MotorMode mov_m, const MovementUnit mov_u, const double speed, const double  mag):
    movement_mode(mov_m), movement_unit(mov_u), speed(speed), mag(mag)
  {}

  MotorMessage(std::string msg){
    auto params_str = split(msg, ';');
    if (params_str.size()==4){
      std::cout<<"params_str does not have the same number of arguments"<<std::endl;
      throw "can't build";
    }

    int motor_mode;
    std::from_chars(params_str[0].data(),params_str[0].data()+params_str[0].size(),motor_mode);
    this->movement_mode = (MotorMode)movement_mode;

    int movement_unit;
    std::from_chars(params_str[1].data(),params_str[1].data()+params_str[1].size(),movement_unit);
    this->movement_unit= (MovementUnit)movement_unit;

    std::from_chars(params_str[2].data(),params_str[2].data()+params_str[2].size(),this->speed);
    std::from_chars(params_str[3].data(),params_str[3].data()+params_str[3].size(),this->mag);

  }
};
struct MotorSetupMessage{
  //TODO: separeta safe stop pins
  double steps_per_mm;
  double steps_per_degree;
};

struct MotorResponse{
  bool success;
  unsigned long current_step_pos ;
  double current_mm_pos ;
  double current_rev_pos ;

  MotorResponse()=default;
  MotorResponse(const bool success, const unsigned long current_step_pos, const double current_mm_pos, const double current_rev_pos):
    success(success), current_step_pos(current_step_pos), current_mm_pos(current_mm_pos),current_rev_pos(current_rev_pos)
  {}

  MotorResponse(const std::string& str){
    auto params_str = split(str, ';');
    if (params_str.size()==4){
      std::cout<<"params_str does not have the same number of arguments"<<std::endl;
      throw "can't build";
    }
    int succ ;
    std::from_chars(params_str[0].data(),params_str[0].data()+params_str[0].size(),succ);
    this->success = (bool)succ;
    std::from_chars(params_str[1].data(),params_str[1].data()+params_str[1].size(),this->current_step_pos);
    std::from_chars(params_str[2].data(),params_str[2].data()+params_str[2].size(),this->current_mm_pos);
    std::from_chars(params_str[2].data(),params_str[2].data()+params_str[2].size(),this->current_rev_pos);

  }
};


