//
// Created by vahagn on 20.09.21.
//

#include <iostream>
#include <thread>
#include <queue>
#include "ros/ros.h"
#include "serial/setPins.h"
#include "pin_manager.h"

bool execute(serial::setPins::Request &request, serial::setPins::Request &response) {
  return true;
}

int main(int argc, char *argv[]) {
  std::cout << argv[argc - 1] << std::endl;
  serial::PinManager manager("/dev/ttyACM0");
  if(!manager.Initialize())
    return 1;
  ros::init(argc, argv, "NativeSerialService");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("set_pins", &serial::PinManager::Execute, &manager);
  ROS_INFO("Ready to set pins.");
  ros::spin();
  return 0;
}