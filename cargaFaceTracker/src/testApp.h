#pragma once

#include "ofMain.h"
#include "Cara.h"

#include "ofxFaceTrackerThreaded.h"

#define TMUESTRACARA 1000;


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	
		void cargaCaras();
	
	bool	swEnLinea;
	bool	swWireframe;
	bool	swTexture;
	
	enum modos {ENLINEA, CIRCULO, FT2D};
	int		modo;

	enum modosDraw {WIREFRAME, HILO, FACETS};
	
		Cara	cara;

		vector<Cara> caras;
	
		ofxFaceTrackerThreaded ftracker;
		
		int tLastCara;
		int tMuestraCara;
		int nCaraAct;
};
