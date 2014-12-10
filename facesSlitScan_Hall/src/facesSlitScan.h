#pragma once

#include "ofMain.h"
#include "ofxSlitScan.h"
#include "ofxImageSequence_Xtd.h"

#include "SlitScanGL.h"

#include "ofxTimer.h"

#include "ofxUI.h"

#define SCREEN_W 768
#define SCREEN_H 384

class facesSlitScan : public ofBaseApp{

	public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void gotMessage(ofMessage msg);

	// INPUTS
	// lector de v’deo y lector de c‡mara 
	// y de secuencias de imagenes
	ofVideoGrabber	vidGrabber;
	ofVideoPlayer	vidPlayer;

	// Zona vision Hall
	ofRectangle	rectHall;
	
	// Sequences
	ofxImageSequence_Xtd sequenceActiva;
	bool				 swSeqActiva;

	ofPixels			imgSeqPixels;	// *** para convertir de ofTexture a ofPixels y pasarlo al warp_SlitScan
	float fr;

	vector <string> imagesNames;
	void			leer_ImagesNames();
	void			cargarNImages(int nn);
	bool			swImagesLoaded;
	int				nImagesCarga;


	// - - - - - - - input selection - - - 
	bool useLiveVideo;
	void initLiveVideo();
	void initMovie();
	bool isFullScreen;
	bool isPaused;

	// - - - - - - - SLITSCAN - - - - - - - 
	ofxSlitScan		warp, warp1, warp2;
	int				capacity;
	void			setupSlitScan();
	void			setupMaps();
	void			changeCapacity();
	int				WIDTH_SS;
	int				HEIGHT_SS;

	// - - - - - - - SLITSCANGL - - - - - -
	// maxima capacidad en mi equipo: 12 texturas
	ofxSlitScanGL	scanner, scanner1, scanner2;
	bool			swmodoGL;

	bool	bWarpBlending;
	
	ofFbo		fboAux;
	
	// contenedor de mapas de SlitScan
	vector<string> sampleMapStrings;
	vector<ofImage*> sampleMaps;
	int currentSampleMapIndex;

	// GUI: imagenes de apoyo
	ofImage	colorImg;
	ofImage movieImg;
	ofImage	warpImage;	// ****
	ofImage previewImage;
	ofImage movieImage;

	// Formato imagen Final
	int wRender, hRender;

	// Fuente
	ofTrueTypeFont fontFutured;
	ofTrueTypeFont fontVerdana;
	ofTrueTypeFont fontVerdanaPeq;

	// Timer para cambiar autom‡ticamente de secuencia a reproducir
	ofxTimer	timer;
	void		tiempo(ofEventArgs &e);
	
	
    ofxUICanvas *gui; 
	void setupGUI();
    void exit(); 
    void guiEvent(ofxUIEventArgs &e);
	
};


