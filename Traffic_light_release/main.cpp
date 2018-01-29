#include "recog_static.h"
int main()
{
	Traffic_light SingleFlap;
	SingleFlap.src = imread("");
	SingleFlap.rec_static();
	namedWindow("");
	imshow("", SingleFlap.dst);
	waitKey(0);
	return 0;
}