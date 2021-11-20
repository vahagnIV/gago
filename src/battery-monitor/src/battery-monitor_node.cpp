//
// Created by vahagn on 20.11.21.
//
#include <ros/ros.h>
#include <std_msgs/Byte.h>
#include <fstream>
#include <thread>



void ppp() {

}

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "BatteryMonitorNode");
  ros::NodeHandle n;
  ros::Publisher publisher = n.advertise<std_msgs::Byte>("battery_monitor", 16);
  ROS_INFO("Ready to set pins.");

  while(ros::ok()){

      std::ifstream ifstream("/sys/class/power_supply/BAT0/capacity");
      std::string r((std::istream_iterator<char>(ifstream)), std::istream_iterator<char>());
      std_msgs::Byte msg;
      msg.data = atoi(r.c_str());
      publisher.publish(msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  ros::spin();
  return 0;

}