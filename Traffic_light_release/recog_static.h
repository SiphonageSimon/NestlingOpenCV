#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

enum recog_color { GREEN, BLUE, RED, NONE };
enum shape_valuation {NOISE, LIGHT, DIGIT};
class Traffic_light {
public:
	Mat dst; //����ͼ��
	Mat src; //���ͼ��
	int fsm_color = RED; //��ǰ��ɫ״̬
	int recog_digits; //��ǰʶ�������
	double timer; //��ʱ��
	int rec_static(int color = NONE); //ʶ��ͼ�񣬷���ʶ����̵Ƹ�����������-1��������ɫ��ǿ��ʶ��
private:
	Mat imgP; //img_process���ڴ����ͼ��
	Mat imgGray;//�Ҷ�ͼ��
	Mat imgBinary;//��ֵͼ��
	Mat imgCanny;//Canny��Եͼ��
	vector<Vec4i> hierarchy;
	vector<vector<Rect>> contours;//�����㼯
	vector<Rect> roi;//��Ȥ����
	vector<int> contour_variation;//�������࣬�������ƣ�����
	vector<Rect> contour_rects;//������Ӿ���
	int img_process(int color);//��������ͼ��,����ʧ�ܷ���-1,����imgP,�����ɫ����ͼ,���˺���ĻҶ�ͼ����ֵͼ����Եͼ;
	bool light_shape(double width, double height, int picWidth, int picHeight);
	bool digit_shape(double width, double height, int picWidth, int picHeight);
	//void refilter(int color); //���ι���
};