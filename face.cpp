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
void detectAndDisplay( Mat frame, string imageName);
void f1Score(const char** argv);
vector<vector<int>> readGroundTruth(string imageName);
double iou(int px1, int py1, int px2, int py2, int rx1, int ry1, int rx2, int ry2);
double f1ScoreCalc(int truePositives, int falsePositives, int falseNegatives);

/** Global variables */
String cascade_name = "frontalface.xml";
CascadeClassifier cascade;

/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	detectAndDisplay( frame,argv[1] );
	//f1Score(argv);

	// 4. Save Result Image
	imwrite( "detected.jpg", frame );
	
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame, string imageName)
{
	std::vector<Rect> faces;
	Mat frame_gray;
        
	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection ///////
	cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );
      
	vector<vector<int>> groundTruthv=readGroundTruth(imageName);
	
	for( int i = 0; i < faces.size(); i++ )
	{
		// 3. Draw box around faces found
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);		
	}

	for( int i = 0; i < groundTruthv.size(); i++ )
	{
		// 4. Draw box around faces found ground truth
		rectangle(frame, Point(groundTruthv[i][0], groundTruthv[i][1]), Point(groundTruthv[i][2],groundTruthv[i][3]), Scalar( 0, 0, 255 ), 2);		
	}

	int truePositives = 0;
	int falsePositives = 0;
	int falseNegatives = 0;
	
	//Value of intersection accepted.
	double value = 0.5;
	int j =0;

	for( int i = 0; i < groundTruthv.size(); i++ )
	{
		for( int k = 0; k < faces.size(); k++ )
		{

			double iouValue= iou(faces[k].x, faces[k].y, (faces[k].x + faces[k].width),(faces[k].y+ faces[k].height),groundTruthv[i][j], groundTruthv[i][j+1], groundTruthv[i][j+2],groundTruthv[i][j+3]);

			if (iouValue >= value) 
			 {				
			 	truePositives += 1;
			 }								
		}	  
	}

	falsePositives = faces.size() - truePositives;
	falseNegatives = groundTruthv.size() - truePositives;

	//Calculate f1ScoreResult
	double f1ScoreResult = f1ScoreCalc(truePositives, falsePositives, falseNegatives);

	//Results
	std::cout << faces.size() << std::endl;
	printf("F1-score: %f \n", f1ScoreResult);

}


vector<vector<int>> readGroundTruth(string imageName) 
{  
 
    string imgName = imageName.substr(0,(imageName.find(".")));

    //Name of the file with the ground truth of the image.
    string nameof =imgName+".csv"; 

    //Vector with all the positions of ground truth.
    vector<vector<int>> fullpositions;
    
    //Open the file with the values of ground truth of the image.
    ifstream ip(nameof);

     string x1;
     string y1;
     string x2;
     string y2;
     
     while (ip.good())
     {

        //Temporal vector to store the 4 point of each ground truth.
     	vector<int> points;
     	
     	getline(ip,x1, ',');
     	getline(ip,y1, ',');
     	getline(ip,x2, ',');
     	getline(ip,y2, '\n');

     	//std::cout << "x1 " << atoi(x1.c_str())<<'\n';

        points.push_back(atoi(x1.c_str()));
        points.push_back(atoi(y1.c_str()));
        points.push_back(atoi(x2.c_str()));
        points.push_back(atoi(y2.c_str()));

        //Save the 4 positions in one line
        fullpositions.push_back(points);
     }

     ip.close();

   /**   for (int i=0; i<fullpositions.size(); ++i)
    {
        for (size_t j=0; j<fullpositions[i].size(); ++j)
        {
            cout << fullpositions[i][j] << "|"; // (separate fields by |)
       }
        cout << "\n";
    }   */

    return fullpositions;
}

double iou(int px1, int py1, int px2, int py2, int rx1, int ry1, int rx2, int ry2)
{

	/**cout << "px1: " << px1 << '\n';
	cout << "py1: " << py1 << '\n';
	cout << "px2: " << px2 << '\n';
	cout << "py2: " << py2 << '\n';

	cout << "rx1: " << rx1 << '\n';
	cout << "ry1: " << ry1 << '\n';
	cout << "rx2: " << rx2 << '\n';
	cout << "ry2: " << ry2 << '\n';*/
  
   int x1 = max(px1,rx1);
   int y1 = max(py1,ry1);
   int x2 = min(px2,rx2);
   int y2 = min(py2,ry2);

   //Intersection between rectangles, if it exists.
   int interseccionRectangles = max(0, x2 -x1 +1) * max (0,y2-y1+1);

   //Areas of the rectangles
   double predictionArea = (px2-px1+1) * (py2-py1+1);
   double groundTruthArea = (rx2-rx1+1) * (ry2-ry1+1);
    
   double areaRectangles = (double) predictionArea + (double) groundTruthArea - (double) interseccionRectangles;

   //Intersection Over Union (IOU)
   double iou = (double)interseccionRectangles/(double)areaRectangles;

   //printf("%f \n", iou);

   return iou;
}


double f1ScoreCalc(int truePositives, int falsePositives, int falseNegatives)
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
 		
}
