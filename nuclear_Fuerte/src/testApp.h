#pragma once

#include "ofMain.h"
#include "ParticleX.h"
#include "Emisor.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	ofVec3f zentro;
	
	ofPolyline camino, camino1;
	
	vector<ParticleX>	particulas;
	void				addParticleLateral();
	
	ofVboMesh			particMesh;
	
	vector<ofVec2f>		ptsChoque;
	vector<ofVec2f>		tangChoque;
	bool				swDifraccion;
	
	Emisor	emitter;
	
	
	
	ofPath	testPath;
	
	
	void	setupGUI();
	
};
