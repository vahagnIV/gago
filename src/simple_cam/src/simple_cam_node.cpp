//
// Created by vahagn on 22/11/2021.
//

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("OrbImage", 1);
  cv::VideoCapture c(0);
  cv::Mat image;

  ros::Rate loop_rate(60);
  while (nh.ok()) {
    if (!c.read(image))
      continue;
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", image).toImageMsg();
    pub.publish(msg);
    loop_rate.sleep();
  }
}
