#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

// Pick up location
double pickUpX = -1.7;
double pickUpY = 2.5;
double pickUpZ = 0.0;

// Drop off location
double dropOffX = 2.5;
double dropOffY = 1.0;
double dropOffZ = 0.0;

ros::Publisher marker_pub;

// Set up marker status
enum marker_status { AVAILABLE, PICKEDUP, DROPPED };
marker_status markerStatus;

void moveMarker(const nav_msgs::Odometry::ConstPtr& msg) {
  // Create a marker
  visualization_msgs::Marker marker;

  // Set the frame ID and timestamp.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();

  // Set the namespace and id for this marker. This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "basic_shapes";
  marker.id = 0;

  // Set the marker type.
  marker.type = visualization_msgs::Marker::CUBE;

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.3;
  marker.scale.y = 0.3;
  marker.scale.z = 0.3;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  // Location from odom sensor
  double x_pos = msg->pose.pose.position.x;
  double y_pos = msg->pose.pose.position.y;
  double z_pos = msg->pose.pose.position.z;
  /* ROS_INFO("x: [%f] | y: [%f] | z: [%f]", x_pos, y_pos, z_pos); */

  /* marker_status markerStatus; */
  /* if (x_pos == pickUpX && y_pos == pickUpY) { */
  /* else if (x_pos == dropOffX && y_pos == dropOffY) { */
  // TODO probably won't need these ranges
  if (-2.4 * 1.15 < x_pos && x_pos < -2.4 * 0.85 &&
      y_pos > -1.7 * 1.15 && y_pos < -1.7 * 0.85) {
    ROS_INFO("Picking up the item!");
    sleep(5);
    markerStatus = PICKEDUP;
  } else if (-0.7 * 1.15 < x_pos && x_pos < -0.7 * 0.85 &&
             y_pos > 2.5 * 0.85 && y_pos < 2.5 * 1.15) {
    ROS_INFO("Dropping off the item!");
    sleep(5);
    markerStatus = DROPPED;
  }

  /* ROS_INFO("[%d]", markerStatus); */
  // Determine where the marker should be placed
  switch (markerStatus) {
    case AVAILABLE:
      marker.pose.position.x = pickUpX;
      marker.pose.position.y = pickUpY;
      marker.pose.position.z = pickUpZ;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
      break;
    case PICKEDUP:
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      break;
    case DROPPED:
      marker.pose.position.x = dropOffX;
      marker.pose.position.y = dropOffY;
      marker.pose.position.z = dropOffZ;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
      break;
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);

  // Subscribe to odom from robot
  ros::Subscriber sub = n.subscribe("odom", 10, moveMarker);

  // Create publisher for creating markers
  marker_pub =
      n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  ros::spin();
  return 0;
}
