/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

// header inclusion
// header inclusion
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
void f1Score(const char** argv);
//double f1ScoreCalc(int truePositives, int falsePositives, int falseNegatives);

/** Global variables */
String cascade_name = "frontalface.xml";
CascadeClassifier cascade;

class groundTruth{

	int x1, y1, x2, y2;
 
     public: 
        groundTruth(int x1, int y1, int x2, int y2)
        {
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;    
        }
};   


/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	detectAndDisplay( frame );
	//f1Score(argv);

	// 4. Save Result Image
	imwrite( "detected.jpg", frame );

	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
	std::vector<Rect> faces;
	Mat frame_gray;

    // std::vector<groundTruth> realFaces;
    std::vector<int> realfacesx;
    std::vector<int> realfacesy;
    realfacesx.push_back(65);
    realfacesx.push_back(118);
    realfacesy.push_back(140);
    realfacesy.push_back(94);

    //realFaces.push_back(groundTruth(60+5,135+5,123-5,99-5));
    //std::cout << realFaces << std::endl;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection 
	cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

       // 3. Print number of Faces found
	std::cout << faces.size() << std::endl;

       // 4. Draw box around faces found
	for( int i = 0; i < faces.size(); i++ )
	{
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
		printf("%d %d\n", faces[i].x, faces[i].y);
		printf("%d %d\n", faces[i].x + faces[i].width, faces[i].y + faces[i].height);
	}

    rectangle(frame, Point(realfacesx[0], realfacesy[0]), Point(realfacesx[1], realfacesy[1]), Scalar( 0, 0, 255 ), 2);
	//rectangle(frame, Point(realFaces[0].x1, realFaces[0].y1), Point(realFaces[0].x2, realFaces[0].y2), Scalar( 0, 0, 255 ), 2);
		//printf("%d %d\n", faces[i].x, faces[i].y);

}

/**{

	int truePositives, falsePositives, falseNegatives;
	string imageName = argv[1];

	if (imageName == "dart4.jpg")
	{
		truePositives = 1;
		falsePositives = 0;
		falseNegatives = 0;
	}
	else if (imageName == "dart5.jpg")
	{
		truePositives = 11;
		falsePositives = 3;
		falseNegatives = 0;
	}
	else if (imageName == "dart13.jpg")
	{
		truePositives = 1;
		falsePositives = 1;
		falseNegatives = 0;
	}
	else if (imageName == "dart14.jpg")
	{
		truePositives = 2;
		falsePositives = 4;
		falseNegatives = 0;
	}
	else if (imageName == "dart15.jpg")
	{
		truePositives = 2;
		falsePositives = 2;
		falseNegatives = 1;
	}


	double f1ScoreResult = f1ScoreCalc (truePositives, falsePositives, falseNegatives);


    printf("F1-score: %.2lf\n", f1ScoreResult);
   
}*/
/**double f1ScoreCalc(int truePositives, int falsePositives, int falseNegatives)
{
	double precision, recall, denominator, f1ScoreValue;	

	precision = (double) truePositives/ ((double) truePositives + (double) falsePositives);
    recall = (double) truePositives / ((double) truePositives + (double) falseNegatives);

    denominator = (precision + recall);

		if (denominator != 0) 			
		{ 
			f1ScoreValue = 2 * ((precision * recall)/ denominator);
		}

		else
		{
			f1ScoreValue = 0;
		}

    return f1ScoreValue;        
 		
}*/
