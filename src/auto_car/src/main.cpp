#include "my_ZED.h"
#include "my_CV.h"
#include "ros/ros.h"
#include <cstdio>
#include "auto_car/MsgControl.h"

#ifndef __CV_STD_NAMESPACE__
#define __CV_STD_NAMESPACE__
using namespace cv;
using namespace std;
#endif

int main(int argc,char** argv)
{
	/***************************camera
    sl::Camera zed;
    sl::InitParameters init_params;
    // Set runtime parameters after opening the camera
    sl::RuntimeParameters runtime_parameters;
    zed_init(init_params, zed, runtime_parameters);
	***********************************/
	///**************************picture
	
	//**********************************/
    ros::init(argc,argv,"TX2");
    ros::NodeHandle nh;
    ros::Publisher talker = nh.advertise<auto_car::MsgControl>("ros_msg",100);
    auto_car::MsgControl msg;

    sl::Mat image_zed;
    cv::Mat image_origin;
    int steeringInfo;
	image_origin = imread("/home/nvidia/Documents/chije1.png");
    while(true)
    {
        //image_origin=getCVImage(zed, runtime_parameters, image_zed);
        if(!image_origin.empty())
        {
            steeringInfo = imageProcess(image_origin);
            cout<<"wheel will go "<<steeringInfo<<"degree\n";
            msg.data = steeringInfo;
            talker.publish(msg);
			while(1);
        }
    }
}
