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

//Threshold in HSV colorspace for skin color
bool R1(float H, float S, float V) {
	bool in_h = false;
	bool in_s = false;
	bool in_v = false;
	bool is_skin = false;
	if(H>8 && H < 58) in_h = true;
	if(S<90) in_s = true;
	if(V>90 && V<220) in_v = true;
	is_skin = in_h && in_s && in_v;
    return is_skin;
}

//Src: RGB input image (for hand), Srco: RGB input image (for the object) 
void _detectSkinColor(Mat src, Mat srco) {

	Mat src_temp = src.clone();

	erode(src_temp, src_temp, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( src_temp, src_temp, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    Mat src_hsv;
    cvtColor(src_temp, src_hsv, CV_BGR2HSV);
    uchar b,g,r;
    bool c = false;
    /*Vec3b hsvVal = src_hsv.at<Vec3b>(128,261);
    uchar h1,s1,v1;
    h1 = hsvVal.val[0];
    s1 = hsvVal.val[1];
	v1 = hsvVal.val[2];
	cout << (int)h1 << '\t';
	cout << (int)s1 << '\t';
	cout << (int)v1 << std::endl;*/
    
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {

            Vec3b pix_hsv = src_hsv.at<Vec3b>(i,j);
            float H = pix_hsv.val[0];
            float S = pix_hsv.val[1];
            float V = pix_hsv.val[2];

            //apply hsv rule
            c = R1(H,S,V);
            if(!c)
            {
				//copy non-hand pixels to object Mat
				Vec3b ColorVal = src.at<Vec3b>(i,j);
				b = ColorVal.val[0];
				g = ColorVal.val[1];
				r = ColorVal.val[2];
				Vec3b &ObjectVal = srco.at<Vec3b>(i,j);
				ObjectVal.val[0] = b;
				ObjectVal.val[1] = g;
				ObjectVal.val[2] = r;
				
				//set non-hand pixels to zero in hand Mat
				Vec3b &SetColorVal = src.at<Vec3b>(i,j);
                SetColorVal.val[0] = 0;
				SetColorVal.val[1] = 0;
				SetColorVal.val[2] = 0;
			}
			else
			{
				//set hand pixels to zero in object Mat
				if(i<205 && j>260){
					Vec3b &ObjectVal = srco.at<Vec3b>(i,j);
					ObjectVal.val[0] = 0;
					ObjectVal.val[1] = 0;
					ObjectVal.val[2] = 0;
				}
				else{
					//copy non-hand pixels to object Mat
					Vec3b ColorVal = src.at<Vec3b>(i,j);
					b = ColorVal.val[0];
					g = ColorVal.val[1];
					r = ColorVal.val[2];
					Vec3b &ObjectVal = srco.at<Vec3b>(i,j);
					ObjectVal.val[0] = b;
					ObjectVal.val[1] = g;
					ObjectVal.val[2] = r;
				
					//set non-hand pixels to zero in hand Mat
					Vec3b &SetColorVal = src.at<Vec3b>(i,j);
      	  	        SetColorVal.val[0] = 0;
					SetColorVal.val[1] = 0;
					SetColorVal.val[2] = 0;
				}
			}
        }
    }
}
