#pragma once

#include "ofMain.h"
#include "ParticleS.h"
#include "Emisor.h"

#include "ofxXmlSettings.h"
#include "ofxUI.h"

#define W_WIDTH		768
#define W_HEIGHT	768

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
	
	ofPolyline	camino, camino1;
	void		tiltCamino();
	bool		bTiltCamino;
	
	vector<ParticleS>	particulas;
	void				addParticleLateral();
	int					ratePartic;
	
	ofVboMesh			particMesh;
	
	vector<ofVec2f>		ptsChoque;
	vector<ofVec2f>		tangChoque;
	bool				swDifraccion;
	
	Emisor	emitter;
	vector <Emisor>		emitters;
	
	float	magnetField;
	bool	swMagnetField;
	
	
	ofPath	testPath;
	
	bool	bDrawCaminos;
	bool	bDrawPtosChoque;

	
	void	setupGUI();
	ofxUISuperCanvas *gui1;	
	void	guiEvent(ofxUIEventArgs &e);
	
    void exit();
	
	
};
