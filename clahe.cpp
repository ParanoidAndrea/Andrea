#include <iostream>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  
#include<cmath>
#include <vector>     

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	cv::Mat inp_img = cv::imread("D:\\color enhancement pics\\p4.jpg");
	if (!inp_img.data) {
		cout << "Something Wrong";
		return -1;
	}
	namedWindow("Input Image", 1);
	//cv::imshow("Input Image", inp_img);

	cv::Mat clahe_img;
	cv::cvtColor(inp_img, clahe_img, COLOR_BGR2Lab);
	std::vector<cv::Mat> channels(3);
	cv::split(clahe_img, channels);

	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	// 直方图的柱子高度大于计算后的ClipLimit的部分被裁剪掉，然后将其平均分配给整张直方图
	// 从而提升整个图像
	clahe->setClipLimit(4.);	// (int)(4.*(8*8)/256)
	//clahe->setTilesGridSize(Size(8, 8)); // 将图像分为8*8块
	cv::Mat dst;
	clahe->apply(channels[0], dst);
	dst.copyTo(channels[0]);
	cv::merge(channels, clahe_img);

	cv::Mat image_clahe;
	cv::cvtColor(clahe_img, image_clahe, COLOR_Lab2BGR);

	//cout << cvFloor(-1.5) << endl;

	namedWindow("CLAHE Image", 1);
	//cv::imshow("CLAHE Image", image_clahe);
	imwrite("D:/color enhancement pics/p4_clahe.jpg", image_clahe);
	cv::waitKey(0);

	return 0;
}


