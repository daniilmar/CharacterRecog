#pragma once
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>

#define SIZE 32

using namespace std;
using namespace cv;

bool findRect(Mat &src, Mat &out);
void thin_b(Mat& image);
void getFeatures2(Mat& image, double * features);
void getFeatures3(Mat& image, double * features);
bool prepare(Mat& src, Mat& out, int cernel_size);
void myCount(Mat &image,double * fet);
int findTriple(Mat &image);
int intersection_count(Mat &image, double proportion, bool vertical = false);
void ourDct(Mat& src, Mat& out);