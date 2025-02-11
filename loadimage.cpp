#include <iostream>
#include <vector>
#include <string>
#include "./image.h"
#include "imageTools.h"

using namespace std;
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
	Image image(imagePath);
	cout << "min: " << image.min() << "  max: " << image.max() << endl;
	//open the image
	system(imagePath.c_str());
	image.sobel(true, true);
	image.writeOrigin();
	image.threshold(50);

	imageTools::drawImage(image);

}







