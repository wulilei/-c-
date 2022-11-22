#include "TLmarkmotionidchange.h"

TLmarkmotionidchange::TLmarkmotionidchange()
{
	if_first = 1;
}

TLmarkmotionidchange::~TLmarkmotionidchange()
{
	
}

//***********************************
// ��������: sauvola
// ����˵��: �ֲ���ֵ��ֵ��
// ��    ��:
//           const unsigned char * grayImage        [in]        ����ͼ������
//           const unsigned char * biImage          [out]       ���ͼ������     
//           const int w                            [in]        �������ͼ�����ݿ�
//           const int h                            [in]        �������ͼ�����ݸ�
//           const int k                            [in]        threshold = mean*(1 + k*((std / 128) - 1))
//           const int windowSize                   [in]        ����������
// �� �� ֵ: void
//************************************
void TLmarkmotionidchange::sauvola(const unsigned char * grayImage, 
									unsigned char * biImage,
									const int w, const int h, 
									const float k, const int windowSize) {

	int whalf = windowSize >> 1;

	int i, j;

	int IMAGE_WIDTH = w;

	int IMAGE_HEIGHT = h;

	// create the integral image

	unsigned long * integralImg = (unsigned long*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT * sizeof(unsigned long*));

	unsigned long * integralImgSqrt = (unsigned long*)malloc(IMAGE_WIDTH*IMAGE_HEIGHT * sizeof(unsigned long*));

	int sum = 0;

	int sqrtsum = 0;

	int index;

	//�ռ����� integralImg���غͻ���ͼ integralImgSqrt����ƽ���ͻ���ͼ

	for (i = 0; i < IMAGE_HEIGHT; i++) {

		// reset this column sum

		sum = 0;

		sqrtsum = 0;

		for (j = 0; j < IMAGE_WIDTH; j++)

		{

			index = i * IMAGE_WIDTH + j;

			sum += grayImage[index];

			sqrtsum += grayImage[index] * grayImage[index];

			if (i == 0) {

				integralImg[index] = sum;

				integralImgSqrt[index] = sqrtsum;

			}

			else {

				integralImgSqrt[index] = integralImgSqrt[(i - 1)*IMAGE_WIDTH + j] + sqrtsum;

				integralImg[index] = integralImg[(i - 1)*IMAGE_WIDTH + j] + sum;

			}

		}

	}

	//Calculate the mean and standard deviation using the integral image

	int xmin, ymin, xmax, ymax;

	double mean, std, threshold;

	double diagsum, idiagsum, diff, sqdiagsum, sqidiagsum, sqdiff, area;

	for (i = 0; i < IMAGE_WIDTH; i++) {

		for (j = 0; j < IMAGE_HEIGHT; j++) {

			xmin = max(0, i - whalf);

			ymin = max(0, j - whalf);

			xmax = min(IMAGE_WIDTH - 1, i + whalf);

			ymax = min(IMAGE_HEIGHT - 1, j + whalf);

			area = (xmax - xmin + 1) * (ymax - ymin + 1);

			if (area <= 0) {

				biImage[i * IMAGE_WIDTH + j] = 255;

				continue;

			}

			if (xmin == 0 && ymin == 0) {

				diff = integralImg[ymax * IMAGE_WIDTH + xmax];

				sqdiff = integralImgSqrt[ymax * IMAGE_WIDTH + xmax];

			}

			else if (xmin > 0 && ymin == 0) {

				diff = integralImg[ymax * IMAGE_WIDTH + xmax] - integralImg[ymax * IMAGE_WIDTH + xmin - 1];

				sqdiff = integralImgSqrt[ymax * IMAGE_WIDTH + xmax] - integralImgSqrt[ymax * IMAGE_WIDTH + xmin - 1];

			}

			else if (xmin == 0 && ymin > 0) {

				diff = integralImg[ymax * IMAGE_WIDTH + xmax] - integralImg[(ymin - 1) * IMAGE_WIDTH + xmax];

				sqdiff = integralImgSqrt[ymax * IMAGE_WIDTH + xmax] - integralImgSqrt[(ymin - 1) * IMAGE_WIDTH + xmax];;

			}

			else {

				diagsum = integralImg[ymax * IMAGE_WIDTH + xmax] + integralImg[(ymin - 1) * IMAGE_WIDTH + xmin - 1];

				idiagsum = integralImg[(ymin - 1) * IMAGE_WIDTH + xmax] + integralImg[ymax * IMAGE_WIDTH + xmin - 1];

				diff = diagsum - idiagsum;

				sqdiagsum = integralImgSqrt[ymax * IMAGE_WIDTH + xmax] + integralImgSqrt[(ymin - 1) * IMAGE_WIDTH + xmin - 1];

				sqidiagsum = integralImgSqrt[(ymin - 1) * IMAGE_WIDTH + xmax] + integralImgSqrt[ymax * IMAGE_WIDTH + xmin - 1];

				sqdiff = sqdiagsum - sqidiagsum;

			}

			mean = diff / area;

			std = sqrt((sqdiff - diff * diff / area) / (area - 1));

			threshold = mean * (1 + k * ((std / 128) - 1));

			if (grayImage[j*IMAGE_WIDTH + i] < threshold)

				biImage[j*IMAGE_WIDTH + i] = 0;

			else

				biImage[j*IMAGE_WIDTH + i] = 255;

		}

	}

	free(integralImg);

	free(integralImgSqrt);
}

bool TLmarkmotionidchange::border_judgment(cv::Point2d centroids_now)
{
	if (centroids_now.x >= COL_BORDER && centroids_now.x <= T_HEIGHTMAP_RESIZERATIO * MY_MARKFINDCOLS - COL_BORDER
	  && centroids_now.y >= ROW_BORDER && centroids_now.y <= T_HEIGHTMAP_RESIZERATIO * MY_MARKFINDROWS - ROW_BORDER)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void TLmarkmotionidchange::find_marks(cv::Mat img_src)//�ҳ���ǰ֡�����ĵ㲢����
{
	CV_Assert(img_src.type() == CV_8UC3 && img_src.size() == T_HEIGHTMAP_RESIZEDANDROI_SIZE);

	Mat img;
	img_outlook = img_src.clone();
	cvtColor(img_src, img, COLOR_BGR2GRAY);

	Mat out = img.clone();
	unsigned char *Img = img.data;
	unsigned char *Out = out.data;
	sauvola(Img, Out, T_HEIGHTMAP_GRADIMG_WIDTH, T_HEIGHTMAP_GRADIMG_HEIGHT, 0.35, 12);

	Mat yuzhi = Mat(T_HEIGHTMAP_GRADIMG_HEIGHT, T_HEIGHTMAP_GRADIMG_WIDTH, CV_8UC1, Out, 0);  //unsigned char * -> Mat
	/*	imshow("1", yuzhi);*/

	Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kerne2 = cv::getStructuringElement(MORPH_RECT, Size(2, 2));
	morphologyEx(yuzhi, yuzhi, MORPH_OPEN, kernel);
	//imshow("2", yuzhi);
	morphologyEx(yuzhi, yuzhi, MORPH_CLOSE, kerne2);
	//imshow("3", yuzhi);
	bitwise_not(yuzhi, yuzhi);
	//imshow("34", yuzhi);

	numofmark = cv::connectedComponentsWithStats(yuzhi, labels, stats, centroids);
	//Mat mark_0 = marka.getMat();

	mark_0 = Mat::zeros(0, 1, CV_64FC2);;
	//cout << mark_0.cols << endl;   //test
	if (if_first)  //��һ֡ɸһ��
	{
		for (int i = 1; i < numofmark; i++)
		{
			if ((stats.at<int>(i, cv::CC_STAT_AREA) > less_connected_pix) && (border_judgment(centroids.at<cv::Point2d>(i))))
			{
				/*cout << "centroids" << centroids.at <cv::Point2d>(i)<<endl;*/
				cv::circle(img_outlook, centroids.at<cv::Point2d>(i), 3, cv::Scalar(255, 255, 255), -1);
				//����Ϊ��at<cv::Point2d>���ð�mat.inl.hpp 951��ע����
				mark_0.push_back(centroids.at<cv::Point2d>(i));
			}
		}
	}
	else
	{
		for (int i = 1; i < numofmark; i++)
		{
			if (stats.at<int>(i, cv::CC_STAT_AREA) > less_connected_pix)
			{
				/*cout << "centroids" << centroids.at <cv::Point2d>(i)<<endl;*/
				cv::circle(img_outlook, centroids.at<cv::Point2d>(i), 3, cv::Scalar(255, 255, 255), -1);
				//����Ϊ��at<cv::Point2d>���ð�mat.inl.hpp 951��ע����
				mark_0.push_back(centroids.at<cv::Point2d>(i));
			}
		}
	}


	//imshow("output", img_outlook);  //������
	//cout <<" mark0 =  "<< mark_0.at<cv::Point2d>(0)  << endl;   //test
	//cout << mark_0.at<cv::Point2d>(40).x << endl;   //��������������
}

void TLmarkmotionidchange::get_id()
{
	if (if_first)  //��һ֡
	{
		for (int i = 0; i < mark_0.rows; i++)
		{
			markwithid_pre[i].id = i;
			markwithid_pre[i].location_x = mark_0.at<cv::Point2d>(i).x;
			markwithid_pre[i].location_y = mark_0.at<cv::Point2d>(i).y;
			putText(img_outlook, std::to_string(markwithid_pre[i].id),mark_0.at<cv::Point2d>(i), cv::FONT_HERSHEY_SCRIPT_SIMPLEX,0.5,cv::Scalar(0,255,255));
		}
		pre_mun = mark_0.rows;   //��һ֡�Ƕ��ٸ��� ����Ͳ����ˣ�ֻ�ٲ���
		imshow("output", img_outlook);
	}
	else
	{
		cost_matrices();                //��ȡ�������
		find_min_index();               //�Ӵ��۾������ҳ�ÿһ����С��ֵ���±�
		for (int u = 0; u < pre_mun; u++)
		{
			if (yes_lastframecoordinates[u]) //˵��û�ҵ�ƥ��㣬ʹ����һ֡�ĵ�
			{
				markwithid_next[u].id = u;
				markwithid_next[u].location_x = markwithid_pre[u].location_x;
				markwithid_next[u].location_y = markwithid_pre[u].location_y;
				cv::circle(img_outlook, Point2d(markwithid_pre[u].location_x, markwithid_pre[u].location_y), 3, cv::Scalar(255, 255, 255), -1);//��ԲҲ����
				putText(img_outlook, std::to_string(markwithid_pre[u].id),Point2d(markwithid_pre[u].location_x, markwithid_pre[u].location_y), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(0, 255, 255));
			}
			else
			{
				markwithid_next[u].id = u;
				markwithid_next[u].location_x = mark_0.at<cv::Point2d>(m_min_index[u]).x;//min_index[u]
				markwithid_next[u].location_y = mark_0.at<cv::Point2d>(m_min_index[u]).y;
				putText(img_outlook, std::to_string(markwithid_pre[u].id), mark_0.at<cv::Point2d>(m_min_index[u]), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(0, 255, 255));
			}

		}
		for (int j = 0 ; j < 100; j++)
		{
			markwithid_pre[j] = markwithid_next[j];    //�滻
		}
		imshow("output", img_outlook);
	}

}

void TLmarkmotionidchange::cost_matrices()   //��ȡ���۾���
{
	costmat = Mat::zeros(mark_0.rows, pre_mun, CV_64FC1);
	for (int u = 0; u < pre_mun; u++)  //markwithid_pre
	{
		for (int v = 0; v < mark_0.rows; v++) //mark_0
		{
			costmat.at<double>(v, u) = sqrt(
											pow(markwithid_pre[u].location_x - mark_0.at<cv::Point2d>(v).x, 2)
											+ pow(markwithid_pre[u].location_y - mark_0.at<cv::Point2d>(v).y, 2)
											);//����
			/*cout << costmat.at<double>(v, u)<<" ";*/
		}
		/*cout << endl;*/
	}
	
}

void TLmarkmotionidchange::find_min_index()  //�Ӵ��۾������ҳ�ÿһ����С��ֵ���±�
{
	m_min[100] = {0};
	m_min_index[100] = { 0 };
	yes_lastframecoordinates[100] = { false };
	for (int j = 0; j < costmat.cols; j++)
	{
		m_min[j] = 1000;    
		for (int i = 0; i < costmat.rows; i++)
		{
			if (costmat.at<double>(i, j) < m_min[j] && costmat.at<double>(i, j) < MAX_DISTANCE_FORONEFRAME)//&& costmat.at<double>(i, j) <
			{
				m_min[j] = costmat.at<double>(i, j);
				m_min_index[j] = i;
			}
		}
		if (m_min[j] == 1000)  //˵��û�ҵ�ƥ��㣬ʹ����һ֡�ĵ�
		{
			yes_lastframecoordinates[j] = true;
		}
		
	}
	//cout << m_min_index[2]  << " = " << m_min[2] <<" ; "<< m_min_index[3]  << " = " << m_min[3] << endl;
}

void TLmarkmotionidchange::initofmarkermotioncurrframe(const cv::Mat img_src)
{
	Mat img_resizedandroi;
	cv::resize(img_src, img_resizedandroi, T_HEIGHTMAP_RESIZEDIMG_SIZE, 0, 0, INTER_CUBIC);//��С *0.5

	Rect ROI(T_HEIGHTMAP_GRADIMG_BORDER_SIZE, T_HEIGHTMAP_GRADIMG_BORDER_SIZE, T_HEIGHTMAP_GRADIMG_WIDTH, T_HEIGHTMAP_GRADIMG_HEIGHT);

	m_resizedinCurrColorImg = img_resizedandroi(ROI);   //��ǰ֡�ߴ�ĺ��ԭʼͼ��  �ĳߴ������
	/*std::cout << "size of m_resizedinCurrColorImg" << m_resizedinCurrColorImg.size() << std::endl;*/
}

void TLmarkmotionidchange::markmotionidchange(Mat img_src)
{
	CV_Assert((img_src.type() == CV_8UC3) && (img_src.size() == MY_MARKFINDSIZE));

	initofmarkermotioncurrframe(img_src);
	find_marks(m_resizedinCurrColorImg);

	if (if_first)  //��һ֡
	{
		get_id();
		if_first = 0;
	}
	else
	{
		get_id();  //����
	}
}










//******************************************���貿��*******************************
void TLmarkmotionidchange::AG2E_init(int port)
{
	WSADATA wsaData;
	angle_hand = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock");
		return;
	}

	//�������ڼ������׽���
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024���ϵĶ˿ں�
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR) {
		printf("Failed bind:%d\n", WSAGetLastError());
		return;
	}

	if (listen(sockSrv, 10) == SOCKET_ERROR) {
		printf("Listen failed:%d", WSAGetLastError());
		return;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	//�ȴ��ͻ�������    
	TLmarkmotionidchange::sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
	if (TLmarkmotionidchange::sockConn == SOCKET_ERROR) {
		printf("Accept failed:%d", WSAGetLastError());
		//break;
	}

	printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

	//��������
	int iSend = send(sockConn, init_hand, sizeof(init_hand), 0);
	if (iSend == SOCKET_ERROR) {
		printf("send failed");
		// break;
	}
	for (int i = 0; i < 14; i++)
	{
		template_hand[i] = state0_hand[i];
	}

	char recvBuf[100];
	memset(recvBuf, 0, sizeof(recvBuf));
}

void TLmarkmotionidchange::AG2E_control(int CurrKey)
{

	if ((CurrKey == 'L') || (CurrKey == 'l')) {
		std::cout << "inital" << std::endl;
		send(sockConn, init_hand, 14, 0);
		Sleep(3000);

		angle_hand = 60;
		template_hand[angle_state] = angle_hand;
		std::cout << "angle_hand = " << int(angle_hand) << std::endl;
		send(sockConn, template_hand, 14, 0);
		Sleep(1000);
	}

	if (CurrKey == '+')
	{
		angle_hand++;
		template_hand[angle_state] = angle_hand;
		std::cout << "angle_hand = " << int(angle_hand) << std::endl;
		send(sockConn, template_hand, 14, 0);
		Sleep(30);
	}

	if (CurrKey == '-')
	{
		angle_hand--;
		template_hand[angle_state] = angle_hand;
		std::cout << "angle_hand = " << int(angle_hand) << std::endl;
		send(sockConn, template_hand, 14, 0);
		Sleep(30);
	}

	if (CurrKey == '*')
	{
		jaka_move(joint_pos_inital, 0.5);
	}
	if (CurrKey == '/')
	{
		jaka_move(joint_pos_push, 0.1);
	}
}

void TLmarkmotionidchange::AG2E_close()
{
	closesocket(sockConn);

	closesocket(sockSrv);
	WSACleanup();
}

void TLmarkmotionidchange::jaka_init(const char* port)
{
	// ���ӻ�����
	printf("start\n");
	//ʵ��API����demo

	//��½����������Ҫ��192.168.1.100�滻Ϊ�Լ���������IP
	//demo.login_in("192.168.1.100");
	demo.login_in(port);
	printf("���ӳɹ�\n");
	//�������ϵ�
	//demo.power_on();

	//printf("�������ϵ�\n");
	////��������ʹ��

	//demo.enable_robot();

	printf("��������ʹ��\n");
}

void TLmarkmotionidchange::jaka_move(JointValue a, double speed)
{
	demo.joint_move(&a, ABS, TRUE, speed);
	printf("�������˶�����\n");
	Sleep(6000);
}
