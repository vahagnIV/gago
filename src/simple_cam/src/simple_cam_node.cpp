//
// Created by vahagn on 22/11/2021.
//

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

int main(int argc, char ** argv) {
  ros::init(argc, argv, "ImagePublisherNode");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("OrbImage", 1);
  cv::VideoCapture c(0, cv::CAP_V4L2);
  cv::Mat image;

  ros::Rate loop_rate(60);
  while (nh.ok()) {
    if (!c.read(image))
      continue;
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(320,240));
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", resized).toImageMsg();
    pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
