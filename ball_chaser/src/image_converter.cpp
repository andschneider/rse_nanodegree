#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string RAW_WINDOW = "raw window";
static const std::string CONVERTED_WINDOW = "converted window";
static const std::string THRESHOLD_WINDOW= "threshold window";

cv::Mat cv_hsv;
cv::Mat cv_grey;
cv::Mat thresh_img;
cv::Mat cv_bgr;


class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(RAW_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(RAW_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    // convert input to OpenCV image
    cv_bgr = cv_ptr->image;
    // convert to hsv
    cv::cvtColor(cv_bgr, cv_hsv, cv::COLOR_BGR2HSV);
    // convert to grey scale
    cv::cvtColor(cv_bgr, cv_grey, cv::COLOR_BGR2GRAY);

    // threshold based on a given range
    /* inRange(cv_hsv, cv::Scalar(0,0,250), cv::Scalar(6,6,255), thresh_img); */
    /* inRange(cv_grey, cv::Scalar(250,250,250), cv::Scalar(255,255,255), thresh_img); */
    inRange(cv_bgr, cv::Scalar(250,250,250), cv::Scalar(255,255,255), thresh_img);

    cv::imshow(RAW_WINDOW, cv_ptr->image);
    cv::imshow(CONVERTED_WINDOW, cv_grey);
    /* cv::imshow(CONVERTED_WINDOW, cv_hsv); */
    cv::imshow(THRESHOLD_WINDOW, thresh_img);


    // Draw an example circle on the video stream
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    // Update GUI Window
    /* cv::imshow(RAW_WINDOW, cv_ptr->image); */
    cv::waitKey(3);

    // Output modified video stream
    /* image_pub_.publish(cv_ptr->toImageMsg()); */
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
