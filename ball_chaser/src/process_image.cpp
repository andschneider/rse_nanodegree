#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ball_chaser/DriveToTarget.h"

// Define a global client that can request services
ros::ServiceClient client;

static const std::string RAW_WINDOW = "raw window";
static const std::string CONVERTED_WINDOW = "converted window";
static const std::string THRESHOLD_WINDOW = "threshold window";
static const std::string CONTOUR_WINDOW = "contour window";

cv::Mat cv_hsv;
cv::Mat cv_grey;
cv::Mat thresh_img;
cv::Mat cv_bgr;

// This function calls the command_robot service to drive the robot in the
// specified direction
void drive_robot(float lin_x, float ang_z) {
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  if (!client.call(srv)) ROS_ERROR("failed to call service");
}

int calculate_ball_location(cv_bridge::CvImagePtr input_img) {
  // convert input to OpenCV image
  cv_bgr = input_img->image;
  // convert to hsv
  cv::cvtColor(cv_bgr, cv_hsv, cv::COLOR_BGR2HSV);
  // threshold based on a given range
  cv::Scalar thresh_min = cv::Scalar(0, 0, 250);
  cv::Scalar thresh_max = cv::Scalar(6, 6, 255);
  inRange(cv_hsv, thresh_min, thresh_max, thresh_img);

  /* cv::imshow(RAW_WINDOW, cv_ptr->image); */
  /* cv::imshow(CONVERTED_WINDOW, cv_hsv); */
  /* cv::imshow(THRESHOLD_WINDOW, thresh_img); */

  // find edge of ball
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(thresh_img, contours, hierarchy, CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // draw a contour to a copy of the input image
  cv::Mat contourOverlay = cv_bgr.clone();
  cv::drawContours(contourOverlay, contours, -1, CV_RGB(0, 0, 255), 2, 8,
                   hierarchy, 0);

  // find center of contour
  int center_x = 0;
  if (contours.size() > 0) {
    cv::Rect br = cv::boundingRect(contours[0]);
    center_x = br.x + br.width / 2;
    int center_y = br.y + br.height / 2;
    // draw center of contour on image
    cv::circle(contourOverlay, cv::Point(center_x, center_y), 5,
               CV_RGB(255, 0, 0), -1);
  }

  // display image with contour and center, if there
  cv::imshow(CONTOUR_WINDOW, contourOverlay);

  // figure out location of ball in the image and set driving direction
  enum drive_direction { STOP = -1, RIGHT = 1, CENTER, LEFT };
  int img_width = cv_bgr.cols;

  drive_direction driving;
  if (center_x == 0)
    driving = STOP;
  else if (center_x < (img_width / 3))
    driving = LEFT;
  else if (center_x < (img_width / 3) * 2)
    driving = CENTER;
  else if (center_x <= img_width)
    driving = RIGHT;

  /* std::cout << driving << std::endl; */

  cv::waitKey(3);
  return driving;
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::ImageConstPtr& msg) {
  cv_bridge::CvImagePtr cv_ptr;
  try {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }

  int direction = calculate_ball_location(cv_ptr);

  // stop
  if (direction == -1) drive_robot(0.0, 0.0);
  // right
  else if (direction == 1)
    drive_robot(0.0, -0.5);
  // center
  else if (direction == 2)
    drive_robot(0.5, 0.0);
  // left
  else if (direction == 3)
    drive_robot(0.0, 0.5);
}

int main(int argc, char** argv) {
  // Initialize the process_image node and create a handle to it
  ros::init(argc, argv, "process_image");
  ros::NodeHandle nh;

  // Define a client service capable of requesting services from command_robot
  client = nh.serviceClient<ball_chaser::DriveToTarget>(
      "/ball_chaser/command_robot");

  // Subscribe to /camera/rgb/image_raw topic to read the image data inside the
  // process_image_callback function
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub =
      it.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

  // Handle ROS communication events
  ros::spin();
  return 0;
}
