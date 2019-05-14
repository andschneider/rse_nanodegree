#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <ros/ros.h>

// Define a client for to send goal requests to the move_base server through a
// SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>
    MoveBaseClient;

int main(int argc, char** argv) {
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "simple_navigation_goals");

  // tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while (!ac.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  // pick up location
  move_base_msgs::MoveBaseGoal pickUp;
  pickUp.target_pose.header.frame_id = "map";
  pickUp.target_pose.header.stamp = ros::Time::now();
  pickUp.target_pose.pose.position.x = -2.0;
  pickUp.target_pose.pose.position.y = 2.0;
  pickUp.target_pose.pose.orientation.w = 1.0;

  // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending pick up location");
  ac.sendGoal(pickUp);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached pick up location
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("Made it to the pick up location!!!!");
    // sleep for five seconds
    ros::Duration(5.0).sleep();
  } else
    ROS_INFO("Did not make it to the pick up location for some reason");

  // drop off location
  move_base_msgs::MoveBaseGoal dropOff;
  dropOff.target_pose.header.frame_id = "map";
  dropOff.target_pose.header.stamp = ros::Time::now();
  dropOff.target_pose.pose.position.x = 2.0;
  dropOff.target_pose.pose.position.y = 2.0;
  dropOff.target_pose.pose.orientation.w = 1.0;

  // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending drop off location");
  ac.sendGoal(dropOff);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached drop off location
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("Made it to the drop off location!!!!");
    // sleep for five seconds
    ros::Duration(5.0).sleep();
  } else
    ROS_INFO("Did not make it to the drop off location for some reason");

  return 0;
}
