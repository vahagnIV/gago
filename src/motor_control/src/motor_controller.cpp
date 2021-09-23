//
// Created by vahagn on 22.09.21.
//

#include "../include/motor_controller.h"
#include <serial/setPins.h>
namespace gago {

MotorController::MotorController(ros::NodeHandle &n) :
    pin_client_(
        n.serviceClient<serial::setPins>("set_pins")), speed_(255) {
  ros::Duration d;
  d.fromSec(20);
  if (!pin_client_.waitForExistence(d))
    return;

}

uint32_t MotorController::CreateCommand(uint32_t pin, uint32_t command, uint32_t flags, uint32_t velocity) {
  return pin | (command << 8) | (flags << 16) | (velocity << 24);
}

bool MotorController::Move(motor_control::Move::Request &request, motor_control::Move::Response &response) {

  static uint32_t LeftEnablePin = 3;
  static uint32_t LeftControlPin1 = 4;
  static uint32_t LeftControlPin2 = 2;

  static uint32_t RightEnablePin = 5;
  static uint32_t RightControlPin1 = 7;
  static uint32_t RightControlPin2 = 8;

  if (request.direction == "FORWARD") {

    serial::setPins::Request request;
    serial::setPins::Response response;

    serial::setPins srv;

//    uint32_t command1 = 16843263;
//    srv.request.commands.push_back(command1);
    request.commands.push_back(CreateCommand(LeftControlPin1, 1, 1, 1));
    request.commands.push_back(CreateCommand(LeftControlPin2, 1, 1, 0));
    request.commands.push_back(CreateCommand(LeftEnablePin, 1, 0, speed_));

    request.commands.push_back(CreateCommand(RightControlPin1, 1, 1, 1));
    request.commands.push_back(CreateCommand(RightControlPin2, 1, 1, 0));
    request.commands.push_back(CreateCommand(RightEnablePin, 1, 0, speed_));

//    request.commands.push_back(command2);
    if (pin_client_.call(request, response)) {}
      // TODO: update current state
    else {
      return false;
    }

  }
  return false;
}

bool MotorController::SetSpeed(motor_control::SetSpeed::Request &request, motor_control::SetSpeed::Response &response) {
  return false;
}

void MotorController::Initialize() {

}

}