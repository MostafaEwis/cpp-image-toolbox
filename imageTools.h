#ifndef IMAGE_TOOLS
#define IMAGE_TOOLS

#include <iostream>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <vector>
#include <windows.h>
#include <cmath>
#include "image.h"

using namespace std;

namespace imageTools{
	int width, length, samplesPerPixel;
	int margin = 50;
	vector<vector<vector<int>>>* pixels;

	void myInit(int width, int length)
	{
	    glClearColor(1.0, 1.0, 1.0, 0.0); // white
	    glColor3f(0.0f, 0.0f, 0.0f);
	    glPointSize(4.0);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluOrtho2D(0.0, width + margin, 0.0, length + margin);
	}
	void plotImage(){
		glBegin(GL_POINTS);
		vector<vector<vector<int>>> &pixelRef = *pixels;
	//	//6 phases 
	//	//each phase a color is on another is off another is gradual
	//	//Y = (R × 0.299) + (G × 0.587) + (B × 0.114)
	//	//1.40625
	//	vector<vector<float>> rainbow(256, vector<float>(3, 0.0f));
	//	int i = 0;
	//	float deg = 0;
	//	while(i < rainbow.size()){
	//		float& r = rainbow[i][0];
	//		float& g = rainbow[i][1];
	//		float& b = rainbow[i][2];
	//		float colorOfDeg = modf(deg / 60.0f, nullptr);
	//		if(deg >= 0 && deg < 60){
	//			r = 1;
	//			b = colorOfDeg;
	//			g = 0;
	//		}else if(deg >= 60 && deg < 120){
	//			r = 1 - colorOfDeg;
	//			b = 1;
	//			g = 0;
	//		}else if(deg >= 120 && deg < 180){
	//			r = 0;
	//			b = 1;
	//			g = colorOfDeg;
	//		}else if(deg >= 180 && deg < 240){
	//			r = 0;
	//			b = 1 - colorOfDeg;
	//			g = 1;

	//		}else if(deg >= 240 && deg < 300){
	//			r = colorOfDeg;
	//			b = 0;
	//			g = 1;
	//		}else if(deg >= 300 && deg < 360){
	//			r = 1;
	//			b = 0;
	//			g = 1 - colorOfDeg;
	//		}
	//		i++; 
	//		deg+=1.40625;
	//	}
			for(int y = 0; y < length; y++){
			    for(int x = 0; x < width; x++){
				    if(samplesPerPixel == 1){
					    //vector<float>& color = rainbow[pixelRef[y][x][0]];
					    float val = (float) pixelRef[y][x][0] / 255.0f;
					    glColor3f(val, val, val);
				    }else if(samplesPerPixel == 2){
					    float val1 = (float) pixelRef[y][x][0] / 255.0f;
					    float val2 = (float) pixelRef[y][x][1] / 255.0f;
					    glColor3f(val1, val2, 0);
				    }else if(samplesPerPixel == 3){
					    float val1 = (float) pixelRef[y][x][0] / 255.0f;
					    float val2 = (float) pixelRef[y][x][1] / 255.0f;
					    float val3 = (float) pixelRef[y][x][2] / 255.0f;
					    glColor3f(val1, val2, val3);
				    }
				//reading from the first row but drawing it from the end because the origin is at bottom left 
				    glVertex2i(x + margin / 2, length - y + margin / 2);
			    }
			}
			glEnd();
	}
	void myDisplay()
	{
	    glClear(GL_COLOR_BUFFER_BIT);
	    glColor3f(1, 1, 1);
	    plotImage();	 
	    glFlush();
	}

	void drawImage(Image& image){
		width = image.getWidth();
		length = image.getLength();
		samplesPerPixel= image.getSamplesPerPixel();
		pixels = &image.getPixels();
		int dullArgc = 1;
		int * dullArgcPtr = &dullArgc;
		char dullArgv = 'a';
		char * dullArgvPtr = &dullArgv; 
		glutInit(dullArgcPtr, &dullArgvPtr);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(width + margin, length + margin);
		glutInitWindowPosition(400, 100);
		glutCreateWindow("Abdo Nawar");
		glutDisplayFunc(myDisplay);
		myInit(width, length);
		glutMainLoop();
	}
}

#endif
