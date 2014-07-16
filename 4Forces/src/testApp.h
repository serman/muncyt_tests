#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


// -------------------------------------------------

class testApp : public ofBaseApp {
	
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

	void	draw4Forces();	
	bool	bDraw4Forces;
	void	drawBola4(ofVec2f pos, float radius, float rot=0);
	
	ofxBox2d                                box2d;			  //	the box2d world
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
	bool	bAddCircle, bAddBox;
	void	addCircle(ofPoint _pos);
	void	addBox(ofPoint _pos);
	
	ofVec2f ptoMedio(vector <ofPtr<ofxBox2dCircle> > shapes);
	ofVec2f ptoMedio(vector <ofPtr<ofxBox2dRect> > shapes);
	ofVec2f ptoMed_circles, ptoMed_boxes;
	
	bool	isKeyPressed;
	
	bool	isMousePressed;
	
	ofImage bola;
	float ladoPart1, ladoPart2;
	
	vector<ofColor> coloresBN;
	
	vector<ofPolyline>	sectores;
	void setupSectores();

	ofPath borde;

};

