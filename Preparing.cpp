#include "stdafx.h"
#include "Preparing.h"



void getFeatures(Mat& image, double * features){
	Mat image2 = image.clone();
	prepare(image, image);

	prepare2(image2, image2);
	Mat skelet = image2.clone();
	thin_b(skelet);

	imshow("Hello World", skelet);
	//features[0] = countNonZero(skelet);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(image, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);// может лучше skelet
/*	features[1] = contours.size();
	features[2] = contourArea(contours[0]); //изменения здесь повлекут изменения в дальнейшем
	if ((arcLength(contours[0], true)* arcLength(contours[0], true))!=0)
		features[3] = features[2] / (arcLength(contours[0], true)* arcLength(contours[0], true));
	else
		features[3] = 1000;

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
		if (features[2] != 0)
			features[4] = s / features[2];
		else
			features[4] = 1000;
	}
	/*features[5] = moments(image, true).m00;
	features[6] = moments(image, true).m01;
	features[7] = moments(image, true).m02;
	features[8] = moments(image, true).m03;
	features[9] = moments(image, true).m10;
	features[10] = moments(image, true).m11;
	features[11] = moments(image, true).m12;
	features[12] = moments(image, true).m20;
	features[13] = moments(image, true).m21;
	features[14] = moments(image, true).m30;


	features[15] = moments(image, true).mu20;
	features[16] = moments(image, true).mu11;
	features[17] = moments(image, true).mu02;
	features[18] = moments(image, true).mu30;
	features[19] = moments(image, true).mu21;
	features[20] = moments(image, true).mu12;
	features[21] = moments(image, true).mu03;*/
	/*if (features[2] != 0)
		features[5] = image.rows*image.cols / features[2];
	else
		features[5] = 1000;*/
	features[0] = findTriple(skelet);
	/*features[1] = moments(image, true).nu02;
	features[2] = moments(image, true).nu03;
	features[3] = moments(image, true).nu11;
	features[4] = moments(image, true).nu12;
	features[5] = moments(image, true).nu20;
	features[6] = moments(image, true).nu21;
	features[7] = moments(image, true).nu30;*/
	myCount(skelet, &features[1]);
	//features[22] = image.rows*image.cols / features[2];

}

void getFeatures2(Mat& image, double * features){
	prepare(image, image);
	Mat discos;
	image.convertTo(discos, CV_32FC1);
	dct(discos, discos);
	int n = 4;
	Mat discos1(discos, Rect(0, 0, n, n));
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			features[i*n + j] = discos1.at<float>(i, j);
		}
	}
	features[n*n] = moments(image, true).nu02;
	Mat skelet = image.clone();
	thin_b(skelet);
	features[n*n + 1] = findTriple(skelet);
}

void getFeatures3(Mat& image, double * features){
	Mat image2 = image.clone();
	prepare(image, image);

	prepare2(image2, image2);
	Mat skelet = image2.clone();
	thin_b(skelet);

	imshow("Hello World", skelet);
	//features[0] = countNonZero(skelet);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/// Find contours
	findContours(image, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);// может лучше skelet
	
	
	features[0] = findTriple(skelet);
	features[1] = contours.size();
	if (contours.size() == 1)
		features[2] = 0;
	else
	{
		int count = 1;
		double s = 0;
		while (count < contours.size()){
			s += contourArea(contours[count]);
			++count;
		}
		s /= (count - 1);
		features[2] = contourArea(contours[0])/s;
	}

	features[3] = moments(image, true).nu02;
	features[4] = moments(image, true).nu03;
	features[5] = moments(image, true).nu11;
	features[6] = moments(image, true).nu12;
	features[7] = moments(image, true).nu20;
	features[8] = moments(image, true).nu21;
	features[9] = moments(image, true).nu30;
	myCount(skelet, &features[10]);
	//features[22] = image.rows*image.cols / features[2];

}
bool prepare(Mat& src, Mat& out){
	Mat src_gray;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	cvtColor(src, src_gray, CV_BGR2GRAY);
	threshold(src_gray, src_gray, 128, 255, CV_THRESH_BINARY_INV);
	
	//--------- обрезаем букву ------------
	if (!findRect(src_gray, src_gray))
		return -1;
	resize(src_gray, src_gray, Size(SIZE, SIZE));
	threshold(src_gray, src_gray, 64, 255, CV_THRESH_BINARY);

	//--------- удаляем разрывы контура ------------
	dilate(src_gray, src_gray, element);
	erode(src_gray, src_gray, element);
	out = src_gray;
	return true;
}
bool prepare2(Mat& src, Mat& out){
	Mat src_gray;
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	cvtColor(src, src_gray, CV_BGR2GRAY);
	threshold(src_gray, src_gray, 128, 255, CV_THRESH_BINARY_INV);

	//--------- обрезаем букву ------------
	if (!findRect(src_gray, src_gray))
		return -1;
	resize(src_gray, src_gray, Size(SIZE, SIZE));
	threshold(src_gray, src_gray, 64, 255, CV_THRESH_BINARY);

	//--------- удаляем разрывы контура ------------
	dilate(src_gray, src_gray, element);
	erode(src_gray, src_gray, element);
	out = src_gray;
	return true;
}


bool findRect(Mat &src, Mat &out){// обрезает букву под точный размер +1
	int start_x = src.cols - 1, start_y = src.rows - 1, end_x = 0, end_y = 0;
	//std::cout << src.channels();
	int addSize = 1;
	bool ok = false;
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j)
		{
			if (src.data[i* src.cols + j] > 128)
			{
				ok = true;
				if (i<start_y)
					start_y = i;
				if (i>end_y)
					end_y = i;
				if (j<start_x)
					start_x = j;
				if (j>end_x)
					end_x = j;
			}
		}
	}
	if (start_x - addSize < 0){
		start_x = addSize;
	}
	if (start_y - addSize < 0){
		start_y = addSize;
	}
	if (end_x + addSize > src.cols - 1){
		end_x = src.cols - addSize;
	}
	if (end_y + addSize > src.rows - 1){
		end_y = src.rows - addSize;
	}
	out = src(Rect(start_x - addSize, start_y - addSize, end_x - start_x + addSize * 2 + 1, end_y - start_y + addSize * 2 + 1));
	return true;
}


void thin_b(Mat& image)
{
	/*		Thinning algorithm: CACM 1984 march (Zhang and Suen)	*/

	int i, j, n, m, k, cont, br = 0, ar, p1, p2;
	int *a = new int[8];
	Mat res = image.clone();

	cont = 1;

	bool isPlus = true;
	while (cont) {
		cont = 0;
		/*	Sub-iteration 1: */
		for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++) {		/* Scan the entire image */
			if (image.data[i*image.cols + j] == 0) {
				res.data[i*image.cols + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[4];
			p2 = a[2] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.cols + j] = 255;
				cont = 1;
			}
			else res.data[i*image.cols + j] = 0;
		}


		image -= res;


		for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++) {		/* Scan the entire image */
			if (image.data[i*image.cols + j] == 0) {
				res.data[i*image.cols + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[6];
			p2 = a[0] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.cols + j] = 255;
				cont = 1;
			}
			else res.data[i*image.cols + j] = 0;
		}

		image -= res;
	}
	delete[]a;
}


int t1a(Mat &image, int i, int j, int * a, int  &b)

{
	/*	Return the number of 01 patterns in the sequence of pixels
	P2 p3 p4 p5 p6 p7 p8 p9.					*/

	int n, m;

	for (n = 0; n<8; n++) a[n] = 0;
	if (i - 1 >= 0) {
		a[0] = image.data[(i - 1)*image.cols + j];
		if (j + 1 < image.cols) a[1] = image.data[(i - 1)*image.cols + j + 1];
		if (j - 1 >= 0) a[7] = image.data[(i - 1)*image.cols + j - 1];
	}
	if (i + 1 < image.rows) {
		a[4] = image.data[(i + 1)*image.cols + j];
		if (j + 1 < image.cols) a[3] = image.data[(i + 1)*image.cols + j + 1];
		if (j - 1 >= 0) a[5] = image.data[(i + 1)*image.cols + j - 1];
	}
	if (j + 1 < image.cols) a[2] = image.data[(i)*image.cols + j + 1];
	if (j - 1 >= 0) a[6] = image.data[(i)*image.cols + j - 1];

	m = 0;
	b = 0;
	for (n = 0; n<7; n++) {
		if ((a[n] == 0) && (a[n + 1] == 255)) m++;
		b = b + a[n];
	}
	if ((a[7] == 0) && (a[0] == 255)) m++;
	b = b + a[7];
	return m;
}

void myCount(Mat &image,double *fet){

	int counter = 0;
	bool counted = false;
	for (int i = 0; i < image.rows; i++)
	{
		fet[i] = 0;
		for (int j = 0; j < image.cols; j++)
		{
			if (image.data[i*image.cols + j] != 0)
				fet[i] += 1;
		}
	}
	for (int j = 0; j < image.cols; j++)
	{
		fet[j + image.rows] = 0;
		for (int i = 0; i < image.rows; i++)
		{
			if (image.data[i*image.cols + j] != 0)
				fet[j+ image.rows] += 1;
		}
	}
}
int findTriple(Mat &image){
	int sum = 0;
	int count = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image.data[(i)*image.cols + j] != 0)
			{
				if (i - 1 >= 0) {
					sum += image.data[(i - 1)*image.cols + j];
					if (j + 1 < image.cols)
						sum += image.data[(i - 1)*image.cols + j + 1];
					if (j - 1 >= 0)
						sum += image.data[(i - 1)*image.cols + j - 1];
				}
				else{
					if (j + 1 < image.cols)
						sum += image.data[(i)*image.cols + j + 1];
					if (j - 1 >= 0)
						sum += image.data[(i)*image.cols + j - 1];
				}
				if (i + 1 < image.rows) {
					sum += image.data[(i + 1)*image.cols + j];
					if (j + 1 < image.cols)
						sum += image.data[(i + 1)*image.cols + j + 1];
					if (j - 1 >= 0)
						sum += image.data[(i + 1)*image.cols + j - 1];
				}
				else{
					if (j + 1 < image.cols)
						sum += image.data[(i)*image.cols + j + 1];
					if (j - 1 >= 0)
						sum += image.data[(i)*image.cols + j - 1];
				}

				sum /= 255;
				if (sum > 3)
					++count;
			}
			sum = 0;
		}
	}
	return count;
}

int intersection_count(Mat &image, double proportion, bool vertical){
	int line = 0;
	int counter = 0;
	bool counted = false;
	if (vertical) {
		line = image.cols*proportion;
		for (int i = 0; i < image.cols; ++i){
			if (image.data[i*image.cols + line] != 0){
				if (!counted){
					++counter;
					counted = true;
				}
			}
			else
				counted = false;
		}
		return counter;
	}
	else{
		line = image.rows*proportion;
		for (int i = 0; i < image.cols; ++i){
			if (image.data[line*image.rows + i] != 0){
				if (!counted){
					++counter;
					counted = true;
				}
			}
			else
				counted = false;
		}
		return counter;
	}
}


