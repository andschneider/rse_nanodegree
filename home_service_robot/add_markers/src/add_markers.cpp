#include <math.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

// Pick up location
double pick_up_x = -1.7;
double pick_up_y = 2.5;
double pick_up_z = 0.0;

// Drop off location
double drop_off_x = 2.5;
double drop_off_y = 1.0;
double drop_off_z = 0.0;

ros::Publisher marker_pub;

// Set up marker status
enum marker_status { AVAILABLE, PICKEDUP, DROPPED };
marker_status markerStatus;

// Calculate the Euclidean distance between a desired point and the actual point
double CalcDistance(double x_coord, double y_coord, double x_desired,
                    double y_desired) {
  double distance =
      sqrt(pow(x_coord - x_desired, 2) + pow(y_coord - y_desired, 2));
  return distance;
}

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

  // Location from odom sensor, which is from the coordinate system of the robot
  double x_pos = msg->pose.pose.position.x;
  double y_pos = msg->pose.pose.position.y;
  double z_pos = msg->pose.pose.position.z;
  /* ROS_INFO("x: [%f] | y: [%f] | z: [%f]", x_pos, y_pos, z_pos); */

  // Calculate distance to pick up locations. The positions are seen from the
  // robot's coordinate system so they have different locations than the marker
  // locations
  double pick_up_distance = CalcDistance(x_pos, y_pos, -2.4, -1.7);
  double drop_off_distance = CalcDistance(x_pos, y_pos, -0.7, 2.5);
  /* ROS_INFO("pick up distance: [%f] | drop off: [%f]", pick_up_distance,
   * drop_off_distance); */
  /* ROS_INFO("[%d]", markerStatus); */

  if (pick_up_distance < 0.15) {
    ROS_INFO("Picking up the item!");
    sleep(5);
    markerStatus = PICKEDUP;
  } else if (drop_off_distance < 0.15) {
    ROS_INFO("Dropping off the item!");
    sleep(5);
    markerStatus = DROPPED;
  }

  // Determine where the marker should be placed
  switch (markerStatus) {
    case AVAILABLE:
      marker.pose.position.x = pick_up_x;
      marker.pose.position.y = pick_up_y;
      marker.pose.position.z = pick_up_z;
      marker.action = visualization_msgs::Marker::ADD;
      marker_pub.publish(marker);
      break;
    case PICKEDUP:
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      break;
    case DROPPED:
      marker.pose.position.x = drop_off_x;
      marker.pose.position.y = drop_off_y;
      marker.pose.position.z = drop_off_z;
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
