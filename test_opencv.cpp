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
void thin_b(Mat &image);
int t1a(Mat& image, int i, int j, int * a, int  &b);
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
	
	src = imread("images/0046_‘_02.png");// считываем картинку 

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
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

	//------------------—келетезаци€----------------------------------------------

	thin_b(src_gray);
	//-----------------------------------------------------
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
	findContours(src_gray, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	//findContours(src_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(src_gray.size(), CV_8UC3);
	std::cout<<contours.size();
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8);// , hierarchy, 0, Point());
	}

	/// Show in a window

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	resize(drawing, drawing, Size(128, 128));
	imshow("Contours", drawing);
}

void thin_b(Mat& image)
{
	/*		Thinning algorithm: CACM 1984 march (Zhang and Suen)	*/

	int i, j, n, m, k, cont, br=0, ar, p1, p2;
	int *a=new int[8];
	Mat res = image.clone();
	
	cont = 1;

	bool isPlus = true;
	while (cont) {
		cont = 0;
		/*	Sub-iteration 1: */
		for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++) {		/* Scan the entire image */
			if (image.data[i*image.rows + j] == 0) {
				res.data[i*image.rows + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[4];
			p2 = a[2] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.rows + j] = 255;
				cont = 1;
			}
			else res.data[i*image.rows + j] = 0;
		}

		
		image -= res;


		for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++) {		/* Scan the entire image */
			if (image.data[i*image.rows + j] == 0) {
				res.data[i*image.rows + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[6];
			p2 = a[0] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.rows + j] = 255;
				cont = 1;
			}
			else res.data[i*image.rows + j] = 0;
		}
		
		image -= res;
	}
	delete[]a;
}


int t1a(Mat &image, int i, int j, int * a, int  &b)

{
	/*	Return the number of 01 patterns in the sequence of pixels
	P2 p3 p4 p5 p6 p7 p8 p9.					*/

	int n, m;

	for (n = 0; n<8; n++) a[n] = 0;
	if (i - 1 >= 0) {
		a[0] = image.data[(i-1)*image.rows + j];
		if (j + 1 < image.cols) a[1] = image.data[(i - 1)*image.rows + j+1];
		if (j - 1 >= 0) a[7] = image.data[(i - 1)*image.rows + j-1];
	}
	if (i + 1 < image.rows) {
		a[4] = image.data[(i + 1)*image.rows + j];
		if (j + 1 < image.cols) a[3] = image.data[(i + 1)*image.rows + j + 1];
		if (j - 1 >= 0) a[5] = image.data[(i + 1)*image.rows + j-1];
	}
	if (j + 1 < image.cols) a[2] = image.data[(i)*image.rows + j + 1];
	if (j - 1 >= 0) a[6] = image.data[(i )*image.rows + j-1];

	m = 0;	
	b = 0;
	for (n = 0; n<7; n++) {
		if ((a[n] == 0) && (a[n + 1] == 255)) m++;
	b = b + a[n];
	}
	if ((a[7] == 0) && (a[0] == 255)) m++;
	b = b + a[7];
	return m;
}
