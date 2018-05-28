#ifndef __MY_ZED_H__
#define __MY_ZED_H__
#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>

#ifndef __CV_STD_NAMESPACE__
#define __CV_STD_NAMESPACE__
using namespace cv;
using namespace std;
#endif

cv::Mat slMat2cvMat(sl::Mat& input);
cv::Mat getCVImage(sl::Camera &zed, sl::RuntimeParameters &runtime_parameters, sl::Mat &image_zed);
void zed_init(sl::InitParameters &init_params, sl::Camera &zed, sl::RuntimeParameters &runtime_parameters);
#endif
