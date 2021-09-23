//
// Created by vahagn on 22.09.21.
//

#ifndef SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_
#define SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_

#include <ros/ros.h>
#include "motor_control/Move.h"
#include "motor_control/SetSpeed.h"

namespace gago {

class MotorController {
 public:
  MotorController(ros::NodeHandle &n);

  bool Move(motor_control::Move::Request &request, motor_control::Move::Response &response);
  bool SetSpeed(motor_control::SetSpeed::Request &request, motor_control::SetSpeed::Response &response);

  void Initialize();
 private:
  uint32_t CreateCommand(uint32_t pin, uint32_t command, uint32_t flags, uint32_t velocity);
  ros::ServiceClient pin_client_;
  uint8_t speed_;

};

}

#endif //SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_
