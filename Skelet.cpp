#include "stdafx.h"
#include "Skelet.h"


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
			if (image.data[i*image.rows + j] == 0) {
				res.data[i*image.rows + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[4];
			p2 = a[2] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.rows + j] = 255;
				cont = 1;
			}
			else res.data[i*image.rows + j] = 0;
		}


		image -= res;


		for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++) {		/* Scan the entire image */
			if (image.data[i*image.rows + j] == 0) {
				res.data[i*image.rows + j] = 0;
				continue;
			}
			ar = t1a(image, i, j, a, br);	/* Function A */
			p1 = a[0] * a[2] * a[6];
			p2 = a[0] * a[4] * a[6];
			if ((ar == 1) && ((br >= 2 * 255) && (br <= 6 * 255)) &&
				(p1 == 0) && (p2 == 0))  {
				res.data[i*image.rows + j] = 255;
				cont = 1;
			}
			else res.data[i*image.rows + j] = 0;
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
		a[0] = image.data[(i - 1)*image.rows + j];
		if (j + 1 < image.cols) a[1] = image.data[(i - 1)*image.rows + j + 1];
		if (j - 1 >= 0) a[7] = image.data[(i - 1)*image.rows + j - 1];
	}
	if (i + 1 < image.rows) {
		a[4] = image.data[(i + 1)*image.rows + j];
		if (j + 1 < image.cols) a[3] = image.data[(i + 1)*image.rows + j + 1];
		if (j - 1 >= 0) a[5] = image.data[(i + 1)*image.rows + j - 1];
	}
	if (j + 1 < image.cols) a[2] = image.data[(i)*image.rows + j + 1];
	if (j - 1 >= 0) a[6] = image.data[(i)*image.rows + j - 1];

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