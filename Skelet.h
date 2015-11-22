#pragma once
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;

void thin_b(Mat& image);
int t1a(Mat &image, int i, int j, int * a, int  &b);