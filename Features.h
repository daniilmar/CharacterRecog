#pragma once
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "Skelet.h"
#include <vector>

using namespace std;
using namespace cv;
void getFeatures(Mat& image, double * features);