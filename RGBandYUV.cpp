#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

cv::Mat RGB2YUV(cv::Mat src, bool accelerate = false) {

	CV_Assert(src.channels() == 3);
	cv::Mat dst(src.size(), CV_32FC3);  //�������Ҫ�ø�����,����ɥʧ����
	cv::Vec3b rgb;
	int r = src.rows;
	int c = src.cols;

	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			rgb = src.at<cv::Vec3b>(i, j);
			int B = rgb[0]; int G = rgb[1]; int R = rgb[2];
			if (accelerate == false) {
				dst.at<Vec3f>(i, j)[0] = R * 0.299 + G * 0.587 + B * 0.114; //Y
				dst.at<Vec3f>(i, j)[1] = -0.169 * R - 0.331 * G + 0.500 * B + 128; //U
				dst.at<Vec3f>(i, j)[2] = 0.500 * R - 0.419 * G - 0.081 * B + 128;  //V
			}
			else {
				dst.at<Vec3f>(i, j)[0] = ((R << 6) + (R << 3) + (R << 2) + R + (G << 7) + (G << 4) + (G << 2) + (G << 1) + (B << 4) + (B << 3) + (B << 2) + B) >> 8; //Y
				dst.at<Vec3f>(i, j)[1] = (-((R << 5) + (R << 3) + (R << 1) + R) - ((G << 6) + (G << 4) + (G << 2) + G) + (B << 7) + 32768) >> 8; //U
				dst.at<Vec3f>(i, j)[2] = ((R << 7) - ((G << 6) + (G << 5) + (G << 3) + (G << 3) + G) - ((B << 4) + (B << 2) + B) + 32768) >> 8; //V
			}
		}
	}
	dst.convertTo(dst, CV_8UC3);
	return dst;
}

cv::Mat YUV2RGB(cv::Mat src, bool accelerate = false) {
	CV_Assert(src.channels() == 3);
	cv::Mat dst(src.size(), CV_32FC3); //����һ��Ҫ�ø�����,����ɥʧ����
	cv::Vec3b yuv;
	int r = src.rows;
	int c = src.cols;
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			yuv = src.at<cv::Vec3b>(i, j);
			int Y = yuv[0]; int U = yuv[1]; int V = yuv[2];
			U = U - 128;
			V = V - 128;
			if (accelerate == false) {
				dst.at<Vec3f>(i, j)[0] = Y + 1.770 * U;//B
				dst.at<Vec3f>(i, j)[1] = Y - 0.343 * U - 0.714 * V;//G
				dst.at<Vec3f>(i, j)[2] = Y + 1.403 * V;//R
			}
			else {
				dst.at<Vec3f>(i, j)[0] = Y + U + ((U * 198) >> 8);
				dst.at<Vec3f>(i, j)[1] = Y - ((U * 88) >> 8) - ((V * 183) >> 8);
				dst.at<Vec3f>(i, j)[2] = Y + V + ((V * 103) >> 8);
			}
		}
	}
	dst.convertTo(dst, CV_8UC3);
	return dst;
}


int main() {
	cv::Mat src = cv::imread("Gamma1.jpg");

	if (src.empty()) {
		return -1;
	}
	cv::Mat dst, dst1, dst2;


	double t2 = (double)cv::getTickCount(); //��ʱ��

	cv::cvtColor(src, dst1, COLOR_RGB2YUV); //RGB2YUV

	t2 = (double)cv::getTickCount() - t2;
	double time2 = (t2 * 1000.) / ((double)cv::getTickFrequency());
	std::cout << "Opencv_RGB2YUV=" << time2 << " ms. " << std::endl << std::endl;

	//RGB2YUV//
	double t1 = (double)cv::getTickCount(); //��ʱ��

	dst = RGB2YUV(src, true); //RGB2YUV
	dst2 = YUV2RGB(dst, true); //YUV2BGR

	t1 = (double)cv::getTickCount() - t1;
	double time1 = (t1 * 1000.) / ((double)cv::getTickFrequency());
	std::cout << "My_RGB2YUV=" << time1 << " ms. " << std::endl << std::endl;


	cv::namedWindow("src", 1);
	imshow("src", src);
	cv::namedWindow("My_RGB2YUV", 1);
	imshow("My_RGB2YUV", dst);
	cv::namedWindow("My_YUV2RGB", 1);
	imshow("My_YUV2RGB", dst2);
	cv::namedWindow("Opencv_RGB2YUV", 1);
	imshow("Opencv_RGB2YUV", dst1);
	cv::waitKey(0);
	bool saveFlag = cv::imwrite("D:/color enhancement pics/YUV1.jpg", dst);
	printf("saveFlag=%d\n", saveFlag);
	return 0;

}