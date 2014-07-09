/*
 *  SlitScanGL.cpp
 *  SlitScanGL
 *
 *  Created by guillermo casado on 02/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "SlitScanGL.h"
void ofxSlitScanGL::setup(int w, int h) {

	setup(w,h,MAX_CAPACITY);
}

void ofxSlitScanGL::setup(int w, int h, int _capacity) {

	width = w;
	height = h;
	
	capacity = (_capacity>MAX_CAPACITY)? MAX_CAPACITY:_capacity;
	
	
	// preparar shader, fbo...
	
	shader.load( "shaders/multiTex.vert","shaders/multiTex.frag");
	
//    fbo.allocate(width,height);
//	
//    // Let´s clear the FBO´s
//    fbo.begin();
//    ofClear(0,0,0,255);
//    fbo.end();
    
	for(int i=0; i<2; i++) {
		fboPingPong[i].allocate(width,height);
		fboPingPong[i].begin();
		ofClear(0,0,0,255);
		fboPingPong[i].end();	
	}
	
	timer = 0;
	
	
	my_mesh.addVertex(ofVec3f(0,0,0)); 
	my_mesh.addColor(ofColor(255.0)); 
	my_mesh.addTexCoord(ofVec2f(0,0));
	
	my_mesh.addVertex(ofVec3f(width,0,0));
	my_mesh.addColor(ofColor(255.0)); 
	my_mesh.addTexCoord(ofVec2f(width,0));
	
	my_mesh.addVertex(ofVec3f(width,height,0));
	my_mesh.addColor(ofColor(255.0)); 
	my_mesh.addTexCoord(ofVec2f(width,height));
	
	my_mesh.addVertex(ofVec3f(0,height,0));
	my_mesh.addColor(ofColor(255.0)); 
	my_mesh.addTexCoord(ofVec2f(0,height));	
	
	my_vbo.setMesh(my_mesh, GL_STATIC_DRAW);
	
	
	
}

void ofxSlitScanGL::setCapacity(int _capacity) {
	capacity = (_capacity>MAX_CAPACITY)? MAX_CAPACITY:_capacity;		
}
int ofxSlitScanGL::getCapacity() {
	return capacity;
}


void ofxSlitScanGL::addFrameTex(ofTexture& tex) {

	frameTexs.push_back(tex);
	// comprobar tamaño de frameTexs
	if(frameTexs.size()>capacity) {
		frameTexs.erase(frameTexs.begin(), frameTexs.end()-capacity);		
	}
	
}

// inserta un frame en una de los tercios de la secuencia de frames (push en ese tercio)
void ofxSlitScanGL::addFrameTexIn3(ofTexture& tex, int n3) {}
// inserta un frame en una de los cuartos de la secuencia de frames (push en ese cuarto)
void ofxSlitScanGL::addFrameTexIn4(ofTexture& tex, int n4) {}

// set un frame de la secuencia (no hace push)
// nFrame: 0,1,...
void ofxSlitScanGL::setFrameTexInFrame(ofTexture& tex, int nFrame) {
	if(nFrame<=frameTexs.size()) {
		frameTexs[nFrame] = tex;
	}
	else ofLogError("ofxSlitScanGL::setFrameTexInFrame.  nFrame es mayor que el numero de frames");
}


void ofxSlitScanGL::cleanFrames() {
	frameTexs.clear();
}


int ofxSlitScanGL::getWidth() {
	return width;
}

int ofxSlitScanGL::getHeight() {
	return height;	
}

int ofxSlitScanGL::getNumFrames() {
	return frameTexs.size();
}


void ofxSlitScanGL::setDelayMap(ofImage& img) {
	mapaActTex = img.getTextureReference();
}

void ofxSlitScanGL::setDelayMap(ofTexture& tex) {
	mapaActTex = tex;
}



void ofxSlitScanGL::update(){

	timer++;
	timer%=2;
	
    // MULTITEXTURE MIXING FBO
    //
	int nFrames = frameTexs.size();
	
//    fbo.begin();
    fboPingPong[timer].begin();
	
		ofClear(0, 0, 0,255);
		shader.begin();
			shader.setUniformTexture("texBase",	mapaActTex, 0);
	
			// frame Textures
			for(int i=0; i<nFrames; i++) {
				int j=i+1;
				string texref = "tex"+ofToString(j);
				shader.setUniformTexture(texref.c_str(), frameTexs[i] , j );
			}			
			// Variables
			shader.setUniform1i("nFrames", nFrames);
			
			// dibujamos un cuadro donde se queda la textura generada por el shader
			my_vbo.draw(GL_QUADS, 0, 4);
		
		shader.end();
	fboPingPong[timer].end();
//    fbo.end();
	
	// Ahora fbo ya tiene la textura deseada
}

ofTexture& ofxSlitScanGL::getOutputTex() {
//	return fbo.getTextureReference();
	return fboPingPong[timer].getTextureReference();
}

ofTexture& ofxSlitScanGL::getDelayMapTex() {
	return mapaActTex;
}

