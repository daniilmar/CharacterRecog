#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "Preparing.h"
#include "TrainBase.h"

using namespace cv;
using namespace cv::ml;

int _tmain(int argc, _TCHAR* argv[])
{
	string infofile = "char/char/file_info.txt",
		trainfile = "char/char/train.txt",
		traininfo = "char/char/traininfo.txt";

	CreateTrainData(infofile, trainfile, traininfo, 18);
	
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
	
	double* features = new double[20];
	int right = 0;

	for (int i = 1; i < 56; i++) {

		string filename = "char/char/test/" + to_string(i) + ".png";

		Mat sample = imread(filename);
		getFeatures2(sample, features);

		Mat query = Mat(1, 23, CV_32F, features);
		Mat res;

		double min = 0;
		int k = 0;
		for (int i = 0; i < num_features; i++) {
			min += (features[i] - trainingDataMat.at<float>(0, i)) * (features[i] - trainingDataMat.at<float>(0, i));
		}
		min = sqrt(min);

		for (int i = 1; i < num_samples - 1; i++) {
			int tmp = 0;
			for (int j = 0; j < num_features; j++) {
				tmp += (features[j] - trainingDataMat.at<float>(i, j)) * (features[j] - trainingDataMat.at<float>(i, j));
			}
			tmp = sqrt(tmp);

			if (tmp < min) {
				min = tmp;
				k = i;
			}
		}

		if (labelsMat.at<float>(0, k) == ((i - 1) / 5 + 1)) {
			right += 1;
		}

		std::cout << i << ".png - " << (i - 1) / 5 + 1 << " " << labelsMat.at<float>(0, k) << std::endl;
	}
	
	cout << (right/56.0) * 100 << "%" << endl;
	system("pause");
	return 0;
}
