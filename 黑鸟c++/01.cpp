
//#include<opencv2/opencv.hpp>
//#include<iostream>
//
//using namespace cv;
//using namespace std;
//
//int main(int argc, char** argv) {
//	// 打开摄像头
//	// VideoCapture capture(0); 
//
//	// 打开文件
//	VideoCapture capture;
//	capture.open(CameraID);
//	if (!capture.isOpened()) {
//		printf("could not read this video file...\n");
//		return -1;
//	}
//	Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),
//		(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
//	int fps = capture.get(CV_CAP_PROP_FPS);
//	printf("current fps : %d \n", fps);
//	VideoWriter writer(testvideo_out, CV_FOURCC('P', 'I', 'M', '1'), 20, S, true);
//
//	Mat frame;
//	TLmarkmotionidchange m_TLmarkmotionidchange;
//	namedWindow("camera-demo", CV_WINDOW_AUTOSIZE);
//	while (capture.read(frame)) {
//
//		m_TLmarkmotionidchange.markmotionidchange(frame);
//		writer.write(m_TLmarkmotionidchange.img_outlook);
//		char c = waitKey(50);
//		if (c == 27) {
//			break;
//		}
//	}
//	capture.release();
//	writer.release();
//	waitKey(0);
//	return 0;
//}


//
//#include "stdio.h"
//#include "vector"
//
//using namespace std;
//
//void myprint(vector<int>& v)
//{
//	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
//	{
//		cout << *it << " ";
//	}
//	cout << endl;
//}
//
//
//void main()
//{
//	vector<int> a;
//	a.push_back(2111);
//	myprint(a);
//
//	a.clear();
//	cout << a.empty()<<endl;
//	myprint(a);
//}
//

//const char init_hand[14] = { 0xFF,0xFE,0xFD,0xFC,0x01,0x08,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0xFB };
//char template_hand[14] = { 0xFF,0xFE,0xFD,0xFC,0x01,0x06,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0xFB };
//const int angle_state = 9;
//
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//
//#include <WinSock2.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include "iostream"
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/features2d/features2d.hpp"
//#include <opencv2/video/video.hpp>
//
//#pragma comment(lib, "ws2_32.lib")
//using namespace cv;
//
//
//void main()
//{
//	WSADATA wsaData;
//	int port = 8888;
//	char angle_hand = 0;
//
//	Mat dst = imread("D:/桌面/2.jpg");
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		printf("Failed to load Winsock");
//		return;
//	}
//
//	//创建用于监听的套接字
//	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
//
//	SOCKADDR_IN addrSrv;
//	addrSrv.sin_family = AF_INET;
//	addrSrv.sin_port = htons(port); //1024以上的端口号
//	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//
//	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
//	if (retVal == SOCKET_ERROR) {
//		printf("Failed bind:%d\n", WSAGetLastError());
//		return;
//	}
//
//	if (listen(sockSrv, 10) == SOCKET_ERROR) {
//		printf("Listen failed:%d", WSAGetLastError());
//		return;
//	}
//
//	SOCKADDR_IN addrClient;
//	int len = sizeof(SOCKADDR);
//
//	//等待客户请求到来    
//	SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
//	if (sockConn == SOCKET_ERROR) {
//		printf("Accept failed:%d", WSAGetLastError());
//		//break;
//	}
//
//	printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));
//
//	//发送数据
//	int iSend = send(sockConn, init_hand, sizeof(init_hand), 0);
//	if (iSend == SOCKET_ERROR) {
//		printf("send failed");
//		// break;
//	}
//
//	char recvBuf[100];
//	memset(recvBuf, 0, sizeof(recvBuf));
//
//	////         //接收数据
//	//recv(sockConn, recvBuf, sizeof(recvBuf), 0);
//	//printf("%s\n", recvBuf);
//
//	/***************************************夹爪控制指令************************************/
//
//	while (1)
//	{
//		imshow("out",dst);
//		/*cout << "初始化按L 加请按+ 减请按- q退出" << endl;*/
//
//		int CurrKey = waitKey(1);
//		if ((CurrKey == 'L') || (CurrKey == 'l')) {
//			std::cout << "inital"  << std::endl;
//			send(sockConn, init_hand, 14, 0);
//			Sleep(3000);
//
//			angle_hand = 60;
//			template_hand[angle_state] = angle_hand;
//			std::cout << "angle_hand = " << int(angle_hand) << std::endl;
//			send(sockConn, template_hand, 14, 0);
//			Sleep(1000);
//		}
//
//		if (CurrKey == '+')
//		{
//			angle_hand ++;
//			template_hand[angle_state] = angle_hand;
//			std::cout << "angle_hand = " << int(angle_hand) << std::endl;
//			send(sockConn, template_hand, 14, 0);
//			Sleep(30);
//		}
//
//		if (CurrKey == '-')
//		{
//			angle_hand--;
//			template_hand[angle_state] = angle_hand;
//			std::cout << "angle_hand = " << int(angle_hand) << std::endl;
//			send(sockConn, template_hand, 14, 0);
//			Sleep(30);
//		}
//
//		if (CurrKey == 'q')
//		{
//			break;
//		}
//	}
//
//	closesocket(sockConn);
//
//	closesocket(sockSrv);
//	WSACleanup();
//	system("pause");
//}


////爪子+臂
//#include "TLmarkmotionidchange.h"
//
//void main()
//{
//	TLmarkmotionidchange m_TLmarkmotionidchange;
//	Mat dst = imread("D:/桌面/2.jpg");
//
//
//	m_TLmarkmotionidchange.AG2E_init(8888);
//	m_TLmarkmotionidchange.jaka_init(port_jaka);
//
//	while (true)
//	{
//		imshow("out",dst);
////		/*cout << "初始化按L 加请按+ 减请按- q退出" << endl;*/
//
//		int CurrKey = waitKey(1);
//		m_TLmarkmotionidchange.AG2E_control(CurrKey);
//	}
//
//	m_TLmarkmotionidchange.AG2E_close();
//}


////臂
//#include "TLmarkmotionidchange.h"
//void main()
//{
//	TLmarkmotionidchange m_TLmarkmotionidchange;
//	m_TLmarkmotionidchange.jaka_init(port_jaka);
//
//	m_TLmarkmotionidchange.jaka_move(joint_pos_inital,0.5);
//	m_TLmarkmotionidchange.jaka_move(joint_pos_push,0.1);
//}


//
//#include <iostream>
//#include "TLmarkmotionidchange.h"
//
//#define WIN32_LEAN_AND_MEAN
//HANDLE hMutex = NULL;//互斥量
//
//using namespace std;
//
//void aprint() {
//	while (true)
//	{
//		cout << "子线程" << endl;
//	}
//
//}
//
//DWORD WINAPI a(LPVOID lpParamter)
//{
//	WaitForSingleObject(hMutex, INFINITE);
//
//	aprint();
//	Sleep(200);
//
//
//	//释放互斥量锁
//	ReleaseMutex(hMutex);
//	return 0L;
//}
//
//
//
//
//int main() {
//
//	HANDLE E_Thread = CreateThread(NULL, 0, a, NULL, 0, NULL);
//	hMutex = CreateMutex(NULL, FALSE, L"screen");
//	CloseHandle(E_Thread);
//	Sleep(1000);
//
//	while (true)
//	{
//		cout << "主线程1111111111111111" << endl;
//	}
//
//
//	return 0;
//}
//


//#include<iostream>
//#include<thread>
//using namespace std;
//
//void proc()
//{
//	for (int i = 0; i < 6; i++)
//	{
//		cout << i << endl;
//	}
//}
//int main()
//{
//	cout << "我是主线程" << endl;
//	int a = 9;
//	thread th2(proc);//第一个参数为函数名，第二个参数为该函数的第一个参数，如果该函数接收多个参数就依次写在后面。此时线程开始执行。
//	while (true)
//	{
//		cout << "主线程1111111111111111" << endl;
//	}
//	th2.join();//此时主线程被阻塞直至子线程执行结束。
//		return 0;
//}
//
//
//


// //#include <opencv2/opencv.hpp>
// //#include <opencv2/highgui/highgui.hpp>
// //
// //using namespace cv;
// //
// //int main()
// //{
// //	VideoCapture camera(0+CV_CAP_DSHOW);
// //	double width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
// //	double height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
// //	std::cout << "width = " << width << "height = " << height << std::endl;
// //	camera.set(CV_CAP_PROP_FRAME_WIDTH, width);
// //	camera.set(CV_CAP_PROP_FRAME_HEIGHT, height);
// //	camera.set(CV_CAP_PROP_FPS, 20);
// //	Mat img, gray;
// //	while (1)
// //	{
// //		double TempSartTime = (double)cv::getTickCount();
// //		camera >> img;
// //		imshow("USB_Camera_Input", img);
// //
// //		//cvtColor(img, gray, CV_BGR2GRAY);
// //		//imshow("USB_Camera_Input_Gray", gray);
// //
// //		double TempTimeCost1 = ((double)cv::getTickCount() - TempSartTime) / cv::getTickFrequency();
// //		TempTimeCost1 = TempTimeCost1 * 1000.0;
// //		std::cout << "GetCameraCurrFrame Cost = " << TempTimeCost1 << " ms" << std::endl;  //运行时间 先注释
// //
// //		if (waitKey(10) == 27)break;//按Esc退出
// //	}
// //	return 0;
// //}

//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//using namespace cv;
//
//int main()
//{
//	VideoCapture camera;
//	camera.open(0);
//	Mat img, gray;
//
//	double width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
//	double height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
//	camera.set(CV_CAP_PROP_FRAME_WIDTH, width);
//	camera.set(CV_CAP_PROP_FRAME_HEIGHT, height);
//	//camera.set(CAP_PROP_FOURCC, ('M', 'J', 'P', 'G'));
//	//camera.set(CV_CAP_PROP_FRAME_WIDTH, 320);
//	//camera.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
//
//	while (1)
//	{
//		double TempSartTime = (double)cv::getTickCount();
//		camera.read(img);
//		imshow("USB_Camera_Input", img);
//
//
//		double TempTimeCost1 = ((double)cv::getTickCount() - TempSartTime) / cv::getTickFrequency();
//		TempTimeCost1 = TempTimeCost1 * 1000.0;
//		std::cout << "GetCameraCurrFrame Cost = " << TempTimeCost1 << " ms" << std::endl;  //运行时间 先注释
//
//		
//		if (waitKey(1) == 'q')break;//按Esc退出
//	}
//	return 0;
//}


////***********************************************xls测试

//#include <iostream>
//#include "libxl.h"
//#pragma comment(lib,"libxl.lib")
//using namespace libxl;
//
////int main()
////{
////	Book* book = xlCreateBook();
////	if (book)
////	{
////		if (book->load(L"D:\\桌面\\2.xls"))
////		{
////			Sheet* sheet = book->getSheet(0);
////			if (sheet)
////			{
////				double d = sheet->readNum(3, 1);
////				sheet->writeNum(3, 1, d * 2);
////				sheet->writeStr(4, 1, L"new string");
////			}
////			book->save(L"D:\\桌面\\2.xls");
////		}
////
////		book->release();
////	}
////	return 0;
////}
//
//int main()
//{
//	Book* book = xlCreateBook();
//
//	book->load(L"D:\\桌面\\2.xls");
//
//	Sheet* sheet = book->getSheet(0);
//
//	double d = sheet->readNum(3, 1);
//	sheet->writeNum(3, 1, d * 2);
//	sheet->writeStr(4, 1, L"new string");
//
//	book->save(L"D:\\桌面\\2.xls");
//
//	book->release();
//
//	return 0;
//}

////***********************************************
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{
	printf("hello,world!");

}

////***********************************************直方图测试

//#include <iostream>
//#include <time.h>
//#include <opencv2/opencv.hpp>
//
//using namespace std;
//using namespace cv;
//
//cv::Mat DrawHistImg(cv::Mat &hist);
//
//int main()
//{
//	cv::Mat src = 5*imread("D:\\桌面\\11.jpg", 0);
//
//	// 绘制均衡化后直方图
//	cv::Mat hrI = DrawHistImg(src);
//
//	imshow("original", src);
//	imshow("hist", hrI);
//	waitKey(0);
//
//	return 0;
//}
//
//
//// 绘制简易直方图
//cv::Mat DrawHistImg(cv::Mat &src)
//{
//	cv::Mat hist = cv::Mat::zeros(1, 256, CV_32FC1);
//	for (int i = 0; i < src.rows; ++i)
//	{
//		for (int j = 0; j < src.cols; ++j)
//		{
//			hist.at<float>(0, src.at <uchar>(i, j))++;
//		}
//	}
//	cv::Mat histImage = cv::Mat::zeros(540, 1020, CV_8UC1);
//	const int bins = 255;
//	double maxValue;
//	cv::Point2i maxLoc;
//	cv::minMaxLoc(hist, 0, &maxValue, 0, &maxLoc);
//	int scale = 4;
//	int histHeight = 540;
//
//	for (int i = 0; i < bins; i++)
//	{
//		float binValue = (hist.at<float>(i));
//		int height = cvRound(binValue * histHeight / maxValue);
//		cv::rectangle(histImage, cv::Point(i * scale, histHeight),
//			cv::Point((i + 1) * scale - 1, histHeight - height), cv::Scalar(255), -1);
//
//	}
//	return histImage;
//}


