#ifndef IMAGE_TOOLS
#define IMAGE_TOOLS

#include <iostream>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <vector>
#include <windows.h>
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
		for(int y = 0; y < length; y++){
		    for(int x = 0; x < width; x++){
			    if(samplesPerPixel == 1){
				    float val = (float) pixelRef[y][x][0] / 255;
				    glColor3f(val, val, val);
			    }else if(samplesPerPixel == 2){
				    float val1 = (float) pixelRef[y][x][0] / 255;
				    float val2 = (float) pixelRef[y][x][1] / 255;
				    glColor3f(val1, val2, 0);
			    }else if(samplesPerPixel == 3){
				    float val1 = (float) pixelRef[y][x][0] / 255;
				    float val2 = (float) pixelRef[y][x][1] / 255;
				    float val3 = (float) pixelRef[y][x][2] / 255;
				    glColor3f(val1, val2, val3);
			    }
			// reading from the first row but drawing it from the end because the origin is at bottom left 
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
