#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <math.h>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <deque>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ball_chaser/DriveToTarget.h"

// Define a global client that can request services
ros::ServiceClient client;

static const std::string RAW_WINDOW = "color selection";
static const std::string TRACKING_WINDOW = "tracking window";

// default thresholding values for selecting white
cv::Scalar min_thresh = cv::Scalar(0, 0, 250);
cv::Scalar max_thresh = cv::Scalar(5, 5, 255);

cv::Mat cv_hsv;
cv::Mat cv_grey;
cv::Mat thresh_img;
cv::Mat cv_bgr;

// create a queue of the ball centers for drawing a trail
std::deque<cv::Point> center_trail;

// This function calls the command_robot service to drive the robot in the
// specified direction
void drive_robot(float lin_x, float ang_z) {
  ball_chaser::DriveToTarget srv;
  srv.request.linear_x = lin_x;
  srv.request.angular_z = ang_z;

  if (!client.call(srv)) ROS_ERROR("failed to call service");
}

// calculates a range of values to threshold by, clamped at 0 and 180 or 255
std::tuple<int, int> get_min_max(int selected, int upper_bound = 255) {
  int lower = selected - 10;
  int upper = selected + 10;

  if (lower < 0) lower = 0;

  if (upper > upper_bound) upper = upper_bound;

  return std::make_tuple(lower, upper);
}

// processes click data to threshold image by
void onClick(int event, int x, int y, int, void*) {
  if (event != cv::EVENT_LBUTTONDOWN) return;

  // get hsv values at selected pixel
  cv::Vec3b selected_color = cv_hsv.at<cv::Vec3b>(cv::Point(x, y));
  /* std::cout << selected_color << std::endl; */

  // get upper and lower bounds for thresholding
  // hue has a max of 180
  int lower_hue, upper_hue, lower_saturation, upper_saturation, lower_value,
      upper_value;
  std::tie(lower_hue, upper_hue) = get_min_max(selected_color[0], 180);
  std::tie(lower_saturation, upper_saturation) = get_min_max(selected_color[1]);
  std::tie(lower_value, upper_saturation) = get_min_max(selected_color[2]);

  // threshold image
  min_thresh = cv::Scalar(lower_hue, lower_saturation, lower_value);
  max_thresh = cv::Scalar(upper_hue, upper_saturation, upper_value);
  inRange(cv_hsv, min_thresh, max_thresh, thresh_img);
}

int calculate_ball_location(cv_bridge::CvImagePtr input_img) {
  // convert input to OpenCV image
  cv_bgr = input_img->image;
  // convert to hsv
  cv::cvtColor(cv_bgr, cv_hsv, cv::COLOR_BGR2HSV);

  // default threshold to white, gets overwritten by click data
  inRange(cv_hsv, min_thresh, max_thresh, thresh_img);

  // display the raw image for color selection
  cv::imshow(RAW_WINDOW, cv_bgr);
  // threshold based on mouse click
  cv::setMouseCallback(RAW_WINDOW, onClick, 0);

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
  int center_y = 0;
  if (contours.size() > 0) {
    cv::Rect br = cv::boundingRect(contours[0]);
    center_x = br.x + br.width / 2;
    center_y = br.y + br.height / 2;

    // add point to queue
    cv::Point center_pt = cv::Point(center_x, center_y);
    center_trail.push_front(center_pt);

    // draw center of contour on image
    cv::circle(contourOverlay, center_pt, 5, CV_RGB(255, 0, 0), -1);
  }

  // draw trail of ball center
  int trail_length = 50;
  for (int i = 1; i < center_trail.size(); i++) {
    int thickness = static_cast<int>(sqrt(trail_length / (i + 1)) * 2);
    cv::line(contourOverlay, center_trail[i - 1], center_trail[i],
             CV_RGB(125, 125, 125), thickness);
  }
  if (center_trail.size() == trail_length) center_trail.pop_back();

  // display image with contour and center, if there
  cv::imshow(TRACKING_WINDOW, contourOverlay);

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
