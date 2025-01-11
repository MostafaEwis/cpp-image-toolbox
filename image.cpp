#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "hexTools.h"
#include "image.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <map>

using namespace std;


const int Image::max(){
	return maxVal;
}
const int Image::min(){
	return minVal;
}

void Image::overlay(Image& otherImage, bool edge){
	//this needs to modified so that images of different width and heights can be added
	if(width != otherImage.getWidth()|| length != otherImage.getLength()){
		cout << "can't overlay images of different dimentions" << endl;
		return;
	}
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			if(edge){
				if(otherImage.getPixels()[y][x][0] == 255){
						pixels[y][x][0] = 255;
						pixels[y][x][1] = 0;
						pixels[y][x][2] = 0;
				}
			}else{
				pixels[y][x][0] = pixels[y][x][0] < otherImage.getPixels()[y][x][0] ? pixels[y][x][0] : otherImage.getPixels()[y][x][0];

			}
		}
	
	}
}
void Image::laplace(){
	int val = 0;
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			(y + 1 < length) && (val += pixelsOrigin[y + 1][x][0]);
			(y - 1 >= 0) && (val += pixelsOrigin[y - 1][x][0]);
			(x + 1 < width) && (val += pixelsOrigin[y][x + 1][0]);
			(x - 1 >= 0) && (val += pixelsOrigin[y][x - 1][0]);
			val -= 4 * pixels[y][x][0];

			pixels[y][x][0] = val;

			val = 0;
		}
	}
}
void Image::threshold(int thresh){
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			pixels[y][x][0] = pixelsOrigin[y][x][0] < thresh ? 0 : 255;
		}
	}

}
void Image::invert(){
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			pixels[y][x][0] = pixels[y][x][0] == 255? 0 : 255;
		}
	}
}
void Image::contour(int kernel, int thresh){
	threshold(thresh);
	erode(kernel);
	for(int y = 0; y < length; y++)
		for(int x = 0; x < width; x++)
			pixels[y][x][0] = (bool)(pixels[y][x][0] == 0 && pixelsOrigin[y][x][0] == 255) ? 255 : 0;
}
void Image::erode(int kernel){
	int gap = (kernel- 1) / 2;
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			if(pixelsOrigin[y][x][0] == 255){
				int state = 1;
				vector<int> mask;
				for(int maskY = y - gap; maskY <= y + gap; maskY++){
					for(int maskX = x - gap; maskX <= x + gap; maskX++){
							if(maskX < 0 || maskX >= width || maskY < 0 || maskY >= length) continue;
							mask.push_back(pixelsOrigin[maskY][maskX][0] == 255 ? 1 : 0);	
					}
				}
				for(int e : mask) state *= e;
				pixels[y][x][0] = (int)(bool)state * 255;
			}

		}
	}
}
void Image::dilate(int kernel){
	int gap = (kernel - 1) / 2;
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			if(pixelsOrigin[y][x][0] == 0){
				int state = 0;
				vector<int> mask;
				for(int maskY = y - gap; maskY <= y + gap; maskY++){
					for(int maskX = x - gap; maskX <= x + gap; maskX++){
							if(maskX < 0 || maskX >= width || maskY < 0 || maskY >= length) continue;
							mask.push_back(pixelsOrigin[maskY][maskX][0] == 255 ? 1 : 0);	
					}
				}
				for(int e : mask) state += e;
				pixels[y][x][0] = (bool)state * 255;
			}

		}
	}
}
void Image::writeOrigin(){
	for(int y = 0; y < length; y++)
		for(int x = 0; x < width; x++)
			pixelsOrigin[y][x][0] = pixels[y][x][0];
}
void Image::open(int kernel){
	erode(kernel);
	writeOrigin();
	dilate(kernel);
}
void Image::close(int kernel){
	dilate(kernel);
	writeOrigin();
	erode(kernel);
}
void Image::unsharp(float k){
	//get a blured copy of current
	//subtract from current
	//add to origin
	blur(3);
	for(int y = 0; y < length; y++)
		for(int x = 0; x < width; x++)
			pixels[y][x][0] = pixelsOrigin[y][x][0] + (uint32_t)round(k * (pixelsOrigin[y][x][0] - pixels[y][x][0]));
		
}
void Image::logTrans(float c){
	//32*log(r);
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			pixels[y][x][0] = (uint32_t)round(c * log2(pixelsOrigin[y][x][0]));
		}
	}
}
void Image::sobel(bool h, bool v){
	int vl = 0;
	int vr = 0;
	int hl = 0;	
	int hr = 0;	
		for(int y = 0; y < length; y++){
			for(int x = 0; x < width; x++){
				if(h){
					(y + 1 < length && x + 1 < width) && (hr+= pixelsOrigin[y + 1][x + 1][0]);
					(y - 1 > 0 && x + 1 < width) && (hr+= pixelsOrigin[y - 1][x + 1][0]);
					(y + 1 < length && x - 1 > 0) && (hr-= pixelsOrigin[y + 1][x - 1][0]);
					(y - 1 > 0 && x - 1 > 0) && (hr-= pixelsOrigin[y - 1][x - 1][0]);

					(x + 1 < width) && (hr+= 2 * pixelsOrigin[y][x + 1][0]);
					(x - 1 >= 0) && (hr-= 2 * pixelsOrigin[y][x - 1][0]);

					(y + 1 < length && x + 1 < width) && (hl-= pixelsOrigin[y + 1][x + 1][0]);
					(y - 1 > 0 && x + 1 < width) && (hl-= pixelsOrigin[y - 1][x + 1][0]);
					(y + 1 < length && x - 1 > 0) && (hl+= pixelsOrigin[y + 1][x - 1][0]);
					(y - 1 > 0 && x - 1 > 0) && (hl+= pixelsOrigin[y - 1][x - 1][0]);

					(x + 1 < width) && (hl-= 2 * pixelsOrigin[y][x + 1][0]);
					(x - 1 >= 0) && (hl+= 2 * pixelsOrigin[y][x - 1][0]);
				}
				if(v){
					(y + 1 < length && x + 1 < width) && (vl -= pixelsOrigin[y + 1][x + 1][0]);
					(y - 1 > 0 && x + 1 < width) && (vl += pixelsOrigin[y - 1][x + 1][0]);
					(y + 1 < length && x - 1 > 0) && (vl -= pixelsOrigin[y + 1][x - 1][0]);
					(y - 1 > 0 && x - 1 > 0) && (vl += pixelsOrigin[y - 1][x - 1][0]);

					(y + 1 < length) && (vl += 2 * pixelsOrigin[y + 1][x][0]);
					(y - 1 >= 0) && (vl -= 2 * pixelsOrigin[y - 1][x][0]);
				}

				pixels[y][x][0] = hl < hr ? hr : hl;
				pixels[y][x][0] += vl < vr ? vr : vl;
				vl = 0;
				hl = 0;
				vr = 0;
				hr = 0;

			}
		}
}
void Image::localHistEqual(int size){
	//if I intially creat a map here and then clear it, this wouldn't work for some reason so I have to creat a map inside the for loop each time I iterate over a pixel
	//the main reason I want to intialize a map once and then reuse it, is to not have to reallocate memory repeatedly, which is so expensive as far as I know.
	//this needs to be heavely debuged but it's 5AM and I have had no sleep and I barely am.
	
	float mn = 1 / (float)(size * size);
	
	int gap = (size - 1) / 2;

	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			map<int, int> freq;
			map<int, float> cdf;
			//get the frequency array;
			for(int maskY = y - gap; maskY <= y + gap; maskY++){
				for(int maskX = x - gap; maskX <= x + gap; maskX++){
					if(maskY < 0 || maskY > length - 1 || maskX < 0 || maskX > width - 1) continue;
					freq[pixelsOrigin[maskY][maskX][0]]++;
				}
			}
			//get the cdf array
			cdf[freq.begin() -> first] = (float)(freq.begin() -> second) * mn;
			for(map<int,int>::iterator it = next(freq.begin() ,1); it != freq.end(); ++it){
				cdf[it->first] = cdf[prev(it, 1) -> first] + (float)(it -> second) * mn;	
			}
				
			pixels[y][x][0] = round(255.0f * cdf[pixelsOrigin[y][x][0]]);
		}
		//freq.clear();
		//cdf.clear();
	}


}
void Image::histEqual(){
	vector<int> freq(256, 0);
	vector<float> cdf(256,0);

	float mn = 1 / (float)(width * length);
	for(int y = 0; y < length; y++)
		for(int x = 0; x < width; x++)
			freq[pixelsOrigin[y][x][0]]++;

	cdf[0] = (float)freq[0] * mn;
	for(int i = 1; i < cdf.size(); i++)
		cdf[i] = cdf[i - 1] + (float)freq[i] * mn;

	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			pixels[y][x][0] = round(255.0f * cdf[pixelsOrigin[y][x][0]]);
		}
	}
}
const vector<vector<vector<int>>> & Image::getPixelsOrigin(){
	return pixelsOrigin;
}
void Image::bitLevel(int l){
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			pixels[y][x][0] = bitset<8>{(uint8_t)pixelsOrigin[y][x][0]}[l] == 0 ? 0 : 255;
		}
	}

}
void Image::blur(int level){
	if(level%2 == 0) throw invalid_argument( "blurImage: Mask dimensions should be odd" );
	//gap = level - 1 / 2
	//calc through the gap by traversing
	//myltiply by 1 / level**2
	int gap = (level - 1) / 2;
	float div = 1.0f / (float)(level * level);
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			float val = 0.0f;
			for(int maskY = y - gap; maskY <= y + gap; maskY++){
				for(int maskX = x - gap; maskX <= x + gap; maskX++){
					if(maskX < 0 || maskX >= width || maskY < 0 || maskY >= length)
						val += minVal;	
					else
						val += pixelsOrigin[maskY][maskX][0];
				}
			}
			
		//	cout << y << "   " << x << "   rBefore: " << val << endl;
			val *= div; 
		//	cout << y << "   " << x << "   rAfter: " << val << endl;
			pixels[y][x][0] = round(val);
		}
	}
}

void Image::toGray(){
	if(samplesPerPixel < 2) return;
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			float rVal = 0.0f;
			for(int s = 0; s < samplesPerPixel; s++){
				rVal += pixelsOrigin[y][x][s];
			}
			pixelsOrigin[y][x][0] = round(rVal / 3.0f);
			for(int s = 1; s < samplesPerPixel; s++){
				pixels[y][x].pop_back();
			}
		}
	}
	samplesPerPixel = 1;
}

void Image::contrastStretch(int newMin, int newMax, bool calcMinMax){
	//calclualte min max is used when you have alterd an image and now need to get the new minimum and maximum of the image
	int min = calcMinMax? 256 : minVal;
	int max = calcMinMax? 0 : maxVal;
	if(calcMinMax){
		for(int y = 0; y < length; y++){
			for(int x = 0; x < width; x++){
				if(min > pixels[y][x][0]) min = pixels[y][x][0];
				if(max < pixels[y][x][0]) max = pixels[y][x][0];
			}
		}
	}
	cout << min << "   " << max << endl;
	for(int y = 0; y < length; y++){
		for(int x = 0; x < width; x++){
			int &val = pixels[y][x][0];
			val = round((float)(val - min) / (float)(max - min) * newMax + newMin);
		}
	}

}
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
	//I though at first this was a very bad implementation, due to the vector growing and copying datea over and over again,
	//but then I realized it's using the c++11 move mechnaism.	
	for(int stripCount = 0; stripCount < stripOffset.size(); stripCount++){
		int stripStart = stripOffset[stripCount];
		int stripEnd = stripStart + stripByteCount[stripCount];
		for(int row = stripStart; row < stripEnd; row+=width * samplesPerPixel){
			pixels.push_back({});
			pixelsOrigin.push_back({});
			for(int pixel = row; pixel < row + width * samplesPerPixel; pixel+=samplesPerPixel){
				pixels[pixels.size() - 1].push_back({});
				pixelsOrigin[pixelsOrigin.size() - 1].push_back({});
				for(int sample = 0; sample < samplesPerPixel; sample++){
					int sampleVal =
						hexTools::hexToInt(getBytes(pixel + sample,
									pixel + sample + 1, true));
					int latestRow = pixels.size() - 1;
					int latestPixel = pixels[latestRow].size() - 1;
					if(sampleVal < minVal) minVal = sampleVal;
					if(sampleVal > maxVal) maxVal = sampleVal;
					pixels[latestRow][latestPixel].push_back(sampleVal);
					pixelsOrigin[latestRow][latestPixel].push_back(sampleVal);

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
		if(!image){
			throw invalid_argument( "the image may not exist or is not available at the moment");
		}
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
Image::Image(string path):path{path}{
	loadImage();
	IFD();
	loadPixels();
}

