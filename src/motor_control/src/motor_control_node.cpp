//
// Created by vahagn on 22.09.21.
//
#include <ros/ros.h>
#include "motor_controller.h"

int main(int argc, char *argv[]) {


  ros::init(argc, argv, "MotorControlNode");
  ros::NodeHandle n;
  gago::MotorController controller(n);

  ros::ServiceServer set_speed = n.advertiseService("SetSpeed", &gago::MotorController::SetSpeed, &controller);
  ros::ServiceServer move = n.advertiseService("Move", &gago::MotorController::Move, &controller);
  ROS_INFO("Ready to move motor.");
  ros::spin();
  return 0;
}