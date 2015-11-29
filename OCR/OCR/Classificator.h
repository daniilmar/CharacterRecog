#pragma once
#include "Preparing.h"
#include "TrainBase.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/ml.hpp>
#include <opencv2/opencv.hpp>

class Classificator
{
private:
	Mat trainingDataMat;//mustCreateData - требуется ли пересчитать тренировочные данные
	Mat labelsMat;
	int num_features;
	int num_samples;
	double* features;
public:
	Classificator(bool mustCreateData);
	~Classificator();
	int Classify(Mat& src);
};

