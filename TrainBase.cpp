#include "stdafx.h"
#include "TrainBase.h"
using namespace cv;
using namespace std;


bool CreateTrainData(string infofile, string trainfile, string traininfo, int sizeFeatureVect){
	std::ifstream info(infofile);
	if (!info.is_open()) {
		std::cout << "Can not open " << infofile << std::endl;
		return false;
	}
	std::ofstream data(trainfile);
	if (!data.is_open()){
		std::cout << "Can not open " << trainfile << std::endl;
		return false;
	}
	std::ofstream trinfo(traininfo);
	if (!trinfo.is_open()){
		std::cout << "Can not open " << traininfo << std::endl;
		return false;
	}
	int type = 0, num_files = 0, num_samples = 0;
	char Name = 0;
	Mat sample;
	string folder = "";
	double* features = new double[sizeFeatureVect];
	while (!info.eof()){
		info >> type;
		info >> Name;
		folder = "char/" + string(&Name,1) + "/";
		info >> num_files;
		for (int i = 1; i <= num_files; ++i){
			sample = imread(folder + std::to_string(i) + ".png");
			if (sample.empty()) {
				std::cout << "Can not open " << folder << std::to_string(i) << ".png" << std::endl;
				return false;
			}
			else{
				//------- предобработка и построение вектора признаков -----------
				getFeatures(sample, features);
				for (int j = 0; j < sizeFeatureVect; ++j){
					data << features[j] << " ";
				}
				data << type << "\n";
				//--------------------------------------------------
				++num_samples;
			}
		}
		std::cout << type << " is complited" << std::endl;
	}
	trinfo << num_samples << " " << sizeFeatureVect;
	delete features;
	info.close();
	data.close();
	trinfo.close();
	return true;
}
