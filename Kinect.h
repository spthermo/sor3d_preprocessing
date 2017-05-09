/* Sensorimotor preprocessing code
 * remove green background
 * detect hand/skin color
 * separate hand from object depth map
 * 
 * S.Thermos, PhD Candidate
 * Visual Computing Lab
 * ITI-CERTH
 * 2016 */

#include<opencv2/highgui/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

using namespace cv;
using namespace std;

bool R1(float H, float S, float V);
bool R2(float H, float S, float V);
bool Rskin(float R, float G, float B);
bool greenVal(uchar B, uchar G, uchar R);
void _detectSkinColor(Mat src, Mat srco);
void _removeGreen(Mat src, Mat srcd);
void _separateHand(Mat src, Mat srcd, Mat srcdo);
Mat _colorizeDepth(Mat srcd);
