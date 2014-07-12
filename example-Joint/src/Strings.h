/*
 *  Strings.h
 *  example-Joint
 *
 *  Created by guillermo casado on 09/07/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBox2d.h"

#ifndef STRINGS
#define STRINGS

// String lineal
class StringL {
public:
	b2World *       world2d;			  //	the box2d world
	ofxBox2dCircle  anchor1, anchor2;	  //	fixed anchor
	int nNodos;
	
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dJoint> >		joints;			  //	joints
	
	// constructor
	//	StringL();
	//	~StringL();
	
	// setup
	void setup(b2World * _world2d, ofPoint p1, ofPoint p2, int _nNodos, float fJoint = 0.5) {
		world2d = _world2d;
		
		anchor1.setup(world2d, p1.x, p1.y, 4);
		anchor2.setup(world2d, p2.x, p2.y, 4);
		
		nNodos = _nNodos;
		
		ofVec2f v1 = ofVec2f(p1.x,p1.y);
		ofVec2f v2 = ofVec2f(p2.x,p2.y);
		
		float d12 = v1.distance(v2);
		float lengthJoint = d12/(nNodos+1)*fJoint;
		ofLogNotice("d12: " + ofToString(d12) + " -   lengthJoint: " + ofToString(lengthJoint));
		ofLogNotice("v1: " + ofToString(v1) + " -   v2: " + ofToString(v2));
		
		// first we add just a few circles
		for (int i=0; i<nNodos; i++) {
			float ff = float(i)/(nNodos+1.0);
			ofVec2f vi = v1.getInterpolated(v2, ff );
			ofLogNotice("nodo "+ofToString(i)+": "+ofToString(vi)+"   "+ofToString(ff));
			ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
			circle.get()->setPhysics(3.0, 0.53, 0.1);
			circle.get()->setup(world2d, vi.x, vi.y, 16);
			circles.push_back(circle);
		}
		
		// now connect each circle with a joint
		for (int i=0; i<circles.size(); i++) {
			
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
			
			// if this is the first point connect to the top anchor.
			if(i == 0) {
				joint.get()->setup(world2d, anchor1.body, circles[i].get()->body);
			}
			else {
				// unir el circ actual (i) con el anterior (i-1)
				joint.get()->setup(world2d, circles[i-1].get()->body, circles[i].get()->body);
			}
			
			joint.get()->setLength(lengthJoint);
			joints.push_back(joint);
		}
		
		// ultima unión
		ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
		int j=circles.size()-1;
		// punto final se conecta al ultimo anchor
		joint.get()->setup(world2d, circles[j].get()->body, anchor2.body);
		joint.get()->setLength(lengthJoint);
		joints.push_back(joint);
		
	}
	
	// draw
	void draw() {
		ofSetHexColor(0xf2ab01);
		anchor1.draw();
		anchor2.draw();
		
		for(int i=0; i<circles.size(); i++) {
			ofFill();
			ofSetHexColor(0x01b1f2);
			circles[i].get()->draw();
		}
		
		for(int i=0; i<joints.size(); i++) {
			ofSetHexColor(0x444342);
			joints[i].get()->draw();
		}
	}
	
	
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// String circular
class StringC {
public:
	b2World *       world2d;   //	the box2d world
	ofxBox2dCircle  anchor;	  //	fixed anchor: punto central
	int nNodos;
	float radio;
	
	vector		<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dJoint> >		joints;			  //	joints
	vector		<ofPtr<ofxBox2dJoint> >		jointsAnchor;			  //	joints
	
	// constructor
	//	StringC();
	//	~StringC();
	
	// setup
	void setup(b2World * _world2d, ofPoint p1, float _r, int _nNodos, float fJoint = 0.5, float fJointR = 0.5) {
		world2d = _world2d;
		
//		anchor.setPhysics(150.0, 0.83, 0.9);
		anchor.setup(world2d, p1.x, p1.y, 4);
		radio = _r;
		
		nNodos = _nNodos;
		
		float circ = TWO_PI*radio;
		
		ofVec2f v1 = ofVec2f(p1.x,p1.y);
		float lengthJoint = circ/nNodos*fJoint;
		float lengthJointAnchor = radio*fJointR;
		
		ofVec2f vr = ofVec2f(radio,0);
		// first we add just a few circles
		for (int i=0; i<nNodos; i++) {
			ofVec2f vi = vr.getRotated(360.0/nNodos*float(i));
			vi+=v1;
			ofLogNotice("nodo "+ofToString(i)+": "+ofToString(vi));
			ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
			circle.get()->setPhysics(3.0, 0.53, 0.1);
			circle.get()->setup(world2d, vi.x, vi.y, radio/15);
			circles.push_back(circle);
		}
		
		// now connect each circle with a joint
		for (int i=0; i<circles.size(); i++) {
			
			int iprev = (i==0)? circles.size()-1 : i-1;
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
//			joint.get()->setup(world2d, circles[iprev].get()->body, circles[i].get()->body);
			joint.get()->setup(world2d, circles[iprev].get()->body, circles[i].get()->body, 
							   4.0, .2);// float frequencyHz=4.f, float damping=.5f
			joint.get()->setLength(lengthJoint);
			joints.push_back(joint);
		}
		
		// conexion al anchor
		for (int i=0; i<circles.size(); i++) {
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
//			joint.get()->setup(world2d, anchor.body, circles[i].get()->body);
			joint.get()->setup(world2d, anchor.body, circles[i].get()->body, 
								4.0, .01);// float frequencyHz=4.f, float damping=.5f
			joint.get()->setLength(lengthJointAnchor);
			jointsAnchor.push_back(joint);
		}
	}
	
	// draw
	void draw() {
		ofSetHexColor(0xf2ab01);
		anchor.draw();
		
		for(int i=0; i<circles.size(); i++) {
			ofFill();
			ofSetHexColor(0x01b1f2);
			circles[i].get()->draw();
		}
		
		for(int i=0; i<joints.size(); i++) {
			ofSetHexColor(0x444342);
			joints[i].get()->draw();
		}
	}
	
	
};


class Ring {
public:	
	ofMesh aro;
	ofPoint pos;
	float rot, wrot;
	int nPts;
	float radio;
	
	vector<ofVec3f> offsets;
	
	ofColor color;
	
	//Ring();
	
	void setup(ofPoint _pos, float r){
		rot = 0;
		wrot = 0;
		pos = _pos;
		radio = r;
		nPts = 18;
//		aro.setMode(OF_PRIMITIVE_LINE_LOOP);
		aro.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

		for(int i=0;i<2*nPts;i++) {
			float ang = TWO_PI/(2*nPts)*i;
			float rr = (i%2==0)? radio : 1.2*radio;
			aro.addVertex(ofVec3f(rr*cos(ang), rr*sin(ang), 0.0));
			
			// offsets para el noise
			offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));	
		}
		// cerrar el anillo
		aro.addVertex(aro.getVertex(0));
		aro.addVertex(aro.getVertex(1));
		
		color = ofColor(255);
		
	}

	void update() {
		for(int i=0;i<2*nPts;i++) {
			ofVec3f vert = aro.getVertex(i);
			
			float time = ofGetElapsedTimef();
			float timeScale = 5.0;
			float displacementScale = 1.4;
			ofVec3f timeOffsets = offsets[i];
			
			// A typical design pattern for using Perlin noise uses a couple parameters:
			// ofSignedNoise(time*timeScale+timeOffset)*displacementScale
			//     ofSignedNoise(time) gives us noise values that change smoothly over time
			//     ofSignedNoise(time*timeScale) allows us to control the smoothness of our noise (smaller timeScale, smoother values)
			//     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise function to control multiple things and have them look as if they are moving independently
			//     ofSignedNoise(time)*displacementScale allows us to change the bounds of the noise from [-1, 1] to whatever we want 
			// Combine all of those parameters together, and you've got some nice control over your noise
			
			vert.x += (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
			vert.y += (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
			vert.z += (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
			
			aro.setVertex(i, vert);
		}
		// cerrar el anillo
		aro.setVertex(2*nPts,   aro.getVertex(0));
		aro.setVertex(2*nPts+1, aro.getVertex(1));
		
		rot+=wrot;
	}
	
	void draw() {
		
		ofPushMatrix();
		ofPushStyle();
		ofSetLineWidth(5);
		ofTranslate(pos.x,pos.y);
		ofRotate(rot);
		ofSetColor(color);
		aro.draw();
		
		ofPopStyle();
		ofPopMatrix();
		
	}
	
	
};


#endif