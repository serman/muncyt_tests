/*
 *  SlitScanGL.h
 *  SlitScanGL
 *
 *  Created by guillermo casado on 02/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

/**
 * 
 * The MIT License
 * 
 * Copyright (c) 2010, 2011 James George http://www.jamesgeorge.org
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * ofxSlitScan.h
 *
 * ofxSlitScan is a simple utility for creating slit scan
 * effects and video time delays. It works by keeping a 
 * rolling buffer of video frames that can be sampled
 * from arbitrarly via a warp map 
 *
 * For inspiring works using this effect, check out Golan Levin's page
 * on Slit Scan artworks
 *
 * http://www.flong.com/texts/lists/slit_scan/
 *
 *
 * Usage:
 *
 * First call set up with the width and height that matches the video you want to process
 * after calling setup, use any image to set the delay map 
 * then with every incoming frame call addImage();
 * Calling getOutputImage will return a reference to the warped / delayed video frame
 */

#ifndef _OFX_SLITSCAN_GL
#define _OFX_SLITSCAN_GL

#import "ofMain.h"


#define MAX_CAPACITY 12


class ofxSlitScanGL {
public:
	ofxSlitScanGL() {};
	
	void update();
	
    ofFbo       fboPingPong[2];
	int			timer;
    ofShader    shader;
	
	ofTexture	mapaActTex;

	ofMesh		my_mesh;
	ofVbo		my_vbo;
	
	
	
	/**
	 * Width / Height of input stream
	 * El maximo del shader es 12 frames
	 * capacity indica el numero de frames con el que se trabaja ahora
	 
	 * type is  OF_IMAGE_GRAYSCALE, OF_IMAGE_COLOR, or OF_IMAGE_COLOR_ALPHA
	 * default type is OF_IMAGE_COLOR
	 */
	void	setup(int w, int h);
	void	setup(int w, int h, int _capacity);
//	void setup(int w, int h, int _capacity, ofImageType type);
	
//	
	bool isSetup();
//	
//	/**
//	 * set the map used to distort the frame buffer.
//	 * the Map is a grayscale image the same dimensions
//	 * as the input stream. 
//	 * White pixels map to the newest frames and black the oldest,
//	 * gray in between.
//	 */
    void setDelayMap(ofBaseHasPixels& map);
    void setDelayMap(ofPixels& map);
	void setDelayMap(unsigned char* map, ofImageType type);
	void setDelayMap(float* map);
	
	void setDelayMap(ofImage& img);
	void setDelayMap(ofTexture& tex);
	
//	
//	/**
//	 * add an image to the input system
//	 * call this in succession, once per frame, when reading
//	 * an input movie or video stream
//	 */
////	void addImage(ofBaseHasPixels& image);
////	void addImage(ofPixels& image);
////	void addImage(unsigned char* image);
//	
	// add un frame a la secuencia general (push de la secuencia)
	void addFrameTex(ofTexture& tex);
	// inserta un frame en una de los tercios de la secuencia de frames (push en ese tercio)
	void addFrameTexIn3(ofTexture& tex, int n3);
	// inserta un frame en una de los cuartos de la secuencia de frames (push en ese cuarto)
	void addFrameTexIn4(ofTexture& tex, int n4);
	
	// set un frame de la secuencia (no hace push)
	void setFrameTexInFrame(ofTexture& tex, int nFrame);
	
	// secuencia de frames
	vector<ofTexture>	frameTexs;
	vector<ofFbo>		frameFbos;
	
	void		cleanFrames();
	
	void		setCapacity(int _capacity);
	int			getCapacity();
//	
//	/**
//	 * returns the results of the
//	 * distortion
//	 */
////	ofImage& getOutputImage();
//
	ofTexture& getOutputTex();
//
//// NO
//	/**
//	 * gives you the output image back as an ofImage
//	 */
//	ofImage& getDelayMap();
	ofTexture& getDelayMapTex();

	
//	/**
//	 * Allows clamping of the delay amount and width of the delay within the capacity
//	 * timeDelay + timeWidth must be less than the total capacity.
//	 * these values can be scrubbed in real time for glitch and speed ramp effects
//	 */
//	void setTimeDelayAndWidth(int timeDelay, int timeWidth); 
//	void setTimeDelay(int timeDelay);
//	void setTimeWidth(int timeWidth);
//	
//	/**
//	 * turn on to smooth inter-frame differences
//	 */
//	void setBlending(bool blend);
//	void toggleBlending();
//	
//	int getTimeDelay();
//	int getTimeWidth();
	int getWidth();
	int getHeight();
//	int getCapacity();
	int getNumFrames();
	
//	ofImageType getType();
//	bool isBlending();
//
	
protected:
//	bool blend;
//	bool outputIsDirty;
//	ofImage outputImage;
	ofTexture	outputTex;
	
//	bool delayMapIsDirty;
//	ofImage delayMapImage;
	ofTexture	delayMapTex;
	
	int timeWidth, timeDelay, framepointer, capacity;
	
	int width, height;
	
//	ofImageType type;
//	int bytesPerPixel;	
//	int bytesPerFrame;
//	bool buffersAllocated;
};

#endif