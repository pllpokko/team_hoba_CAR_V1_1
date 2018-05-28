#include "my_CV.h"
//get outline using canny
Mat myCanny(Mat &frame)
{
    Mat grayImage, edgeImage;
    cvtColor(frame,grayImage,COLOR_BGR2GRAY); // convert color image
    Canny(grayImage,edgeImage,125,250,3); // using canny get outline
    return edgeImage;
}
Mat getROI(Mat& origImag, Point2f top, Point2f botLeft, Point2f botRight){
    Mat black(origImag.rows, origImag.cols, origImag.type(), cv::Scalar::all(0));
    Mat mask(origImag.rows, origImag.cols, CV_8UC1, cv::Scalar(0));
    vector< vector<Point> >  co_ordinates;
    co_ordinates.push_back(vector<Point>());
    co_ordinates[0].push_back(top);
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
            if(fabs(slope)<0.5)
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
void myransac(vector<int> x,vector<int> y,Point &pt1, Point &pt2,int imageRows)
{
    //gradient -> using r, theta
    double gradient, distance, yIC;
    double resultn = log(1-0.99)/log(1-(0.5*0.5));//store n
    int r1,r2;
    int dataNum = x.size();
    int threshold = 20;
    int inlier = 0, inMax = 0;
    int x1,y1,x2,y2;
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
            int y1 = imageRows;
            int x1 = (int)((y1-yIC)/gradient);
            int y2 = (imageRows*3)/4;
            int x2 = (int)((y2-yIC)/gradient);
            pt1 = Point(x1, y1);
            pt2 = Point(x2, y2);
        }
        inlier = 0;
    }
}
double getControl(Point &ptl1, Point &ptl2, Point &ptr1, Point &ptr2)
{
    double mid1 = (ptl1.x + ptr1.x)/2.0;
    double mid2 = (ptl2.x + ptr2.x)/2.0;
    double result = atan2((ptl1.y - ptl2.y), (mid2 - mid1)) * 180.0/PI;
    return result;
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
    Mat roiImage, edgeImage, linedImage;
    edgeImage = myCanny(frame);
    roiImage = getROI(edgeImage,Point2f(frame.cols/2,0),Point2f(0,frame.rows),Point2f(frame.cols,frame.rows));
    myHough(roiImage, lines);
    groupLines(lines, left_line_x, left_line_y, right_line_x, right_line_y);
    //RANSAC
    myransac(left_line_x,left_line_y,ptl1, ptl2, frame.rows);
    myransac(right_line_x,right_line_y,ptr1, ptr2, frame.rows);
    linedImage = drawLines(frame, ptl1, ptl2, ptr1, ptr2);
    imshow("detectedImage",linedImage);
    double steeringInfo = getControl(ptl1, ptl2, ptr1, ptr2);
    return (int)steeringInfo;
}
