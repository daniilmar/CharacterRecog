#include "Classificator.h"
using namespace cv;
//using namespace cv::ml;
Classificator::Classificator(bool mustCreateData)
{
	string infofile = "char/file_info.txt",
		trainfile = "char/train.txt",
		traininfo = "char/traininfo.txt";

	num_samples = 0;
	num_features = 18;

	if (mustCreateData)
		CreateTrainData(infofile, trainfile, traininfo, num_features);

	std::ifstream trinfo(traininfo);
	if (!trinfo.is_open()) {
		throw "Can not open " + traininfo;
	}

	std::ifstream trainData(trainfile);
	if (!trainData.is_open()) {
		throw "Can not open " + trainfile;
	}

	trinfo >> num_samples;
	trinfo >> num_features;

	features = new double[num_features];

	trinfo.close();

	trainingDataMat = cv::Mat::zeros(num_samples, num_features, CV_32FC1);
	labelsMat = cv::Mat::zeros(num_samples, 1, CV_32FC1);

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
}

Classificator::~Classificator()
{
	delete features;
}

int Classificator::Classify(Mat& src){
	int right = 0;
	
	getFeatures2(src, features);

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
	return labelsMat.at<float>(k, 0);
}