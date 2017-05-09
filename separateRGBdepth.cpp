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

//Src: RGB hand only image, Srcd: depth image (for hand), Srcdo: depth image (for object)
void _separateHand(Mat src, Mat srcd, Mat srcdo)
{
	uchar b,g,r;
	for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {

            Vec3b ColorVal = src.at<Vec3b>(i,j);
            b = ColorVal.val[0];
            g = ColorVal.val[1];
            r = ColorVal.val[2];
            if((b == 0) && (g == 0) && (r == 0))
            {
				//Set non-hand pixels to 0 depth in hand depth image
				srcd.at<float>(i,j) = 0;
			}
			else
			{
				//Set hand pixels to 0 depth in object depth image
				srcdo.at<float>(i,j) = 0;
			}
        }
    }
}
