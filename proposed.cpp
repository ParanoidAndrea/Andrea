#include <iostream>  
#include <opencv2\core\core.hpp>  
#include <opencv2\highgui\highgui.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  
#include<cmath>
#include <vector>       // std::vector
using namespace cv;
Mat gammaTransform(Mat& srcImage, float kFactor)
{

	unsigned char LUT[256];
	for (int i = 0; i < 256; i++)
	{
		float f = (i + 0.5f) / 255;
		f = (float)(pow(f, kFactor));
		LUT[i] = saturate_cast<uchar>(f * 255.0f - 0.5f);
	}
	Mat resultImage = srcImage.clone();

	if (srcImage.channels() == 1)
	{

		MatIterator_<uchar> iterator = resultImage.begin<uchar>();
		MatIterator_<uchar> iteratorEnd = resultImage.end<uchar>();
		for (; iterator != iteratorEnd; iterator++)
		{
			*iterator = LUT[(*iterator)];
		}
	}
	else
	{


		MatIterator_<Vec3b> iterator = resultImage.begin<Vec3b>();
		MatIterator_<Vec3b> iteratorEnd = resultImage.end<Vec3b>();
		for (; iterator != iteratorEnd; iterator++)
		{
			(*iterator)[0] = LUT[((*iterator)[0])];//b
			(*iterator)[1] = LUT[((*iterator)[1])];//g
			(*iterator)[2] = LUT[((*iterator)[2])];//r
		}
	}
	return resultImage;
}
int main(int argc, char** argv)
{
	Mat bgr_image = imread("D:\\color enhancement pics\\p4.jpg");
	if (!bgr_image.data)
	{
		printf("could not load image...\n");
		return -1;
	}
	//取两种不同的gamma值
	float gamma1 = 2.2f;
	//float gamma2 = 0.33f;
	float kFactor1 = gamma1;
	float kFactor2 = 1 / gamma1;
	Mat result1 = gammaTransform(bgr_image, kFactor1);
	Mat result2 = gammaTransform(bgr_image, kFactor2);
	Mat result3 = gammaTransform(bgr_image, 0.75);
	//imshow("原图", bgr_image);imshow("gamma=2.2", result1);imshow("gamma=1/2.2", result2);
	imwrite("D:/color enhancement pics/p4_gamma2.jpg", result2);

	bgr_image = result2;
	// READ RGB color image and convert it to Lab
	cv::Mat lab_image, yuv1_image, yuv2_image;
	cv::cvtColor(bgr_image, lab_image, COLOR_BGR2Lab);
	cv::cvtColor(bgr_image, yuv1_image, COLOR_BGR2YUV);
	cv::cvtColor(bgr_image, yuv2_image, COLOR_RGB2YUV);
	// Extract the L channel
	std::vector<cv::Mat> lab_planes(3);
	cv::split(lab_image, lab_planes);
	std::vector<cv::Mat> yuv1_planes(3);
	cv::split(yuv1_image, yuv1_planes);
	std::vector<cv::Mat> yuv2_planes(3);
	cv::split(yuv2_image, yuv2_planes); // now we have the L image in lab_planes[0]

	// apply the CLAHE algorithm to the L channel
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(4);
	cv::Mat dst;
	clahe->apply(lab_planes[0], dst);
	clahe->apply(yuv1_planes[0], dst);
	clahe->apply(yuv2_planes[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(lab_planes[0]);
	cv::merge(lab_planes, lab_image);
	dst.copyTo(yuv1_planes[0]);
	cv::merge(yuv1_planes, yuv1_image);
	dst.copyTo(yuv2_planes[0]);
	cv::merge(yuv2_planes, yuv2_image);

	// convert back to RGB
	cv::Mat image_clahe1, image_clahe2, image_clahe3;
	cv::cvtColor(lab_image, image_clahe1, COLOR_Lab2BGR);
	cv::cvtColor(yuv1_image, image_clahe2, COLOR_YUV2BGR);
	cv::cvtColor(yuv2_image, image_clahe3, COLOR_YUV2RGB);
	// display the results  (you might also want to see lab_planes[0] before and after).
	//cv::imshow("image original", bgr_image);
	//cv::imshow("image CLAHE1", image_clahe1); cv::imshow("image CLAHE2", image_clahe2); cv::imshow("image CLAHE3", image_clahe3);
	imwrite("D:/color enhancement pics/p4_clahe4.jpg", image_clahe1);
	imwrite("D:/color enhancement pics/p4_clahe5.jpg", image_clahe2);
	//imwrite("D:/color enhancement pics/p4_clahe6.jpg", image_clahe3);
	cv::waitKey();
}