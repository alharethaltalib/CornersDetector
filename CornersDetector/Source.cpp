#include "opencv2/core.hpp"
//#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iomanip>
#include<iostream>

using namespace cv;
using namespace std;

int main() {
	clock_t a = clock();
	Mat frame, Annotated, AnnThresh, H, rectifiedFrame;

	vector<vector<Point>> contours;

	vector<String> pathAnnotated, pathFrames;
	glob("..\\Sample_dataset\\Annotated\\*.png", pathAnnotated);
	glob("..\\Sample_dataset\\Frames\\*.png", pathFrames);
	String pathResults = "..\\Sample_dataset\\Results\\img.";

	for (size_t i = 0; i < pathAnnotated.size(); ++i)
	{
		ostringstream num;
		num << setw(4) << setfill('0') << i;

		Annotated = imread(pathAnnotated[i], IMREAD_GRAYSCALE);
		frame = imread(pathFrames[i]);

		threshold(Annotated, AnnThresh, 200, 255, THRESH_BINARY);
		findContours(AnnThresh, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		for (size_t k = 0; k < contours.size(); k++)
		{
			vector<Point> approx, approx2;
			approxPolyDP(contours[k], approx, arcLength(contours[k], true)*0.02, true);
			if (approx.size() == 4)
			{
				for (size_t j = 0; j < approx.size(); j++)
				{
					circle(frame, approx[j], 5, Scalar(0, 0, 255), CV_FILLED, LINE_AA);
				}
				imwrite(pathResults + num.str() + ".png", frame);
				approx2 = approx;// Previous points
								 //approx2.push_back(Point(914, 204)); approx2.push_back(Point(812, 207)); approx2.push_back(Point(816, 268)); approx2.push_back(Point(915, 263));
			}
			if (approx.size() != 4)
			{
				H = findHomography(approx2, approx, RANSAC);		//,RANSAC,RANSAC 0
																	//cout << H << endl;
																	//perspectiveTransform(approx2, approx, H);
				warpPerspective(AnnThresh, rectifiedFrame, H, frame.size());

				findContours(rectifiedFrame, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
				drawContours(frame, contours, 0, Scalar(0, 0, 255), 2);
				imshow("frame" + to_string(i), frame);
				//imshow("rectifiedFrame" + to_string(i), rectifiedFrame);
				cout << "Press any key to show the next image!" << endl;
				waitKey(0);
			}
		}
		////drawContours(frame, contours, 0, Scalar(0, 0, 255), 1);
		////drawContours(Annotated, contours, 0, Scalar(0, 0, 255), 1);
		//imshow("Annotated" + to_string(i), Annotated);

		////imshow("Annotated " + num.str(), Annotated);
	}

	clock_t z = clock();
	double elapsed_secs = double(z - a) / CLOCKS_PER_SEC;
	std::cout << endl << "THE TOTAL TIME IS : " << elapsed_secs << " Sec" << endl;

	waitKey(0);
	system("pause");
	return 0;
}
