#ifndef __MY_CV_H__
#define __MY_CV_H__
#define PI 3.14159265

#include <cmath>
#include <vector>
#include <ctime>
#include "opencv2/opencv.hpp"

/******************************
//pid control
********************************/
#define Kp 1
#define Ki 1
#define Kd 1
/******************************
//end pid control
********************************/
#ifndef __CV_STD_NAMESPACE__
#define __CV_STD_NAMESPACE__
#include <iostream>
int control_cnt=1;
using namespace cv;
using namespace std;
#endif

Mat myCanny(Mat &frame);
Mat getROI(Mat& origImag, Point2f top, Point2f botLeft, Point2f botRight);
void myHough(Mat edgeImage, vector<Vec4i> &lines);
void groupLines(vector<Vec4i> &lines, vector<int> &left_line_x, vector<int> &left_line_y, vector<int> &right_line_x, vector<int> &right_line_y);
void myransac(vector<int> x,vector<int> y,Point &pt1, Point &pt2,int imageRows);
double getControl(Point &ptl1, Point &ptl2, Point &ptr1, Point &ptr2);
Mat drawLines(Mat frame, Point &ptl1, Point &ptl2, Point &ptr1, Point &ptr2);
int imageProcess(Mat &frame);
#endif
