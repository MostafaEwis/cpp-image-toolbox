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
	void printImageData();
	void localHistEqual(int size);
	void histEqual();
	void logTrans(int c);
	void threshold(int thresh, bool origin);
	void toGray();
	void displayImageData();
	void erode(int maskSize, bool mErode);
	void open(int maskSize);
	void close(int maskSize);
	void contour(int maskSize);
	void writeOrigin();
	const int getWidth();
	const int getLength();
	const int getSamplesPerPixel();
	const vector<vector<vector<int>>>& getPixelsOrigin();
	const int min();
	const int max();
	void contrastStretch(int newMin = 0, int newMax = 255, bool calcMinMax = true);
	void blur(int level, bool origin = true);
	void bitLevel(int l);
	void laplace(bool origin);
	void unsharp(float k);
	void printImageDkta();
	vector<vector<vector<int>>>& getPixels();
	const int getFileSize(){return imageData.size();}
	static string lookUpTag(string hexStr);
	string getBytes(int start, int end, bool rev);
	void loadPixels();

private:
		string path;
		int width, length,rowsPerStrip,  samplesPerPixel;
		int maxVal = -1;
		int minVal = 256;
		vector<int> imageData;
		vector<int> stripByteCount;
		vector<int> stripOffset;
		//width x height x samples per pixel
		vector<vector<vector<int>>> pixels;
		vector<vector<vector<int>>> pixelsOrigin;
};
#endif
