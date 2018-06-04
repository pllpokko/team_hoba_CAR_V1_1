#include "my_CV.h"
//get outline using canny
//debugcode
Mat drawLines2(Mat frame, Point &ptl1, Point &ptl2)
{
    Mat line_img(frame.rows,frame.cols,frame.type(), cv::Scalar(0));
    line(line_img, ptl1, ptl2, Scalar(0, 0, 255),3); // draw line in raw image
    //line(line_img, ptr1, ptr2, Scalar(0, 0, 255),3); // draw line in raw image
    addWeighted(frame, 1.0, line_img, 1.0, 0.0, frame);
    return frame;
}
//debug end
Mat myCanny(Mat &frame)
{
    Mat grayImage, edgeImage;
    cvtColor(frame,grayImage,COLOR_BGR2GRAY); // convert color image
	GaussianBlur(grayImage, grayImage, Size(5, 5), 0.5);
    Canny(grayImage,edgeImage,125,350,3); // using canny get outline
    return edgeImage;
}
Mat getROI(Mat& origImag, Point2f topRight, Point2f topLeft, Point2f botLeft, Point2f botRight){
    Mat black(origImag.rows, origImag.cols, origImag.type(), cv::Scalar::all(0));
    Mat mask(origImag.rows, origImag.cols, CV_8UC1, cv::Scalar(0));
    vector< vector<Point> >  co_ordinates;
    co_ordinates.push_back(vector<Point>());
    co_ordinates[0].push_back(topRight);
	co_ordinates[0].push_back(topLeft);
    co_ordinates[0].push_back(botLeft);
    co_ordinates[0].push_back(botRight);
    drawContours( mask,co_ordinates,0, Scalar(255),CV_FILLED, 8 );

    origImag.copyTo(black,mask);
    return black;
}
//hough transform
void myHough(Mat edgeImage, vector<Vec4i> &lines)
{
    Vec4i params;
    int x1,y1,x2,y2;
    HoughLinesP(edgeImage,lines, 1, CV_PI/180.0, 30, 40, 25);// get lines using hough transform
    return;
}
void groupLines(vector<Vec4i> &lines, vector<int> &left_line_x, vector<int> &left_line_y, vector<int> &right_line_x, vector<int> &right_line_y)
{
    Vec4i params;
    int x1,y1,x2,y2;
    double slope;
    int lineLen = lines.size();
    for(int k=0;k<lineLen;k++)
    {
        //grouping!!
        //convert r,theta coordinate or
        //clustring
        params = lines[k]; // straght line information insert param
        x1 = params[0];
        y1 = params[1];
        x2 = params[2];
        y2 = params[3];
        if(x2 != x1)
        {
            slope = ((double)y2 - y1)/(x2-x1); //get slope
			cout<<slope<<endl;//testcode
            if(fabs(slope)>10)
                continue;
            else if(slope<0)    // leftLine
            {
                left_line_x.push_back(x1);
                left_line_x.push_back(x2);
                left_line_y.push_back(y1);
                left_line_y.push_back(y2);
            }
            else    //rightLine
            {
                right_line_x.push_back(x1);
                right_line_x.push_back(x2);
                right_line_y.push_back(y1);
                right_line_y.push_back(y2);
            }
        }
        else
        {
            //slope is infinit
        }
    }
}
void myransac(vector<int> x,vector<int> y,Point &pt1, Point &pt2,int imageCols)
{
    //gradient -> using r, theta
    double gradient, distance, yIC;
    double resultn = log(1-0.99)/log(1-(0.5*0.5));//store n
    int r1,r2;
    int dataNum = x.size();
    int threshold = 20;
    int inlier = 0, inMax = 0;
    int x1,y1,x2,y2;
    if(x.empty() || y.empty())
		return;
	for(int i=0;i<resultn;i++)
    {
        r1 = rand()%(dataNum);
        r2 = ((rand()%(dataNum))+r1)%(dataNum);
		if(r1 == r2 || x[r1] == x[r2])
        {
            i--;
            continue;
        }
        gradient = ((double)(y[r2]-y[r1]))/(x[r2]-x[r1]);
		yIC = y[r1] - (gradient * x[r1]);
		for(int k=0;k<dataNum;k++)
        {
            if(k!=r1 && k!=r2)
            {
                distance = abs(-(x[k]*gradient) - yIC + y[k])/sqrt((gradient*gradient)+1);
                if(distance < threshold)
                {
                    inlier++;
                }
            }
        }
		if(inlier>inMax)
        {
            inMax = inlier;
			/*
            int y1 = imageRows;
            int x1 = (int)((y1-yIC)/gradient);
            int y2 = (imageRows*3)/4;
            int x2 = (int)((y2-yIC)/gradient);
			*/
			int x1 = imageCols-300;
			int y1 = (int)((gradient*x1)+yIC);
			int x2 = imageCols;
			int y2 = (int)((gradient*x2)+yIC);
            pt1 = Point(x1, y1);
            pt2 = Point(x2, y2);
			cout<<"select point : (x1 = "<< x[r1] <<", y1 = "<<y[r1]<<")   ( x2 = "<<x[r2]<<", y2 = "<<y[r2]<<" )"<<endl; 
			cout<<"gradient : "<<gradient<<" yIC : "<<yIC<<endl;//debug
			cout<<"ransac line : "<<x1<<", "<<y1<<"    "<<x2<<", "<<y2<<endl;//debug code
        }
        inlier = 0;

    }
}
double getControl(Point &ptl1, Point &ptl2, Point &ptr1, Point &ptr2)
{
    const static double desired_angle = 90;
    static double current_angle;
    static double error, pre_error;
    static double P_control, I_control, D_control;
    static double del_time;
    static double PID_control;
    static time_t pre_time, cur_time;

    double mid1 = (ptl1.x + ptr1.x)/2.0;
    double mid2 = (ptl2.x + ptr2.x)/2.0;
    current_angle = atan2((ptl1.y - ptl2.y), (mid2 - mid1)) * 180.0/PI;
	PID_control = current_angle;
	/*
    if(control_cnt) // first_time
    {
        PID_control = desired_angle - current_angle;
        time(&pre_time);
        pre_error = desired_angle - current_angle;
        control_cnt = 0;
    }
    else //first time
    {
        time(&cur_time);
        del_time = difftime(cur_time,pre_time);
        time(&pre_time);

        error = desired_angle - current_angle;
        P_control = Kp * error;
        I_control += Ki * error * del_time;
        D_control = Kd*(error - pre_error)/del_time;
        PID_control = P_control + I_control + D_control;
        pre_error = error;
    }
	**/
    return PID_control;
}
Mat drawLines(Mat frame, Point &ptl1, Point &ptl2, Point &ptr1, Point &ptr2)
{
    Mat line_img(frame.rows,frame.cols,frame.type(), cv::Scalar(0));
    line(line_img, ptl1, ptl2, Scalar(0, 0, 255),3); // draw line in raw image
    line(line_img, ptr1, ptr2, Scalar(0, 0, 255),3); // draw line in raw image
    addWeighted(frame, 0.8, line_img, 1.0, 0.0, frame);
    return frame;
}
int imageProcess(Mat &frame)
{
    vector<Vec4i> lines; // get line information in vector
    vector<int> left_line_x;
    vector<int> left_line_y;
    vector<int> right_line_x;
    vector<int> right_line_y;
    Point ptl1, ptl2, ptr1, ptr2;
    Mat roiImage, houghImage, edgeImage, linedImage;
    /*//hsv start
    Scalar hsv_black_min = cvScalar(41, 0, 0);
    Scalar hsv_black_max = cvScalar(230, 255, 115);
    cvtColor(frame, hsvImage, COLOR_BGR2HSV); // frame -> hsvImage�� HSV��
    inRange(hsvImage, hsv_black_min, hsv_black_max, hsvImage);//hsv->dstImage �÷�����
    //hsv end*/
	edgeImage = myCanny(frame);
    roiImage = getROI(edgeImage,Point2f(frame.cols, frame.rows/2),Point2f(0, frame.rows/2),Point2f(0,frame.rows),Point2f(frame.cols,frame.rows));
    myHough(roiImage, lines);
	/*//debug
		houghImage = frame;
		for(int i=0;i<lines.size();i++)
		{
			Vec4i param = lines[i];
			const int x1= param[0], y1=param[1], x2=param[2], y2=param[3];
			Point p1(x1,y1), p2(x2,y2);
			houghImage = drawLines2(houghImage,p1,p2);
		}
	//end */
    groupLines(lines, left_line_x, left_line_y, right_line_x, right_line_y);
	//RANSAC
    myransac(left_line_x,left_line_y,ptl1, ptl2, 300);
    myransac(right_line_x,right_line_y,ptr1, ptr2, frame.cols);
    linedImage = drawLines(frame, ptl1, ptl2, ptr1, ptr2);
	/*******testcode**************************************
	Mat halfedge, halfroi, halfline;
	resize(edgeImage,halfedge,Size(edgeImage.cols/2,edgeImage.rows/2));
	resize(roiImage,halfroi,Size(roiImage.cols/2,roiImage.rows/2));
	resize(linedImage,halfline,Size(linedImage.cols/2,linedImage.rows/2));
	imshow("edge",halfedge);
	imshow("roi",halfroi);
	imshow("line",halfline);

	// *******testcode**************************************/
    //imshow("edge",edgeImage);
	//imshow("roi",roiImage);
	imshow("roi",roiImage);
	imshow("line",linedImage);
	//imshow("hough",houghImage);
    double steeringInfo = getControl(ptl1, ptl2, ptr1, ptr2);
	waitKey();
    return (int)steeringInfo;
}
