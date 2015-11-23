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
int t1a(Mat &image, int i, int j, int * a, int  &b);
/** признаки 0) длина; 1)количество контуров; 2) площадь 1 контура;
3) S/P^2; 4) отношение вложенных контуров к главному; 5-14)spatial moments;
15-21) central moments;22) отношения S квадрата / S 1-го контура
*/
void getFeatures(Mat& image, double * features);
void getFeatures2(Mat& image, double * features);
void getFeatures3(Mat& image, double * features);

bool prepare(Mat& src, Mat& out);
bool prepare2(Mat& src, Mat& out);

void myCount(Mat &image,double * fet);

int findTriple(Mat &image);

int intersection_count(Mat &image, double proportion, bool vertical = false);