#ifndef IMAGE_H
#define IMAGE_H
#include <vector>
#include <string>
using namespace std;

class Image{
public:

	Image(string path):path{path}{}
	int offset();
	void loadImage();
	void IFD();
	void displayImageData();
	void drawImage();
	void displayImage(int *argc, char **argv);
	void printImageData();
	const vector<vector<vector<int>>>& getPixels();
	const int getFileSize(){return imageData.size();}
	static string lookUpTag(string hexStr);
	string getBytes(int start, int end, bool rev);
	void loadPixels();

private:
		string path;
		int width, length, rowsPerStrip,  samplesPerPixel;
		vector<int> imageData;
		vector<int> stripByteCount;
		vector<int> stripOffset;
		//width x height x samples per pixel
		//vector<vector<vector<int>>> pixels;
		vector<vector<vector<int>>> pixels;
};
#endif
