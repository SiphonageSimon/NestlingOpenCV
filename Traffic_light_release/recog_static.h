#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

enum recog_color { GREEN, BLUE, RED, NONE };
enum shape_valuation {NOISE, LIGHT, DIGIT};
class Traffic_light {
public:
	Mat dst; //输入图像
	Mat src; //输出图像
	int fsm_color = RED; //当前颜色状态
	int recog_digits; //当前识别的数字
	double timer; //计时器
	int rec_static(int color = NONE); //识别图像，返回识别红绿灯个数，出错返回-1，输入颜色则强制识别；
private:
	Mat imgP; //img_process用于处理的图像
	Mat imgGray;//灰度图像
	Mat imgBinary;//二值图像
	Mat imgCanny;//Canny边缘图像
	vector<Vec4i> hierarchy;
	vector<vector<Rect>> contours;//轮廓点集
	vector<Rect> roi;//兴趣区域
	vector<int> contour_variation;//轮廓分类，噪音，灯，数字
	vector<Rect> contour_rects;//轮廓外接矩形
	int img_process(int color);//初步处理图像,处理失败返回-1,读入imgP,输出颜色过滤图,过滤后处理的灰度图，二值图，边缘图;
	bool light_shape(double width, double height, int picWidth, int picHeight);
	bool digit_shape(double width, double height, int picWidth, int picHeight);
	//void refilter(int color); //二次过滤
};