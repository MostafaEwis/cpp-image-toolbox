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
	        throw invalid_argument("No path specified.");
	}
	system(imagePath.c_str());
	Image image(imagePath);
	cout << "min: " << image.min() << "  max: " << image.max() << endl;
	image.toGray();
	//brighten the image too much that the blackest parts are the only remaining
	image.histEqual();
	image.logTrans(42);
	image.writeOrigin();
	image.threshold(250, true);
	image.invert();
	image.writeOrigin();
	image.open(25);
	image.writeOrigin();
	Image image2(imagePath);
	image2.overlay(image);
	imageTools::drawImage(image2);
}







