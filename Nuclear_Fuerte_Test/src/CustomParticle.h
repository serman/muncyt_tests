/*
 *  CustomParticle.h
 *  Nuclear_Fuerte_Test
 *
 *  Created by guillermo casado on 15/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBox2d.h"


#ifndef CUSTOMPARTICLE
#define CUSTOMPARTICLE

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	CustomParticle() {
	}
	ofColor color;
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
		ofNoFill();
		ofCircle(0, 0, radius);	
		
		glPopMatrix();
		
	}

	void draw(ofTexture & _tex) {
		float radius = getRadius();
		float esc = 3.0;
		glPushMatrix();
//		glTranslatef(getPosition().x-_tex.getWidth()/2*esc , getPosition().y-_tex.getHeight()/2*esc , 0);
		glTranslatef(getPosition().x-radius*esc , getPosition().y-radius*esc , 0);
		
		ofSetColor(color.r, color.g, color.b);
//		_tex.draw(0,0, _tex.getWidth()*esc, _tex.getHeight()*esc);
		_tex.draw(0,0, 2*radius*esc, 2*radius*esc);
		
		glPopMatrix();
		
	}
	
	void ApplyForceToCenter(ofVec2f force) {
		if(body != NULL) {
			//body->ApplyLinearImpulse(b2Vec2(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE), b2Vec2(amt.x, amt.y), true);
			body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
		}
	}	
};

#endif