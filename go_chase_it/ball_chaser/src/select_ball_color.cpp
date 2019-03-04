#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "std_msgs/ColorRGBA.h"

// Define a global client that can request services
ros::ServiceClient client;
ros::Publisher color_publisher;

static const std::string RAW_WINDOW = "color selection";

cv::Mat cv_hsv;
cv::Mat cv_grey;
cv::Mat cv_bgr;

// processes click data to threshold image by
void onClick(int event, int x, int y, int, void*) {
  if (event != cv::EVENT_LBUTTONDOWN) return;

  // get hsv values at selected pixel
  cv::Vec3b selected_color = cv_hsv.at<cv::Vec3b>(cv::Point(x, y));
  /* std::cout << selected_color << std::endl; */
  ROS_INFO("Selected HSV color from opencv image. h: %d, s: %d, v: %d",
           selected_color[0], selected_color[1], selected_color[2]);

  std_msgs::ColorRGBA select_hsv;
  select_hsv.r = selected_color[0];
  select_hsv.g = selected_color[1];
  select_hsv.b = selected_color[2];
  select_hsv.a = 1;

  color_publisher.publish(select_hsv);
}

void calculate_ball_location(cv_bridge::CvImagePtr input_img) {}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::ImageConstPtr& msg) {
  cv_bridge::CvImagePtr cv_ptr;
  try {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }

  // convert input to OpenCV image
  cv_bgr = cv_ptr->image;

  // convert to hsv
  cv::cvtColor(cv_bgr, cv_hsv, cv::COLOR_BGR2HSV);

  // display the raw image for color selection
  cv::imshow(RAW_WINDOW, cv_bgr);

  cv::waitKey(3);
  // publish selected hsv values
  cv::setMouseCallback(RAW_WINDOW, onClick, 0);
}

int main(int argc, char** argv) {
  // Initialize the process_image node and create a handle to it
  ros::init(argc, argv, "select_ball_color");
  ros::NodeHandle nh;

  color_publisher =
      nh.advertise<std_msgs::ColorRGBA>("/ball_chaser/selected_color", 10);

  // Subscribe to /camera/rgb/image_raw topic to read the image data inside the
  // process_image_callback function
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub =
      it.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

  // Handle ROS communication events
  ros::spin();
  return 0;
}
