//
// Created by vahagn on 22.09.21.
//
#include <ros/ros.h>
#include "motor_controller.h"
#include <serial/setPins.h>

int main(int argc, char *argv[]) {

  ros::init(argc, argv, "MotorControlNode");
  ros::NodeHandle n;

  ros::ServiceClient pin_client = n.serviceClient<serial::setPins>("set_pins");

  ros::Duration d;
  d.fromSec(20);
  if (!pin_client.waitForExistence(d)) {
    ROS_ERROR("set_pins service is not available.");
    return 1;
  }

  gago::MotorController controller(pin_client);

  if (n.hasParam("/motor_control/left/enable") && n.hasParam("/motor_control/left/pin1")
      && n.hasParam("/motor_control/left/pin2")) {
    gago::MotorPinSet left_pin_set;
    int tmp;
    n.getParam("/motor_control/left/enable", tmp);
    left_pin_set.enable_pin = tmp;

    n.getParam("/motor_control/left/pin1", tmp);
    left_pin_set.control_pin1 = tmp;

    n.getParam("/motor_control/left/pin2", tmp);
    left_pin_set.control_pin2 = tmp;

    controller.SetLeftPinSet(left_pin_set);
  }

  if (n.hasParam("/motor_control/right/enable") && n.hasParam("/motor_control/right/pin1")
  && n.hasParam("/motor_control/right/pin2")) {
    gago::MotorPinSet right_pin_set;
    int tmp;
    n.getParam("/motor_control/right/enable", tmp);
    right_pin_set.enable_pin = tmp;

    n.getParam("/motor_control/right/pin1", tmp);
    right_pin_set.control_pin1 = tmp;

    n.getParam("/motor_control/right/pin2", tmp);
    right_pin_set.control_pin2 = tmp;

    controller.SetRightPinSet(right_pin_set);
  }

  ros::ServiceServer set_speed = n.advertiseService("SetSpeed", &gago::MotorController::SetSpeed, &controller);
  ros::ServiceServer move = n.advertiseService("Move", &gago::MotorController::Move, &controller);
  ROS_INFO("Ready to move motor.");
  ros::spin();
  return 0;
}