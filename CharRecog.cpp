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
	//if(CreateTrainData("char/file_info.txt", "char/train.txt", "char/traininfo.txt", 23)) std::cout << "yohoo!";
	bool train = true;

	
	Mat trainingDataMat;
	Mat labelsMat;
	Ptr<ml::SVM> svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::LINEAR);
	svm->setGamma(3);
	if (train){
		string infofile = "char/file_info.txt",
			trainfile = "char/train.txt",	
			traininfo = "char/traininfo.txt";
		//CreateTrainData(infofile, trainfile, traininfo);
		std::ifstream trinfo(traininfo);
		if (!trinfo.is_open()){
			throw "Can not open " + traininfo;
		}
		std::ifstream trainData(trainfile);
		if (!trainData.is_open()){
			throw "Can not open " + trainfile;
		}
		int num_samples = 0, num_features = 0;
		trinfo >> num_samples;
		trinfo >> num_features;
		trinfo.close();
		/*Mat*/ trainingDataMat = cv::Mat::zeros(num_samples, num_features, CV_32FC1);
		/*Mat*/ labelsMat = cv::Mat::zeros(num_samples, 11, CV_32FC1);
		int type = 0;
		for (int i = 0; i < num_samples; ++i){
			for (int j = 0; j < num_features; ++j){
				trainData >> trainingDataMat.at<float>(i, j);
			}
			//trainData >> labelsMat.at<float>(i,0);
			trainData >> type;
			for (int k = 0; k < 11; ++k){
				//labelsMat.at<float>(i, k) = (type==k+1);
				if (type != 13 && type == k + 1)
					labelsMat.at<float>(i, k) = 1;
				else
					labelsMat.at<float>(i, k) = 0;
			}
			//trainData >> labelsMat.at<float>(i,0);
		}
		trainData.close();

		svm->train(trainingDataMat, ml::ROW_SAMPLE, labelsMat);
		// ...
		svm->save("svm.save");
	}
	
	// edit: the params struct got removed,
	// we use setter/getter now:
	

	//Mat trainData; // one row per feature
	//Mat labels;
	svm = Algorithm::load<SVM>("svm.save");
	//svm->loadFromString("svm.save");
	double* features = new double[23];

	float* features1 = new float[23];

	Mat sample = imread("chat/test/1.png");
	getFeatures(sample, features);
	for (int i = 0; i < 23; ++i)
		features1[i] = features[i];
	Mat query=Mat(1,23,CV_32F,features1); // input, 1channel, 1 row (apply reshape(1,1) if nessecary)
	Mat res;   // output
	float temp=svm->predict(query, res);
	

	
	
	std::system("pause");
	return 0;
}