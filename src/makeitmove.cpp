#include "youBotIOHandler.h"
// Ref: http://ibotics.ucsd.edu/trac/stingray/wiki/ROSNodeTutorialC%2B%2B
/**
 * //TODO:
 * - safe shutdown (send all zero to cmd_vel)
 * - Ref: http://answers.ros.org/question/27655/what-is-the-correct-way-to-do-stuff-before-a-node-is-shutdown/
 * 
 * 
 */
int main(int argc, char** argv)
{
	ros::init(argc, argv, "makeitmove");
	ros::NodeHandle n;
	
	YouBotIOHandler* yb = new YouBotIOHandler();
	
	ros::Subscriber sub_ObjDetect = n.subscribe("object_tracking/object_detected", 1000, &YouBotIOHandler::callbackObjDetected, yb);
	ros::Subscriber sub_PosX = n.subscribe("object_tracking/x_pos", 1000, &YouBotIOHandler::callbackPosX, yb);
	ros::Subscriber sub_PosY = n.subscribe("object_tracking/y_pos", 1000, &YouBotIOHandler::callbackPosY, yb);
	ros::Subscriber sub_area = n.subscribe("object_tracking/area", 1000, &YouBotIOHandler::callbackArea, yb);
	ros::Rate r(50);
	
	ros::Publisher pub_moveit = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	
	float x, y;
	float speed = 0.3;
	
	while(n.ok() && ros::ok()){
		if (yb->isObjectDetected()){
			//ROS_INFO("(x, y) = (%d, %d)", yb->getObjX(), yb->getObjY());
			//ROS_INFO("area   = %.2f", yb->getObjArea());
			
			//TODO: convert camera x y values into youbot's angular speed
			//TODO: make this 320x240 values into variables or ROS Parameter
			x = (float)yb->getObjX()/320;
			y = (float)yb->getObjY()/240;
			ROS_INFO("x = %.2f", x);
			
			//TODO: calculate area then give value to move back and forth. or left and right

			yb->m_twist.linear.x = 0;
			//yb->m_twist.linear.y = x * speed;
			yb->m_twist.linear.y = 0;
			yb->m_twist.linear.z = 0;
			
			yb->m_twist.angular.x = 0;
			yb->m_twist.angular.y = 0;
			yb->m_twist.angular.z = x * speed;
		} else {
			yb->setTwistToZeroes();
			
			ROS_INFO("nothing");
		}
		
		yb->publishTwist(&pub_moveit);
		
		ros::spinOnce();
		r.sleep();
		
	}
	
	
	return 0;
}
