#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "hexTools.h"
#include "image.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

const int Image::getWidth(){
	return width;
}
const int Image::getLength(){
	return length;
}
const int Image::getSamplesPerPixel(){
	return samplesPerPixel;
}
void Image::loadPixels(){
	for(int stripCount = 0; stripCount < stripOffset.size(); stripCount++){
		int stripStart = stripOffset[stripCount];
		int stripEnd = stripStart + stripByteCount[stripCount];
		for(int row = stripStart; row < stripEnd; row+=width * samplesPerPixel){
			pixels.push_back({});
			for(int pixel = row; pixel < row + width * samplesPerPixel; pixel+=samplesPerPixel){
				pixels[pixels.size() - 1].push_back({});
				for(int sample = 0; sample < samplesPerPixel; sample++){
					int sampleVal =
						hexTools::hexToInt(getBytes(pixel + sample,
									pixel + sample + 1, true));
					int latestRow = pixels.size() - 1;
					int latestPixel = pixels[latestRow].size() - 1;
					pixels[latestRow][latestPixel].push_back(sampleVal);

				}
			}
		}
	}
}
			
void Image::displayImageData(){
	cout << "width: " << width <<endl;
	cout << "height: " << length << endl;
	cout << "rows per strip: " << rowsPerStrip << endl;
	cout << "samples per pixel: " << samplesPerPixel << endl;
	cout << "strip byte count: ";
	for(auto byteCount : stripByteCount)
		cout << byteCount << " ";
	cout << endl;
	cout << "strip offset: ";
	for(auto offset: stripOffset)
		cout << offset << " ";
	cout << endl;
}
string Image::lookUpTag(string hexStr){
	if(hexStr == "0100") return "Image Width";
	else if(hexStr == "0101") return "Image Length";
	else if(hexStr == "0102") return "Bits Per Sample";
	else if(hexStr == "0103") return "Comperssion";
	else if(hexStr == "0106") return "Photometric Interpretation";
	else if(hexStr == "0107") return "Threshholding";
	else if(hexStr == "010e") return "Image Description";
	else if(hexStr == "0111") return "Strip Offset";
	else if(hexStr == "0112") return "Orientation";
	else if(hexStr == "0115") return "Samples Per Pixel";
	else if(hexStr == "0116") return "Rows Per Strip";
	else if(hexStr == "0117") return "Strip Byte Counts";
	else if(hexStr == "011a") return "X Resolution";
	else if(hexStr == "011b") return "Y Resolution";
	else if(hexStr == "011c") return "Planar Configuration";
	else if(hexStr == "0128") return "Resolution Unit";
	else if(hexStr == "0129") return "Page Number";
	else return hexStr;

}
void Image::loadImage(){
		ifstream image(path);

		//get the length of file
		int length = 0;
		image.seekg(0, image.end);
		length = image.tellg();
		image.seekg(0, image.beg);
		
		//create buffer
		char* imageBuffer = new char[length];

		//read into buffer
		image.read(imageBuffer, length);

		//transform buffer to data
		for(size_t i = 0; i < length; i++) imageData.push_back((int)(uint8_t)imageBuffer[i]);
		
		//free
		image.close();
		delete [] imageBuffer;

}


string Image::getBytes(int start, int end, bool rev = true){
		stringstream hexStream("");
		for(int i = end - 1; i >= start; i--){
			hexStream << setw(2) << setfill('0') << hex << imageData[i];
		}
		string revStr = hexStream.str();
		if(rev) reverse(revStr.begin(), revStr.end());
		return revStr;
}

int Image::offset(){
		string hexStr = getBytes(4, 8);
		return hexTools::hexToInt(hexStr);
}

vector<vector<vector<int>>>& Image::getPixels(){
	return pixels;
}
void Image::printImageData(){
	int off = offset();
	string hexArgu = getBytes(off, off + 2);	
	int argu = hexTools::hexToInt(hexArgu);
	int start = off + 2;
	int finish = start + argu * 12;
	for(int i = start; i < finish; i+=12){
		string hexTag = getBytes(i, i + 2, false);
		int type = hexTools::hexToInt(getBytes(i + 2, i + 4));
		int count = hexTools::hexToInt(getBytes(i + 4, i + 8));
		int value = hexTools::hexToInt(getBytes(i + 8, i + 12));
		cout <<setw(16) <<left<< lookUpTag(hexTag) << " " << type << " " << count << " " << value << endl;
	}
}

void Image::IFD(){
	int off = offset();
	string hexArgu = getBytes(off, off + 2);	
	int argu = hexTools::hexToInt(hexArgu);
	int start = off + 2;
	int finish = start + argu * 12;
	for(int i = start; i < finish; i+=12){
		string hexTag = getBytes(i, i + 2, false);
		int type = hexTools::hexToInt(getBytes(i + 2, i + 4));
		type = type == 3 ? 2 : type;
		int count = hexTools::hexToInt(getBytes(i + 4, i + 8));
		int value = hexTools::hexToInt(getBytes(i + 8, i + 12));
		cout <<setw(16) <<left<< lookUpTag(hexTag) << " " << type << " " << count << " " << value << endl;
		if(hexTag == "0100"){
			width = value;
		}else if(hexTag == "0101"){
			length = value;
		}else if(hexTag == "0111"){
			if(count == 1)
				stripOffset.push_back(value);
			else
				for(int i = value; i < value + type * count; i+=type){
					stripOffset.push_back(hexTools::hexToInt(getBytes(i, i + type)));
				}
		}else if(hexTag == "0115"){
			samplesPerPixel = value;
		}else if(hexTag == "0117"){
			if(count == 1)
				stripByteCount.push_back(value);
			else
				for(int i = value; i < value + type * count; i+=type){
					stripByteCount.push_back(hexTools::hexToInt(getBytes(i, i + type)));
				}
		}else if(hexTag == "0116"){
			rowsPerStrip = value;
		}
	}
}
