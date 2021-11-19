//
// Created by vahagn on 22.09.21.
//

#ifndef SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_
#define SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_

#include <ros/ros.h>
#include "motor_control/Move.h"
#include "motor_control/SetSpeed.h"
#include <serial/setPins.h>

namespace gago {

struct MotorPinSet {
  uint8_t fwd_pwn_pin;
  uint8_t bcw_pwn_pin;
};

class MotorController {
 public:
  MotorController(ros::ServiceClient &pin_client);

  bool Move(motor_control::Move::Request &request, motor_control::Move::Response &response);
  bool SetSpeed(motor_control::SetSpeed::Request &request, motor_control::SetSpeed::Response &response);
  void SetLeftPinSet(MotorPinSet &left);
  void SetRightPinSet(MotorPinSet &right);

 private:
  static uint32_t CreateCommand(uint32_t pin, uint32_t command, uint32_t flags, uint32_t velocity);
  void FillForwardCommands(const MotorPinSet &pin_set, serial::setPins::Request &request) const;
  void FillBackwardCommands(const MotorPinSet &pin_set, serial::setPins::Request &request) const;

 private:

  ros::ServiceClient pin_client_;
  uint8_t speed_;

  MotorPinSet right_pin_set_;
  MotorPinSet left_pin_set_;

};

}

#endif //SRC_MOTOR_CONTROL_INCLUDE_MOTOR_CONTROLLER_H_
