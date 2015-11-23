// CharRecog.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "TrainBase.h"

using namespace cv;
using namespace cv::ml;

int _tmain(int argc, _TCHAR* argv[])
{
	bool create_base = false;
	bool train = false;
	bool test = true;

	try{
		int num_samples = 0,
			num_features = 18; //изменить при изменении набора признаков
		Mat trainingDataMat, labelsMat;
		Ptr<KNearest> kN = cv::ml::KNearest::create();
		kN->setAlgorithmType(ml::KNearest::BRUTE_FORCE);

		if (create_base){
			CreateTrainData("char/file_info.txt", "char/train.txt", "char/traininfo.txt", num_features);
		}

		if (train){
			string infofile = "char/file_info.txt",
				trainfile = "char/train.txt",
				traininfo = "char/traininfo.txt";
			std::ifstream trinfo(traininfo);
			if (!trinfo.is_open()){
				throw "Can not open " + traininfo;
			}
			std::ifstream trainData(trainfile);
			if (!trainData.is_open()){
				throw "Can not open " + trainfile;
			}
			trinfo >> num_samples;
			trinfo >> num_features;
			trinfo.close();
			trainingDataMat = cv::Mat::zeros(num_samples, num_features, CV_32FC1);
			labelsMat = cv::Mat::zeros(num_samples, 1, CV_32FC1);
			for (int i = 0; i < num_samples; ++i){
				for (int j = 0; j < num_features; ++j){
					trainData >> trainingDataMat.at<float>(i, j);
				}
				trainData >> labelsMat.at<float>(i, 0);
			}
			trainData.close();
			kN->train(trainingDataMat, ml::ROW_SAMPLE, labelsMat);
			kN->save("kN.save");
		}
		if (test){
			kN = Algorithm::load<KNearest>("kN.save");
			double* features = new double[num_features];
			float* features1 = new float[num_features];
			Mat sample, query, res;
			for (int i = 1; i < 56; ++i){
				sample = imread("char/test/" + std::to_string(i) + ".png");
				getFeatures2(sample, features);
				for (int i = 0; i < num_features; ++i)
					features1[i] = features[i];
				query = Mat(1, num_features, CV_32F, features1);
				std::cout << i << ".png - " << (i - 1) / 5 + 1 << " " << kN->findNearest(query, 10, res) << std::endl;
			}
		}
	}
	catch (string msg){
		std::cout << msg << std::endl;
	}
	std::system("pause");
	return 0;
}