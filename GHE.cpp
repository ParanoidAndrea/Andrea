#include<iostream>
#include "opencv2/imgcodecs.hpp"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat img = cv::imread("D:\\color enhancement pics\\p4.jpg");
	if (img.empty()) {
		std::cout << "打开图片失败" << std::endl;
		system("pause");
		return -1;
	}
	//imshow("img", img);
	// 直方图均衡化
	Mat matArray;
	cvtColor(img, matArray, COLOR_BGR2YCrCb);
	Mat imgYcbcr[3];
	split(matArray, imgYcbcr);
	equalizeHist(imgYcbcr[0], imgYcbcr[0]);
	merge(imgYcbcr, 3, matArray);
	cvtColor(matArray, img, COLOR_YCrCb2BGR);
	//imshow("imgHist", img);
	imwrite("D:/color enhancement pics/p4_ghe.jpg", img);
	waitKey();
	return 0;
}
