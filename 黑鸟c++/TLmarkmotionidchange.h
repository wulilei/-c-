#pragma once

//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <ctime>
#include <cmath>

//opencv includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/video/video.hpp>

//AG2E includes
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include "jktypes.h"
#include "jkerr.h"
#include "JAKAZuRobot.h"

#pragma comment(lib, "opencv_world3413d.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace cv;


const int CameraID = 0 + cv::CAP_DSHOW;  // the camera ID
const std::string testpicture1 = "D:/1now/my_reconstruction/slip/flow_img/1.jpg";
const std::string testpicture2 = "D:/1now/my_reconstruction/slip/flow_img/2.jpg";

const std::string testvideo = "D:\\1now\\my_reconstruction\\slip\\flow_img\\jingdong14.mp4";
const std::string testvideo_out = "D:\\����\\GelSight\\GelSightCamera\\now.mp4";

const float T_HEIGHTMAP_RESIZERATIO = (float)(0.5);//MIT����
const int T_HEIGHTMAP_RAWIMG_BORDER_SIZE = 16;
const int T_HEIGHTMAP_GRADIMG_BORDER_SIZE = (int)(T_HEIGHTMAP_RESIZERATIO * T_HEIGHTMAP_RAWIMG_BORDER_SIZE);
const int T_HEIGHTMAP_GRADIMG_HEIGHT = 224;  //480*0.5-16*0.5*2
const int T_HEIGHTMAP_GRADIMG_WIDTH = 304;


const int less_connected_pix = 100 * T_HEIGHTMAP_RESIZERATIO * T_HEIGHTMAP_RESIZERATIO;         //��ͨ����С���ظ���

const int MY_MARKFINDCOLS = 640;
const int MY_MARKFINDROWS = 480;
const cv::Size MY_MARKFINDSIZE = cv::Size(MY_MARKFINDCOLS, MY_MARKFINDROWS);

const int ROW_BORDER = 70 * T_HEIGHTMAP_RESIZERATIO - T_HEIGHTMAP_GRADIMG_BORDER_SIZE;  //���¸���ROW_BORDER��������2*ROW_BORDER - �����MIT���ı߽�8
const int COL_BORDER = 80 * T_HEIGHTMAP_RESIZERATIO - T_HEIGHTMAP_GRADIMG_BORDER_SIZE;  //���Ҹ���COL_BORDER��������2*COL_BORDER - �����MIT���ı߽�

const int MAX_DISTANCE_FORONEFRAME = 40 * T_HEIGHTMAP_RESIZERATIO * T_HEIGHTMAP_RESIZERATIO;

const cv::Size T_HEIGHTMAP_RESIZEDIMG_SIZE = cv::Size(320,240);
const cv::Size T_HEIGHTMAP_RESIZEDANDROI_SIZE = cv::Size(304,224);

//hand control
const char init_hand[14] = { 0xFF,0xFE,0xFD,0xFC,0x01,0x08,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0xFB };
const char state0_hand[14] = { 0xFF,0xFE,0xFD,0xFC,0x01,0x06,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0xFB };
const int angle_state = 9;

//leg control
#define PI 3.1415926
const char port_jaka[] = "192.168.1.103";

const JointValue joint_pos_inital = { 223.185   * PI / 180,  //�ؽ�1�Ƕ�
									  118.775   * PI / 180, //�ؽ�2�Ƕ�
									  -125.151  * PI / 180,
									  159.327   * PI / 180,
									  87.754    * PI / 180,
									  -130.779  * PI / 180 };


const JointValue joint_pos_push = {   223.185   * PI / 180,  //�ؽ�1�Ƕ�
									  107.943   * PI / 180, //�ؽ�2�Ƕ�
									  -147.635  * PI / 180,
									  192.641   * PI / 180,
									  87.754    * PI / 180,
									  -130.779  * PI / 180 };


struct MARKWITHID
{
	double location_x;
	double location_y;
	int id;
};

class TLmarkmotionidchange
{
public:

	TLmarkmotionidchange();
	~TLmarkmotionidchange();

	//�ҳ���ǰ֡�����ĵ㲢����
	void find_marks(Mat src);

	void get_id();

	void markmotionidchange(Mat img_src);

	void initofmarkermotioncurrframe(const cv::Mat inCurrColorImg);


	//******************************************���貿��*******************************
	void AG2E_init(int port);
	void AG2E_control(int CurrKey);
	void AG2E_close();

	void jaka_init(const char* port);
	void jaka_move(JointValue a, double speed);
	
private:
	void sauvola(const unsigned char * grayImage, unsigned char * biImage,
		const int w, const int h, const float k, const int windowSize);

	bool border_judgment(cv::Point2d centroids_now);

	void cost_matrices();

	void find_min_index();

public:
	//find_marks
	cv::Mat img_outlook;//������ͼ
	int if_first;   //�ж��ǲ��ǵ�һ֡
	cv::Mat mark_0;//ɸѡ������������ shape��mark_sum��2��
	MARKWITHID markwithid_pre[100];
	MARKWITHID markwithid_next[100];

	cv::Mat m_resizedinCurrColorImg;//initofmarkermotioncurrframe

	//******************************************���貿��*******************************
	SOCKET sockConn;
	SOCKET sockSrv;
	char angle_hand;
	char template_hand[14];

	JAKAZuRobot demo;

private:
	//find_marks
	int numofmark;  //���mark����
	Mat labels, stats, centroids;   //connectedComponentsWithStats

	Mat costmat;
	int pre_mun;

	double m_min[100];  //TLmarkmotionidchange::find_min_index()
	int  m_min_index[100];
	bool yes_lastframecoordinates[100];

};
