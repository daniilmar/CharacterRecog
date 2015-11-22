#include "stdafx.h"
#include "FindingRect.h"


bool findRect(Mat &src, Mat &out){// обрезает букву под точный размер +1
	int start_x = src.cols - 1, start_y = src.rows - 1, end_x = 0, end_y = 0;
	//std::cout << src.channels();
	int addSize = 1;
	bool ok = false;
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j)
		{
			if (src.data[i* src.rows + j] > 128)
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
