#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>

// Pick up location
double pickUpX = -2.0;
double pickUpY = 2.0;
double pickUpZ = 0.0;

// Drop off location
double dropOffX = 2.0;
double dropOffY = 2.0;
double dropOffZ = 0.0;

ros::Publisher marker_pub;

void checkZone(const nav_msgs::Odometry::ConstPtr& msg)
{
  ros::NodeHandle n;

  visualization_msgs::Marker marker;
  // Set the frame ID and timestamp. See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  
  // Set the namespace and id for this marker. This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "basic_shapes";
  marker.id = 0;
  
  // Set the marker type.
  marker.type = visualization_msgs::Marker::CUBE;
  
  // Set the marker action. Options are ADD, DELETE, and new in ROS Indigo: 3
  // (DELETEALL)
  
  // Set the pose of the marker. This is a full 6DOF pose relative to the
  // frame/time specified in the header
  marker.pose.position.x = pickUpX;
  marker.pose.position.y = pickUpY;
  marker.pose.position.z = pickUpZ;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;
  
  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.1;
  marker.scale.y = 0.1;
  marker.scale.z = 0.1;
  
  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;
  
  /* int marker_disp_time = 5; */
  /* marker.lifetime = ros::Duration(marker_disp_time); */
  
  /* marker_pub.publish(marker); */
  /* ROS_INFO("first, sleeping"); */
  /* sleep(marker_disp_time); */

  // Location from odom sensor
  double x_pos =  msg->pose.pose.position.x;
  double y_pos =  msg->pose.pose.position.y;
  double z_pos =  msg->pose.pose.position.z;
  /* ROS_INFO("x: [%f] | y: [%f] | z: [%f]", x_pos, y_pos, z_pos); */
  /* ROS_INFO("x: [%f] [%f] | y: [%f] [%f]", pickUpX * 0.85, pickUpX * 1.15, pickUpY * 0.85, pickUpY * 1.15); */

  if (pickUpX * 1.15 < x_pos && x_pos < pickUpX * 0.05 && y_pos > pickUpY * 0.85 && y_pos < pickUpY * 1.15) {
  /* if (x_pos == pickUpX && y_pos == pickUpY) { */
    /* ROS_INFO("x: [%f] | y: [%f] | z: [%f]", x_pos, y_pos, z_pos); */
    ROS_INFO("AT PICK UP LOCATION");
    marker.action = visualization_msgs::Marker::DELETE;
    marker_pub.publish(marker);
  } else if (x_pos == dropOffX && y_pos == dropOffY) {
    ROS_INFO("AT DROP OFF LOCATION");
  } else {
    ROS_INFO("asdfasd");
    marker.action = visualization_msgs::Marker::ADD;
    marker_pub.publish(marker);
  }



} 

int main(int argc, char **argv) {
  
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);

  // Subscribe to odom from robot
  ros::Subscriber sub = n.subscribe("odom", 10, checkZone);

  // Create publisher for creating markers
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  /* while (ros::ok()) { */
  /*   visualization_msgs::Marker marker; */
  /*   // Set the frame ID and timestamp.  See the TF tutorials for information on */
  /*   // these. */
  /*   marker.header.frame_id = "map"; */
  /*   marker.header.stamp = ros::Time::now(); */

  /*   // Set the namespace and id for this marker.  This serves to create a unique */
  /*   // ID */
  /*   // Any marker sent with the same namespace and id will overwrite the old one */
  /*   marker.ns = "basic_shapes"; */
  /*   marker.id = 0; */

  /*   // Set the marker type.  Initially this is CUBE, and cycles between that and */
  /*   // SPHERE, ARROW, and CYLINDER */
  /*   marker.type = shape; */

  /*   // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 */
  /*   // (DELETEALL) */
  /*   marker.action = visualization_msgs::Marker::ADD; */

  /*   // Set the pose of the marker.  This is a full 6DOF pose relative to the */
  /*   // frame/time specified in the header */
  /*   marker.pose.position.x = -2; */
  /*   marker.pose.position.y = 2; */
  /*   marker.pose.position.z = 0; */
  /*   marker.pose.orientation.x = 0.0; */
  /*   marker.pose.orientation.y = 0.0; */
  /*   marker.pose.orientation.z = 0.0; */
  /*   marker.pose.orientation.w = 1.0; */

  /*   // Set the scale of the marker -- 1x1x1 here means 1m on a side */
  /*   marker.scale.x = 0.1; */
  /*   marker.scale.y = 0.1; */
  /*   marker.scale.z = 0.1; */

  /*   // Set the color -- be sure to set alpha to something non-zero! */
  /*   marker.color.r = 0.0f; */
  /*   marker.color.g = 1.0f; */
  /*   marker.color.b = 0.0f; */
  /*   marker.color.a = 1.0; */

  /*   int marker_disp_time = 5; */
  /*   marker.lifetime = ros::Duration(marker_disp_time); */

  /*   // Publish the marker */
  /*   while (marker_pub.getNumSubscribers() < 1) { */
  /*     if (!ros::ok()) { */
  /*       return 0; */
  /*     } */
  /*     ROS_WARN_ONCE("Please create a subscriber to the marker"); */
  /*     sleep(1); */
  /*   } */
  /*   marker_pub.publish(marker); */
  /*   ROS_INFO("first, sleeping"); */
  /*   sleep(marker_disp_time); */

  /*   ROS_INFO("deleted first, sleeping"); */
  /*   sleep(marker_disp_time); */

  /*   // Move the marker */
  /*   marker.pose.position.x = 2; */
  /*   marker.pose.position.y = 2; */
  /*   marker.pose.position.z = 0; */

  /*   marker.action = visualization_msgs::Marker::ADD; */
  /*   marker_pub.publish(marker); */
  /*   ROS_INFO("second, sleeping"); */
  /*   sleep(marker_disp_time); */

  /*   r.sleep(); */
  /* } */
  ros::spin();
  return 0;
}
