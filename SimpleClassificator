#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "Preparing.h"

using namespace cv;
using namespace cv::ml;

char  character[] = { 'А' ,'Б','В','Е','Н','О','П','Р','Т','У','Х' };
Mat src;
Mat src_gray;
int thresh = 100;
RNG rng(12345);

int _tmain(int argc, _TCHAR* argv[])
{
	KNearest* knn = KNearest::create();
	
	string infofile = "char/char/file_info.txt",
		trainfile = "char/char/train.txt",
		traininfo = "char/char/traininfo.txt";

	std::ifstream trinfo(traininfo);
	if (!trinfo.is_open()) {
		throw "Can not open " + traininfo;
	}
	std::ifstream trainData(trainfile);
	if (!trainData.is_open()) {
		throw "Can not open " + trainfile;
	}

	int num_samples = 0, num_features = 0;

	trinfo >> num_samples;
	trinfo >> num_features;

	trinfo.close();

	Mat trainingDataMat = cv::Mat::zeros(num_samples, num_features, CV_32FC1);
	Mat labelsMat = cv::Mat::zeros(num_samples, 1, CV_32FC1);

	int type = 0;
	cout << endl;
	for (int i = 0; i < num_samples - 1; ++i) { 
		for (int j = 0; j < num_features; ++j) {
			trainData >> trainingDataMat.at<float>(i, j);
		}

		trainData >> type;
		labelsMat.at<float>(i, 0) = type;

	}
	trainData.close();

	//cv:Ptr<TrainData> trainData = TrainData::create(trainingDataMat, 0, labelsMat);
	knn->setIsClassifier(true);
	knn->train(trainingDataMat, ml::ROW_SAMPLE, labelsMat);
	
	double* features2 = new double[23];

	float* features1 = new float[23];

	for (int i = 1; i < 55; i++) {
		string filename = "char/char/test/" + to_string(i) + ".png";
		cout << filename << endl;
		Mat sample = imread(filename);
		getFeatures(sample, features2);
		for (int i = 0; i < 23; ++i) {
			features1[i] = features2[i];
		}
		Mat query = Mat(1, 23, CV_32F, features1); 
		Mat res;

		double min = 0;
		int k = 0;
		for (int i = 0; i < num_features; i++) {
			min += (features1[i] - trainingDataMat.at<float>(0, i)) * (features1[i] - trainingDataMat.at<float>(0, i));
		}
		min = sqrt(min);

		for (int i = 1; i < num_samples - 1; i++) {
			int tmp = 0;
			for (int j = 0; j < num_features; j++) {
				tmp += ((features1[j] - trainingDataMat.at<float>(i, j)) / 1000.0) * ((features1[j] - trainingDataMat.at<float>(i, j)) / 1000.0);
			}
			tmp = sqrt(tmp);

			if (tmp < min) {
				min = tmp;
				k = i;
			}
		}
		cout << k << " " << labelsMat.at<float>(0, k) << " " << endl;
	}

	waitKey(0);
	system("pause");
	return 0;
}
