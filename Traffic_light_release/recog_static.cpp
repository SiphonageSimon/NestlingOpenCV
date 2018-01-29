#include "recog_static.h"
int Traffic_light::img_process(int color)
{
	const int TH_RED_IN_RED = 100;		//ɸѡ���ʱ�ĺ�ɫ��ֵ
	const int TH_GREEN_IN_RED = 70;		//ɸѡ���ʱ����ɫ��ֵ
	const int TH_RED_IN_GREEN = 27;		//ɸѡ�̵�ʱ�ĺ�ɫ��ֵ
	const int TH_GREEN_IN_GREEN = 110;	//ɸѡ�̵�ʱ����ɫ��ֵ
	const int TH_LIGHT = 150;
	if (imgP.empty())
	{
		_DEBUG_ERROR("IMG IS EMPTY!");
		return -1;
	}
	int width = imgP.cols;
	int height = imgP.rows;
	int B, G, R;
	GaussianBlur(imgP, imgP, Size(5, 5), 5, 5);
	/////////
	for (int y = 0; y<height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			B = imgP.at<Vec3b>(y, x)[0];
			G = imgP.at<Vec3b>(y, x)[1];
			R = imgP.at<Vec3b>(y, x)[2];
			switch (color)
			{
			case GREEN:
				if (G < TH_GREEN_IN_GREEN || R > TH_RED_IN_GREEN)
					imgP.at<Vec3b>(y, x) = (0, 0, 0);
				break;
			case RED:
				if (R < TH_RED_IN_RED || G > TH_GREEN_IN_RED)
					imgP.at<Vec3b>(y, x) = (0, 0, 0);
				break;
			}
		}
	}
	//��ɫ����,�����ϵĵ��Ϊ��ɫ
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(1, 1));
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(3, 3));
	blur(imgP, imgP, Size(5, 5));
	dilate(imgP, imgP, element);
	erode(imgP, imgP, element);
	//��ʴ���ͱ任
	cvtColor(imgP, imgGray, COLOR_RGB2GRAY);
	//��ȡ�Ҷ�ͼ
	threshold(imgGray, imgBinary, TH_LIGHT, 255, THRESH_BINARY);
	//��ȡ��ֵͼ�����ȵ���TH_LIGHT�ĵ㱻��Ϊ��ɫ
	Canny(imgBinary, imgCanny, 0, 255);
	//��ȡ����ͼ
}
//void Traffic_light::refilter(int color){ }




int Traffic_light::rec_static(int color)
{
	if (dst.empty())
	{
		_DEBUG_ERROR("DST IS EMPTY!");
		return -1;
	}
	int width = dst.cols;
	int height = dst.rows;
	dst.copyTo(imgP);
	if (color != NONE)
		fsm_color = color;
	img_process(fsm_color);
	//����ͼ��
	findContours(imgCanny, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	contour_variation.clear();
	for (int i = 0; i < contours.size(); i++)
	{
		contour_rects[i] = boundingRect(Mat(contours[i]));
		if (light_shape(contour_rects[i].width, contour_rects[i].height, width, height))
		{
			contour_variation.push_back(LIGHT);
		}
		else if (digit_shape(contour_rects[i].width, contour_rects[i].height, width, height))
		{
			contour_variation.push_back(DIGIT);
		}
		else
		{
			contour_variation.push_back(NOISE);
		}
	}
	//�Ա�Ե���з���
}
bool Traffic_light::light_shape(double width, double height, int picWidth, int picHeight)
{
	const double SHAPEINDEX = 16.0;		//��С�������
	const double SHAPEINDEX0 = 0.8;	//��߱�����
	const double SHAPEINDEX1 = 1.2;	//��߱�����
	if (width * height < SHAPEINDEX)
		return false;
	else if (width / height >= SHAPEINDEX0 && width / height <= SHAPEINDEX1)
		return true;
	else
		return false;
}

bool Traffic_light::digit_shape(double width, double height, int picWidth, int picHeight)
{
	const double SHAPEINDEX = 15.0;		//��С�������
	const double SHAPEINDEX0 = 0.0;		//��С���ϵ��
	const double SHAPEINDEX1 = 0.95;	//������ϵ��
	const double SHAPEINDEX2 = 0.45;	//�����������
	const double SHAPEINDEX3 = 0.6;		//�����������
	const double SHAPEINDEX4 = 0.1;		//1�ĳ����������
	const double SHAPEINDEX5 = 0.2;		//1�ĳ����������
	double square = width * height;
	double picSquare = (double)picHeight * (double)picWidth;
	if (square < SHAPEINDEX)
		return false;
	else if (square / picSquare > SHAPEINDEX1 || square / picSquare < SHAPEINDEX1)
		return false;
	else if ((width / height < SHAPEINDEX2 || width / height > SHAPEINDEX3) && (width / height < SHAPEINDEX4 || width / height > SHAPEINDEX5))
		return false;
	else
		return true;
}