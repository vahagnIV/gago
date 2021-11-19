//
// Created by vahagn on 22.09.21.
//

#include "../include/motor_controller.h"

namespace gago {

MotorController::MotorController(ros::ServiceClient &pin_client) :
    pin_client_(pin_client), speed_(255) {

  // Defaults
  right_pin_set_.fwd_pwn_pin = 3;
  right_pin_set_.bcw_pwn_pin = 5;

  left_pin_set_.fwd_pwn_pin = 6;
  left_pin_set_.bcw_pwn_pin = 9;

}

uint32_t MotorController::CreateCommand(uint32_t pin, uint32_t command, uint32_t flags, uint32_t velocity) {
  return pin | (command << 8) | (flags << 16) | (velocity << 24);
}

bool MotorController::Move(motor_control::Move::Request &request, motor_control::Move::Response &response) {

  serial::setPins::Request request_commands;
  serial::setPins::Response response_messages;
  if (request.direction == "FORWARD") {
    FillForwardCommands(right_pin_set_, request_commands);
    FillForwardCommands(left_pin_set_, request_commands);
  } else if (request.direction == "BACKWARD") {
    FillBackwardCommands(right_pin_set_, request_commands);
    FillBackwardCommands(left_pin_set_, request_commands);
  } else if (request.direction == "LEFT") {
    FillBackwardCommands(left_pin_set_, request_commands);
    FillForwardCommands(right_pin_set_, request_commands);
  } else if (request.direction == "RIGHT") {
    FillForwardCommands(left_pin_set_, request_commands);
    FillBackwardCommands(right_pin_set_, request_commands);
  } else if (request.direction == "STOP") {
    request_commands.commands.push_back(CreateCommand(right_pin_set_.fwd_pwn_pin, 1, 1, 0));
    request_commands.commands.push_back(CreateCommand(right_pin_set_.bcw_pwn_pin, 1, 1, 0));
    request_commands.commands.push_back(CreateCommand(left_pin_set_.fwd_pwn_pin, 1, 1, 0));
    request_commands.commands.push_back(CreateCommand(left_pin_set_.bcw_pwn_pin, 1, 1, 0));
  }

  // TODO: examine the response and log errors if necessary
  return pin_client_.call(request_commands, response_messages);

}

void MotorController::FillForwardCommands(const MotorPinSet &pin_set, serial::setPins::Request &request) const {
//  request.commands.push_back(CreateCommand(pin_set.control_pin1, 1, 1, 1));
  request.commands.push_back(CreateCommand(pin_set.bcw_pwn_pin, 1, 1, 0));
  request.commands.push_back(CreateCommand(pin_set.fwd_pwn_pin, 1, 0, speed_));

}

void MotorController::FillBackwardCommands(const MotorPinSet &pin_set, serial::setPins::Request &request) const {
//  request.commands.push_back(CreateCommand(pin_set.control_pin1, 1, 1, 0));
  request.commands.push_back(CreateCommand(pin_set.fwd_pwn_pin, 1, 1, 0));
  request.commands.push_back(CreateCommand(pin_set.bcw_pwn_pin, 1, 0, speed_));
}

void MotorController::SetLeftPinSet(MotorPinSet &left) {
  left_pin_set_ = left;
}

void MotorController::SetRightPinSet(MotorPinSet &right) {
  right_pin_set_ = right;
}

bool MotorController::SetSpeed(motor_control::SetSpeed::Request &request,
                               motor_control::SetSpeed::Response &response) {
  speed_ = request.speed;
  return true;
}

}