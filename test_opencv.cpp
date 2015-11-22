// test_opencv.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
void thresh_callback();
Mat src;
Mat src_gray;

bool findRect(Mat &src, Mat &out){// обрезает букву под точный размер +1
	int start_x = src.cols - 1, start_y = src.rows - 1, end_x = 0, end_y = 0;
	std::cout << src.channels();
	int addSize = 1;
	bool ok = false;
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j)
		{
			if (src.data[i* src.rows + j] > 128)
			{
				ok = true;
				if (i<start_y)
					start_y = i;
				if (i>end_y)
					end_y = i;
				if (j<start_x)
					start_x = j;
				if (j>end_x)
					end_x = j;
			}
		}
	}
	if (start_x - addSize < 0){
		start_x = addSize;
	}
	if (start_y - addSize < 0){
		start_y = addSize;
	}
	if (end_x + addSize > src.cols - 1){
		end_x = src.cols - addSize;
	}
	if (end_y + addSize > src.rows - 1){
		end_y = src.rows - addSize;
	}
	out = src(Rect(start_x - addSize, start_y - addSize, end_x - start_x + addSize * 2 + 1, end_y - start_y + addSize * 2 + 1));
	return true;
}
int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture capture;
	Mat frame;
	

	// создаЄм окошко
	namedWindow("Hello World", 0);
	
	src = imread("images/0044_ќ_01.png");// считываем картинку 

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(8, 8));
	cvtColor(src, src_gray, CV_BGR2GRAY);
	threshold(src_gray, src_gray, 128, 255, CV_THRESH_BINARY_INV); 

	//--------- обрезаем букву ------------
	if (!findRect(src_gray, src_gray)) 
		return -1;
	resize(src_gray, src_gray, Size(64, 64));
	threshold(src_gray, src_gray, 64, 255, CV_THRESH_BINARY);

	//--------- удал€ем разрывы контура ------------
	dilate(src_gray, src_gray, element);
	erode(src_gray, src_gray, element);

	//----------------------------------------------------------------
	
	imshow("Hello World", src_gray);
	thresh_callback();
	// ждЄм нажати€ клавиши
	waitKey(0);

	destroyWindow("Hello World");
	return 0;
}

int thresh = 10;
int max_thresh = 255;
RNG rng(12345);
void thresh_callback()
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(src_gray.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}