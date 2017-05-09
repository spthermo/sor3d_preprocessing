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
#include "skinColor.cpp"
#include "removeGreen.cpp"
#include "separateRGBdepth.cpp"
#include "colorizeDepth.cpp"

int main(int argc, char* argv[])
{
	std::cout<<"OpenCV version: "CV_VERSION<<std::endl;
	char* path;
	char* path2;
	if (argc < 3){
		std::cout << "Insert PathIn and PathOut as arguments.. " << std::endl;
		exit(0);
	} 
	path = argv[1];
	path2 = argv[2];
	ifstream rgb,depth;

	Mat img, imgd, DepthMap, img_skin, imgd_object, fullRGBimage, imgd_crop, imgd_object_crop, origDepth_crop, origObjDepth_crop, origHandDepth_crop;
	Mat DepthMap_crop, img_skin_crop, img_skin_object_crop, fullRGBimage_crop, fullDepth_c, object_c, hand_c;
	Rect roi(106,84,300,300);

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path)) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {

	  	if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ){
				
			std::cout << ent->d_name<< std::endl;
			char pathIn[100];
			sprintf (pathIn, "%s/%s",path ,ent->d_name);
			char pathOut[100];
			sprintf (pathOut, "%s/%s",path2 ,ent->d_name);
			int result = mkdir(pathOut,0777);
			DIR *dirIn;
			struct dirent *entIN;
			if ((dirIn = opendir (pathIn)) != NULL) {
				while ((entIN = readdir (dirIn)) != NULL) {
					if( strcmp(entIN->d_name, ".") != 0 && strcmp(entIN->d_name, "..") != 0 ){
						std::cout << entIN->d_name<< std::endl;


						char actionName[1000];
						sprintf (actionName, "%s/%s",pathOut,entIN->d_name);
						result = mkdir(actionName,0777);
						
						char OutputHandName[1000];
						sprintf (OutputHandName, "%s/handDepth",actionName);
						result = mkdir(OutputHandName,0777);
						
						char OutputObjectName[1000];
						sprintf (OutputObjectName, "%s/objectDepth",actionName);
						result = mkdir(OutputObjectName,0777);
						
						char OutputDepthMapName[1000];
						sprintf (OutputDepthMapName, "%s/sceneDepth",actionName);
						result = mkdir(OutputDepthMapName,0777);
						
						char OutputRGBHandName[1000];
						sprintf (OutputRGBHandName, "%s/handRGB",actionName);
						result = mkdir(OutputRGBHandName,0777);
						
						char OutputRGBObjectName[1000];
						sprintf (OutputRGBObjectName, "%s/objectRGB",actionName);
						result = mkdir(OutputRGBObjectName,0777);
						
						char OutputRGBName[1000];
						sprintf (OutputRGBName, "%s/sceneRGB",actionName);
						result = mkdir(OutputRGBName,0777);
						
						char depth[120];
						sprintf (depth, "%s/%s/depth",pathIn,entIN->d_name);
						char rgb[100];
						sprintf (rgb, "%s/%s/rgb",pathIn,entIN->d_name);
						//char flow[100];
						//sprintf (flow, "%s/%s/flow",pathIn,entIN->d_name);
						



						struct dirent **namelist;
					    int n;
						int i =0;
					   	n = scandir(depth, &namelist, NULL, alphasort);
					    if (n < 0)
					        perror("scandir");
					    else {


					    	while (i<n) {
					    		
					    		if( strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0 ){
								//printf("%s\n", namelist[i]->d_name);
								
								char rgbName[100];
								sprintf (rgbName, "%s/%s",rgb,namelist[i]->d_name);
								char depthName[100];
								sprintf (depthName, "%s/%s",depth,namelist[i]->d_name);
			
								img = imread(rgbName, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH); 
								imgd = imread(depthName, CV_LOAD_IMAGE_ANYDEPTH);
								if((!img.data) || (!imgd.data)) cout << "Image Error" << std::endl;
								imgd.convertTo(imgd, CV_32F);
								
								fullDepth_c = img.clone();
								object_c = img.clone();
								hand_c = img.clone();
								
								//remove green background
								_removeGreen(img, imgd);
								DepthMap = imgd.clone();
								fullRGBimage = img.clone();
								img_skin = img.clone();
								Mat img_skin_object(img.size(), CV_8UC3);
								
								//detect hand/skin color
								_detectSkinColor(img_skin, img_skin_object);
								imgd_object = imgd.clone();
								
								//separate hand from object and produce two depth maps
								_separateHand(img_skin, imgd, imgd_object);
								
								//colorize depth frames
								fullDepth_c = _colorizeDepth(DepthMap);
								hand_c = _colorizeDepth(imgd);
								object_c = _colorizeDepth(imgd_object);
		
								//crop frames to 300x300
								imgd_crop = hand_c(roi);
								imgd_object_crop = object_c(roi);
								DepthMap_crop = fullDepth_c(roi);
								
								//crop RGB frames to 300x300
								img_skin_crop = img_skin(roi);
								img_skin_object_crop = img_skin_object(roi);
								fullRGBimage_crop = fullRGBimage(roi);
								
								Mat channels[3];
								split(DepthMap_crop, channels);
								channels[0] = Mat::zeros(DepthMap_crop.rows, DepthMap_crop.cols, CV_8UC1);
								merge(channels, 3, DepthMap_crop);
								
								split(imgd_crop, channels);
								channels[0] = Mat::zeros(imgd_crop.rows, imgd_crop.cols, CV_8UC1);
								merge(channels, 3, imgd_crop);
								
								split(imgd_object_crop, channels);
								channels[0] = Mat::zeros(imgd_object_crop.rows, imgd_object_crop.cols, CV_8UC1);
								merge(channels, 3, imgd_object_crop);
								Mat lel;
								char save_OutputHandName[1000],save_OutputObjectName[1000],save_OutputDepthMapName[1000], save_OutputRGBHandName[1000], save_OutputRGBObjectName[1000], save_OutputRGBName[1000];
								sprintf (save_OutputHandName, "%s/%s",OutputHandName,namelist[i]->d_name);
								sprintf (save_OutputObjectName, "%s/%s",OutputObjectName,namelist[i]->d_name);
								sprintf (save_OutputDepthMapName, "%s/%s",OutputDepthMapName,namelist[i]->d_name);
								sprintf (save_OutputRGBHandName, "%s/%s",OutputRGBHandName,namelist[i]->d_name);
								sprintf (save_OutputRGBObjectName, "%s/%s",OutputRGBObjectName,namelist[i]->d_name);
								sprintf (save_OutputRGBName, "%s/%s",OutputRGBName,namelist[i]->d_name);
								imwrite(save_OutputHandName, imgd_crop);
								imwrite(save_OutputObjectName, imgd_object_crop);
								imwrite(save_OutputDepthMapName, DepthMap_crop);
								//cvtColor(img_skin_crop, lel, CV_BGR2HSV);
								//imshow("lel", lel);
								//waitKey(0);
								imwrite(save_OutputRGBHandName, img_skin_crop);
								imwrite(save_OutputRGBObjectName, img_skin_object_crop);
								imwrite(save_OutputRGBName, fullRGBimage_crop);
								
					    		}
						         i++;
						         free(namelist[i-1]);
						      }
						      free(namelist);

					  
					    }
						
					}
				}
			}
		}
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	  return EXIT_FAILURE;
	}
	return 0;
}
