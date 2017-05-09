/* Sensorimotor preprocessing code
 * remove green background
 * detect hand/skin color
 * separate hand from object depth map
 * 
 * S.Thermos, PhD Candidate
 * Visual Computing Lab
 * ITI-CERTH
 * 2016 */

#include "Kinect.h"

//Depth map colorization
Mat _colorizeDepth(Mat srcd)
{
	double min = 0;
	double max = 0;
	minMaxIdx(srcd, &min, &max);
	Mat adjMap;
	float scale = 255/(max-min);
	srcd.convertTo(adjMap,CV_8UC1, scale, -min);
	//cout.precision(10);
	//cout << fixed <<(double)max-min << endl;
	Mat falseColorsMap;
	applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_JET);
	return falseColorsMap;
}
