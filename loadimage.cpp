#include <iostream>
#include <vector>
#include <string>
#include "./image.h"
#include "imageTools.h"

using namespace std;
//show image
//write output images

//notes:
//bileve images are stored as bit maps, e.g. a bilevel image of black and white will have 8 images each for each bit

//refactor long lines
//	void drawImage(vector<vector<vector<int>>>& pixels, int& width, int& length, int& samplesPerPixel){
int main(int argc, char* argv[]){
	string imagePath;
	if(argc > 1){
		imagePath = argv[1];
	}else{
	        throw invalid_argument( "No path specified." );
	}

	Image image(imagePath);
	image.loadImage();
	image.IFD();
	image.loadPixels();
	imageTools::drawImage(image);
}







