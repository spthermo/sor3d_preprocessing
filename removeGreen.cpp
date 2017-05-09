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

//Threshold in RGB colorspace for green background
bool greenVal(uchar B,uchar G,uchar R)
{
	return ((G<215 && G>50));
}

//Threshold in HSV colorspace for green background
bool R2(float H, float S, float V) {
    return (((H>5) && (H < 75) ) && (S>80) && (((V>25) && (V<215))));
}

//Src: RGB image with green background, Srcd: respective depth image
void _removeGreen(Mat src, Mat srcd)
{
	//dilate( src, src, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	//erode(src, src, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	Mat src_temp = src.clone();
	Mat hsv = src.clone();
	dilate( src_temp, src_temp, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	erode(src_temp, src_temp, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	cvtColor(src_temp,hsv,CV_BGR2HSV);
	uchar h, s, v, blue, green, red;
	/*Vec3b hsvVal = hsv.at<Vec3b>(201,287);
    uchar h1,s1,v1;
    h1 = hsvVal.val[0];
    s1 = hsvVal.val[1];
	v1 = hsvVal.val[2];
	cout << (int)h1 << '\t';
	cout << (int)s1 << '\t';
	cout << (int)v1 << std::endl;
	imshow("hsv with green", hsv);
	waitKey(0);*/
	
	for(int i = 0; i < src.rows; i++) 
	{
        for(int j = 0; j < src.cols; j++) 
        {
            Vec3b ColorVal = hsv.at<Vec3b>(i,j);
            h = ColorVal.val[0];
            s = ColorVal.val[1];
            v = ColorVal.val[2];
            
            Vec3b RGBColorVal = src.at<Vec3b>(i,j);
            blue = RGBColorVal.val[0];
            green = RGBColorVal.val[1];
            red = RGBColorVal.val[2];
            
            //Apply HSV and RGB rule for green
			if(R2(h,s,v) && greenVal(blue, green, red))
			{
				//Set green-background pixels to 0 in RGB image
				Vec3b &SetColorVal = src.at<Vec3b>(i,j);
				SetColorVal.val[0] = 0;
				SetColorVal.val[1] = 0;
				SetColorVal.val[2] = 0;
				
				//Set green-background pixels to 0 depth in depth image
				
				srcd.at<float>(i,j) = 0;
			}
		}
	}
}
