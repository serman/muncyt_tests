#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxBox2d.h"

#include "CustomParticle.h"
#include "Nucleon.h"
#include "quarkParticle.h"


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

	ofVec2f	zentro;
	
	ofFbo			nucleonFbo;
	
	ofxBox2d		box2d;
	void			setupBox2d();

	ofxBox2d		box2dX;
	void			setupBox2dX();
	void			setupNucleonesSystem();
	
	int nNucleones;
	vector <ofPtr <Nucleon> >	nucleones;
	vector <ofPtr <NucleonPart> >	nucleonesP;
	vector <ofPtr <ofxBox2dJoint> >	jointsNucleonesP;
	ofxBox2dCircle			anchorZ;
	
	// variables del nucleon
	ofVec2f					pos;
	float					masaQ;
	float					radioNucleon;
	float					radioQuark;
	float					fJoint;
	float					fJointR;
	float					freqQQ, dampQQ;
	float					freqQA, dampQA;
	
	// colores
	ofColor				colores[3];
	ofColor				antiColores[3];
	
	// particulas lanzadas
	ofxBox2d		box2dQ;
	vector <ofPtr <QuarkParticle> >	quarksP;
	void	setupBox2dQ();
	void	crearQuark(float x, float y);
	void	updateQuarks();
	void	drawQuarks();
				
	//        ofPtr<ofxBox2dPolygon> poly = ofPtr<QuarkParticle>(new ofxBox2dPolygon);
	//        poly.get()->addVertices(shape.getVertices());
	//        poly.get()->setPhysics(1.0, 0.3, 0.3);
	//        poly.get()->create(box2d.getWorld());
	//        polyShapes.push_back(poly);
	
	
	
	
	
	
	ofxUISuperCanvas *gui;	
	void	setupGUI();
	
	ofImage	imgPunto;
};
