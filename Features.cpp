#include "stdafx.h"
#include "Features.h"
void getFeatures(Mat& image, double * features){// признаки 0) длина; 1)количество контуров; 2) площадь 1 контура;
	//3) S/P^2; 4) отношение вложенных контуров к главному; 5-14)spatial moments; 15-21) central moments;
	//22) отношения S квадрата / S 1-го контура
	Mat skelet = image.clone();
	thin_b(skelet);

	imshow("Hello World", skelet);
	features[0] = countNonZero(skelet);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(image, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	features[1] = contours.size();
	features[2] = contourArea(contours[0]); //изменения здесь повлекут изменения в дальнейшем
	features[3] = features[2] / (arcLength(contours[0], true)* arcLength(contours[0], true));
	if (contours.size() == 1)
		features[4] = 0;
	else
	{
		int count = 1;
		double s = 0;
		while (count < contours.size()){
			s += contourArea(contours[count]);
			++count;
		}
		s /= count - 1;
		features[4] = s / features[2];
	}
	features[5] = moments(skelet, true).m00;
	features[6] = moments(skelet, true).m01;
	features[7] = moments(skelet, true).m02;
	features[8] = moments(skelet, true).m03;
	features[9] = moments(skelet, true).m10;
	features[10] = moments(skelet, true).m11;
	features[11] = moments(skelet, true).m12;
	features[12] = moments(skelet, true).m20;
	features[13] = moments(skelet, true).m21;
	features[14] = moments(skelet, true).m30;


	features[15] = moments(skelet, true).mu20;
	features[16] = moments(skelet, true).mu11;
	features[17] = moments(skelet, true).mu02;
	features[18] = moments(skelet, true).mu30;
	features[19] = moments(skelet, true).mu21;
	features[20] = moments(skelet, true).mu12;
	features[21] = moments(skelet, true).mu03;
	features[22] = skelet.rows*skelet.cols / features[2];
}


