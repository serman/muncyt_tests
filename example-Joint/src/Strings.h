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
	void setup(b2World * _world2d, ofPoint p1, float _r, int _nNodos, float fJoint = 0.5) {
		world2d = _world2d;
		
//		anchor.setPhysics(150.0, 0.83, 0.9);
		anchor.setup(world2d, p1.x, p1.y, 4);
		radio = _r;
		
		nNodos = _nNodos;
		
		float circ = TWO_PI*radio;
		
		ofVec2f v1 = ofVec2f(p1.x,p1.y);
		float lengthJoint = circ/nNodos*fJoint;
		float lengthJointAnchor = radio*0.6;
		
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
			joint.get()->setup(world2d, circles[iprev].get()->body, circles[i].get()->body);
			joint.get()->setLength(lengthJoint);
			joints.push_back(joint);
		}
		
		// conexion al anchor
		for (int i=0; i<circles.size(); i++) {
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
			joint.get()->setup(world2d, anchor.body, circles[i].get()->body);
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


#endif