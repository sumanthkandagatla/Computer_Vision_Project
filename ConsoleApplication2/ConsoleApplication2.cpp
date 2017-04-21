// ConsoleApplication2.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include < opencv2/video/background_segm.hpp>

#include <math.h>
using namespace cv;
using namespace std;


/// Global variables
Mat src, erosion_dst, dilation_dst;
Mat gray_image;
Mat gau_blur, edge, dil, erode1, contours;
float disparity = 0;

float distance_between_Camera = 200;
float object_Height = 0, object_Width = 0;
float image_Height = 1599;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;
float focal_length = 1830;// 26.73;
RNG rng(12345);
int coods_Left[4][4];
int coods_Right[4][4];
int coods_ThreeDimensional[4][4];
int leftOrRight = 0;
int i = 0, j = 0;//counter for button click
float depth;
/** Function Headers */

void onMouse(int, int, int, int, void*);

void calculate_Distance_Method();

Mat bounding_mat;
Mat bg_image;

int main(int, char** argv)
{
	const char * left_file_name = "C:/Users/Raghunandan/Desktop/LBack.jpg";
	const char * right_file_name = "C:/Users/Raghunandan/Desktop/RBack.jpg";
	Mat src_mat, gray_mat, canny_mat;
	Mat contour_mat;
	const char* src_name = "Source";
	const char* contours_name = "Contours";
	const char* bounding_name = "Bounding";


	//1.Read image file & clone.
	int count = 0;
	do
	{
		if (leftOrRight == 0)
		{
			src_mat = imread(left_file_name);
			
			printf("FOR LEFT IMAGE:\n");
		}
		else if (leftOrRight == 1)
		{
			src_mat = imread(right_file_name);
			
			printf("FOR RIGHT IMAGE:\n");
		}
		//Mat src_output;
		//cv::absdiff(src_mat, bg_image, src_output);// Absolute differences between the 2 images 
		//cv::threshold(src_mat, src_mat, 15, 255, CV_THRESH_BINARY); // set threshold to ignore small differences you can also use inrange function
		//cv::imshow("BG Subtract output", src_mat); // display image

		contour_mat = src_mat.clone();
		bounding_mat = src_mat.clone();

		//2. Convert to gray image and apply canny edge detection
		cvtColor(src_mat, gray_mat, COLOR_RGB2GRAY);
		GaussianBlur(gray_mat, canny_mat, Size(7, 7), 0, 0);
		//Perform Canny Edge Detection
		Canny(canny_mat, canny_mat, 50, 150, 3, false);
		//
		//Dilate and Erode the image for better accuracy
		cv::dilate(canny_mat, canny_mat, cv::Mat(), cv::Point(-1, -1));
		cv::erode(canny_mat, canny_mat, cv::Mat(), cv::Point(-1, -1));

		//3. Find & process the contours
		//3.1 find contours on the edge image.
		vector< vector< cv::Point> > contours;
		findContours(canny_mat, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		//3.2 draw contours & property value on the source image.
		double area, length;
		//drawContours(contour_mat, contours, -1, cv::Scalar(0), 2);  //draw contours on the image
		for (int i = 0; i < contours.size(); ++i)
		{
			Point2f center;
			float radius;

			area = contourArea(contours[i]);
			length = arcLength(contours[i], true);
			minEnclosingCircle(contours[i], center, radius);

			//draw contour property value at the contour center.
			char buffer[64] = { 0 };
			//sprintf(buffer, "Area: %.2lf", area);
			putText(contour_mat, buffer, center, FONT_HERSHEY_SIMPLEX, .6, Scalar(0), 1);

			//sprintf(buffer, "Length: %.2lf", length);
			putText(contour_mat, buffer, Point(center.x, center.y + 20), FONT_HERSHEY_SIMPLEX, .6, Scalar(0), 1);
		}

		//3.3 find bounding of each contour, and draw it on the source image.
		for (int i = 0; i < contours.size(); ++i)
		{
			Point2f points[4];
			Point2f center;
			float radius;
			Rect rect;
			RotatedRect rotate_rect;

			//compute the bounding rect, rotated bounding rect, minum enclosing circle.
			rect = boundingRect(contours[i]);
			rotate_rect = minAreaRect(contours[i]);
			minEnclosingCircle(contours[i], center, radius);

			rotate_rect.points(points);

			vector< vector< Point> > polylines;
			polylines.resize(1);
			for (int j = 0; j < 4; ++j)
				polylines[0].push_back(points[j]);

			//draw them on the bounding image.
			cv::rectangle(bounding_mat, rect, Scalar(0, 0, 255), 2);

			if (rect.height > object_Height)
				object_Height = rect.height;
			if (rect.width > object_Width)
				object_Width = rect.width;
			if (i == (contours.size() - 1))
			{
				char buffer[64] = { 0 };
	//			sprintf(buffer, "Height: %d", object_Height);
				putText(bounding_mat, buffer, center, FONT_HERSHEY_SIMPLEX, .6, Scalar(255, 255, 0), 2);

//				sprintf(buffer, "Width: %d", object_Width);
				putText(bounding_mat, buffer, Point(center.x, center.y + 20), FONT_HERSHEY_SIMPLEX, .6, Scalar(255, 255, 0), 2);
			}
			//cv::polylines(bounding_mat, polylines, true, Scalar(0, 255, 0), 2);
			//cv::circle(bounding_mat, center, radius, Scalar(255, 0, 0), 2);
		}
		printf("Height of rec: %d :: ", object_Height);
		printf("Width of rec: %d", object_Width);
		printf("\n");

		//4. show window
		//namedWindow(src_name, WINDOW_AUTOSIZE);
		//namedWindow(contours_name, WINDOW_AUTOSIZE);
		//namedWindow(bounding_name, WINDOW_AUTOSIZE);

		//imshow(src_name, src_mat);
		//imshow(contours_name, contour_mat);
		if (leftOrRight == 0)
			imwrite("C:/Users/Raghunandan/Desktop/Bounding_LEFT.jpg", bounding_mat);
		else
			imwrite("C:/Users/Raghunandan/Desktop/Bounding_RIGHT.jpg", bounding_mat);

		//imshow(bounding_name, bounding_mat);


		namedWindow("image", CV_WINDOW_FREERATIO);
		//resizeWindow("image", 600, 600);
		imshow("image", bounding_mat);
		setMouseCallback("image", onMouse, 0);
		waitKey(0);

	} while (leftOrRight < 2);
	//TODO:
	// depth = (focal_length*distance_between_Camera) / disp;
	//calculate_Original_Dimensions();
	calculate_Distance_Method();
	Ptr< BackgroundSubtractor> pMOG; //MOG Background subtractor
	int z = 0;
	printf("\nEnter a number to EXIT:");
	cin >> z;
	return 0;
}

void calculate_Distance_Method()
{
	depth = (focal_length*distance_between_Camera) / disparity;
	printf("\nDepth : %lf\n", depth);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			coods_ThreeDimensional[i][j] = (coods_Left[i][j] * depth) / (focal_length);
		}
	}

	float distancex = (coods_ThreeDimensional[1][0] - coods_ThreeDimensional[0][0]) ^ 2;
	float distancey = (coods_ThreeDimensional[1][1] - coods_ThreeDimensional[0][1]) ^ 2;
	float calcdistance = sqrt(distancex + distancey);

	printf("Width : %lf\n", calcdistance);

	float distancex_h = (coods_ThreeDimensional[2][0] - coods_ThreeDimensional[1][0]) ^ 2;
	float distancey_h = (coods_ThreeDimensional[2][1] - coods_ThreeDimensional[1][1]) ^ 2;
	float calcdistance_h = sqrt(distancex_h + distancey_h);
	distancex_h = (coods_ThreeDimensional[3][0] - coods_ThreeDimensional[0][0]) ^ 2;
	distancey_h = (coods_ThreeDimensional[3][1] - coods_ThreeDimensional[0][1]) ^ 2;
	float calcdistance_h2 = sqrt(distancex_h + distancey_h);

	printf("Height : %lf\n", (calcdistance_h + calcdistance_h2) / 2);

}
void onMouse(int event, int x, int y, int flags, void* param)
{
	char text[100];
	Mat img2, img3;
	img2 = bounding_mat.clone();

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		Vec3b p = img2.at<Vec3b>(y, x);
		//sprintf(text, "R=%d, G=%d, B=%d", p[2], p[1], p[0]);
		printf("X=%d,Y=%d\n", x, y);

		if (leftOrRight == 0)
		{
			coods_Left[i][0] = x;
			coods_Left[i][1] = y;
			i++;
		}
		else
		{
			i = 0;
			coods_Right[j][0] = x;
			coods_Right[j][1] = y;
			j++;
		}
		if (i == 4)
		{
			cvDestroyWindow("image");
			//cvReleaseImage(&images[i]);

			leftOrRight++;
		}
		else if (j == 4)
		{
			//CALCULATE DISPARITY BETWEEN IMAGES
			disparity = coods_Left[0][0] - coods_Right[0][0];
			printf("Disparity for Both images is Ax1 : %lf\n", disparity);
			cvDestroyWindow("image");
			leftOrRight++;
		}
	}
	else if (event == CV_EVENT_RBUTTONDOWN)
	{
		cvtColor(bounding_mat, img3, CV_BGR2HSV);
		Vec3b p = img3.at<Vec3b>(y, x);
		//sprintf(text, "H=%d, S=%d, V=%d", p[0], p[1], p[2]);
	}
	else
		//sprintf(text, "x=%d, y=%d", x, y);

	putText(img2, text, Point(5, 15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0));
	//imshow("image", img2);
}