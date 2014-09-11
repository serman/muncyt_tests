#pragma once
#include "ofMain.h"
//#include "ofxBox2d.h"
#include "ofxUI.h"

// -------------------------------------------------

class fGravedad : public ofBaseApp {
	
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
	void resized(int w, int h);
	void exit();
	
	void	setupBorde();

	bool	isKeyPressed;
	
	bool	isMousePressed;
	
	ofImage bola;
	float ladoPart1, ladoPart2;
	
	vector<ofColor> coloresBN;
	
	ofPath borde;

	ofMesh	espacio, espacio_act;
	ofVbo	espacioVBO;
	void	setupMesh(float rrMin, float rrMax, int nnRads, int nnAngs);

	void	updateMesh();
	
	void	setNormals( ofMesh &mesh );
	void	drawNormals( ofMesh &mesh );
	
	bool	bDrawWF, bDrawMeshLines;
	
	float	rMin, rMax;
	int		nRads, nAngs;

	float drr;			// intervalo radial del mesh
	float dAngRads;		// intervalo Angular del mesh
	float dAngRadsDeg;
	
	bool	bColorMesh;
	float	alphaMesh;

	ofVec2f centro;
	void	interact(ofVec2f pt);
	
	vector<int> idsPtosInteract;
	
	
	// Por si quiero iutilizar Box2d
	//	ofxBox2d                                box2d;	
	//	vector		<ofPtr<ofxBox2dCircle> >	circles;
	//	bool		bAddCircle;
	//	void		addCircle(ofPoint _pos);
	//	ofVec2f ptoMedio(vector <ofPtr<ofxBox2dCircle> > shapes);
	//	ofVec2f ptoMed_circles;

	
	bool	bLights;
	ofLight	luz;
	
	// UI 
	
	ofxUISuperCanvas *gui;
	void setupGUI();
	void guiEvent(ofxUIEventArgs &e);
	
	
	ofEasyCam	cam;
	
};

