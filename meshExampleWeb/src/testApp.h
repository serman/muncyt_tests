#pragma once

//
// Ejemplo del tutorial de ofMesh: 
// http://openframeworks.cc/tutorials/graphics/generativemesh.html
//

#include "ofMain.h"

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
	
	void setupGUI();
	
	ofMesh	mesh;
	ofImage	stars, stars_peq;
	bool swPeq;
	
	void	construyeMesh(ofImage img);
	float	intensityThreshold;
	float	connectionDistance;
	bool	bModifThreshold, bModifConnectionDistance;
	
	bool bDrawMesh, bDrawImg;
	
	vector<ofVec3f> offsets;
	
	
	ofEasyCam easyCam;
	
	
	// giro
	// We are going to use these to allow us to toggle orbiting on and off
	ofMesh meshCopy;
	bool orbiting;
	float startOrbitTime;
	
	// These variables will let us store the polar coordinates of each vertex
	vector<float> distances;
	vector<float> angles;
	ofVec3f meshCentroid;
	
	
	
	// Deformacion Barrell
	// Like with the orbiting tweak, this gives us a way to toggle on and off our magnifying effect
	bool mouseDisplacement;
	
	
	
	
	
	// TEST
	// Otro caso: 
	// Plano con puntos a los que se les aplica ruido
	
	// Se intercambia la escena con una tecla
	bool	bDrawPlano;
	
	// 
	// mesh plano 
	//
	ofMesh	planoMesh;
	int		lado;
	int		resol;
	void	setupPlanoMesh();
	void	updatePlanoMesh();
	
	
	
};
