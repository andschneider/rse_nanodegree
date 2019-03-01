#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("failed to call service");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
	/* drive_robot(0.0, 0.5); */
    // define the max position for a pixel for it to be in the left, center, or right third of an image
    int left_img_max = img.step / 3;
    int center_img_max = (2 * img.step) / 3;
    int right_img_max = img.step;

	/* ROS_INFO("left %d, center %d, right %d", left_img_max, center_img_max, right_img_max); */
    for (int i=0; i < img.height * img.step; i++) {
	    if (img.data[i] == 255) {
			int test = i / img.height;
			/* int test = i / img.step; */
			ROS_INFO("%d, %d, %d", i, img.data[i], test);
			break;
		}
	}
	bool drive = false;
	int direction = 0;
    /* for (int i = 0; i < img.height; i++) { */
        /* for (int j = 0; j < img.step; j++) { */
			/* ROS_INFO("%d, %d", i, j); */
			/* int pixel = i * j; */
			/* if (img.data[pixel] == white_pixel) { */
			/* 	int test = pixel / img.height; */
			/* 	ROS_INFO("%d, %d,%d, %d", pixel, i, j, test); */
			/* 	break; */
			/* } */
		/* } */
	/* } */
	/* 		int red = img.data[pixel - 2]; */
	/* 		int grn = img.data[pixel - 1]; */
	/* 		int blu = img.data[pixel]; */
	/*       	// losing accuracy by not using a float, but only looking for 255, so it's ok */
	/* 		int rgb = (red + grn + blu) / 3; */
	/* 		/1* ROS_INFO("%d, %d, %d, %d, %d, %d", i, j, red, grn, blu, rgb); *1/ */
            /* if (rgb == white_pixel) { */
	/* 			ROS_INFO("%d, %d, %d", rgb, j, i*j); */
                /* if (j < left_img_max) { */
	/* 				ROS_INFO("left"); */
	/* 				direction = -1; */
	/* 				drive = true; */
                    /* /1* drive_robot(0.0, 0.5); *1/ */
                /* } else if (j < center_img_max) { */
	/* 				ROS_INFO("center"); */
	/* 				direction = 0; */
	/* 				drive = true; */
                    /* /1* drive_robot(0.5, 0.0); *1/ */
                /* } else { */
	/* 				ROS_INFO("right"); */
	/* 				direction = 1; */
	/* 				drive = true; */
                    /* /1* drive_robot(0.0, -0.5); *1/ */
                /* } */
            /* } */ 
	/* 		else { */
	/* 			drive = false; */
	/* 		} */
        /* } */
    /* } */
	
	if (drive == true) 	
		ROS_INFO("driving %d", direction);

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
	/* ros::Rate r(10); // 10 hz */
    /* ros::spinOnce(); */
	/* r.sleep(); */
	ros::spin();
	
    return 0;
}
