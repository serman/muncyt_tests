#pragma once

#include "ofMain.h"
#include "ParticleS.h"
#include "Emisor.h"
#include "Centro.h"
#include "ParticleTipos.h"

#include "ofxXmlSettings.h"
#include "ofxUI.h"

#define W_WIDTH		768
#define W_HEIGHT	768


class nuclear_fuerte : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
	void init_Scena();
	void exit_Scena();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
//		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
	bool	bSetBack;
	float	alpha_Back;
	
	ofVec3f		zentro;
	float		radioEscena;
	ofPath		borde;
	ofPolyline	circExt;
	
	ofPath		anilloExterior;
	ofPath		anilloExteriorLineas;
	ofMesh		ringExt;
	
	// colores
	void setupColores();
	ofColor				coloresAll[6];
	ofColor				colores[3];
	ofColor				antiColores[3];	
	
	
	ofPolyline	camino;
	void		tiltCamino();
	bool		bTiltCamino;
	
	vector<ParticleS>	particulas, particulas_old;
	void				addParticleLateral();
	void				addParticleFromEmiter(Emisor &em);
	int					ratePartic;
	
	ofVboMesh			particMesh;
	
	vector<ofVec2f>		ptsChoque;
	vector<ofVec2f>		tangChoque;
	bool				swDifraccion;
	
	Emisor	emitter;
	vector <Emisor>		emitters;
	int		totEmitters;
	bool	clearEMitters;
	void	addEmisor(ofVec2f posTmp);
	
	Centro		centroLab;
	
	float	magnetField;
	bool	swMagnetField;
	
	
	bool	bDrawingMode;
	bool	bDrawCaminos;
	bool	bDrawPtosChoque;

	
	void	setupGUI();
	ofxUISuperCanvas *gui1;	
	void	guiEvent(ofxUIEventArgs &e);
	bool	bSave;
	
	ofImage	imgPunto;
	
	ParticleData newParticleData(int tipo);
	vector<ParticleS>	particulas_out, particulas_out_old;
	
    void exit();
	
	
};
