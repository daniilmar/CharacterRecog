// test_opencv.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "Preparing.h"

using namespace cv;
using namespace std;
void thresh_callback();
Mat src;
Mat src_gray;
char  character[] = { 'А' ,'Б','В','Е','Н','О','П','Р','Т','У','Х'};

int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture capture;
	Mat frame;
	ofstream file1("1.txt");

	// создаём окошко
	namedWindow("Hello World", 0);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

	double *ass = new double[24];
	string file_name = "char/char/" + string(&character[4], 1) + "/1.png";;

	stringstream file;
	for (int num_char=1; num_char < 91; ++num_char){
		file_name = "char/char/В/" +  to_string(num_char )+ ".png";;
		/*
		file << "char/char/";
		file << character[num_char];
		file << "/1.png";
		file >> file_name;*/
		src = imread(file_name);// считываем картинку 

		
		getFeatures(src, ass);
		for (int i = 0; i < 24; ++i){
			file1 << ass[i] << '\t';
			//std::cout << ass[i] << '\t';
		}
		file1 << std::endl;
		//std::cout << std::endl;
		char fd[245];
		//std::cin >> fd;
	}
	file1.close();
	//thresh_callback();
	// ждём нажатия клавиши
	waitKey(0);
	delete[] ass;
	destroyWindow("Hello World");
	return 0;
}
/*
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
	//std::cout<<contours.size();
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
*/