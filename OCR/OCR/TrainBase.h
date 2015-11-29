#ifndef __TRAIN_BASE__
#define __TRAIN_BASE__
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include "Preparing.h"

bool CreateTrainData(string infofile, string trainfile, string traininfo, int size);

#endif