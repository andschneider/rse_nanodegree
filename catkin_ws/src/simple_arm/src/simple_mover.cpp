#include "ros/ros.h"
#include "std_msgs/Float64.h"

int main(int argc, char** argv)
{
    // initialize the arm_mover node
    ros::init(argc, argv, "arm_mover");

    // create a handle to the arm_mover node
    ros::NodeHandle n;

    // create a publisher that can publish a std_msgs::Float64 message on the /simple_arm/joint_1_position_controller/command topic
    ros::Publisher joint1_pub = n.advertise<std_msgs::Float64>("/simple_arm/joint_1_position_controller/command", 10);
    // create a publisher that can publish a std_msgs::Float64 message on the /simple_arm/joint_2_position_controller/command topic
    ros::Publisher joint2_pub = n.advertise<std_msgs::Float64>("/simple_arm/joint_2_position_controller/command", 10);

    // set loop frequency of 10Hz
    ros::Rate loop_rate(10);

    int start_time, elapsed;

    // get ROS start time
    while (not start_time) {
        start_time = ros::Time::now().toSec();
    }

    while (ros::ok()) {
        // get ROS elapsed time
        elapsed = ros::Time::now().toSec() - start_time;

        // set the arm joint angles
        std_msgs::Float64 joint1_angle, joint2_angle;
        joint1_angle.data = sin(2 * M_PI * 0.1 * elapsed) * (M_PI / 2);
        joint2_angle.data = sin(2 * M_PI * 0.1 * elapsed) * (M_PI / 2);

        // publish the arm joint angles
        joint1_pub.publish(joint1_angle);
        joint2_pub.publish(joint2_angle);

        // sleep for the time remaining until 10Hz is reached
        loop_rate.sleep();
    }

    return 0;
}
