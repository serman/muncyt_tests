/*
 *  Nucleon.h
 *  Nuclear_Fuerte_Test
 *
 *  Created by guillermo casado on 15/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "testApp.h"
#include "CustomParticle.h"

#ifndef NUCLEON
#define	NUCLEON


class Nucleon {
public:
	b2World *       world2d;   //	the box2d world
	
	// Elementos de nucleon
	vector		<ofPtr<ofxBox2dCircle> >	circlesN;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dJoint> >		jointsN;			  //	joints
	vector		<ofPtr<ofxBox2dJoint> >		jointsAnchorN;
	ofxBox2dCircle							anchorN;	  //	fixed anchor: punto central
	vector		<ofPtr<CustomParticle> >    qParticlesN;  //	this is a custom particle the extends a cirlce
	ofVec2f					pos;
	float					masaQ;
	float					radioNucleon;
	float					radioQuark;
	float					fJoint;
	float					fJointR;
	float	freqQQ, dampQQ;
	float	freqQA, dampQA;

	
	ofColor				colores[3];
	ofColor				antiColores[3];
	
	ofTexture			tex;
	bool				hasTex;
	
	Nucleon() {}
	
	void crearNucleon(b2World * _world2d, ofVec2f _pos, float radioN) {
		world2d = _world2d;
		
		colores[0] = ofColor::red; 
		colores[1] = ofColor::green;
		colores[2] = ofColor::blue;
		antiColores[0] = colores[0]; antiColores[0].invert();
		antiColores[1] = colores[1]; antiColores[1].invert();
		antiColores[2] = colores[2]; antiColores[2].invert();
		
		
		radioNucleon = radioN;
		radioQuark = radioNucleon/5.0;
		
		masaQ = 3.0;
		
//		fJoint = 1.732; // sqrt(3)
//		fJointR = 0.45;
		fJoint = 1.732; // sqrt(3)
		fJointR = 0.25;
		
		freqQQ = 0.8;	// entre quarks
		dampQQ = 0.8;
		
		freqQA = 1.3;	// quark-Anchor
		dampQA = 0.5;
		
		float frictionQ = 3.6;
		
		
		//	pos = ofVec2f(0,0);
		pos = _pos;
		anchorN.setup(world2d, pos.x, pos.y, 4);
		
		float circ = TWO_PI*radioNucleon;
		
		ofVec2f v1 = ofVec2f(pos.x,pos.y);
		float lengthJointAnchor = radioNucleon*fJointR;
		float lengthJoint = lengthJointAnchor*fJoint;
		
		float radio = lengthJointAnchor;
		
		int nNodos = 3;
		ofVec2f vr = ofVec2f(radio,0);
		
		// first we add just a few circles
		for (int i=0; i<nNodos; i++) {
			ofVec2f vi = vr.getRotated(360.0/nNodos*float(i));
			vi+=v1;
			//		ofLogNotice("nodo "+ofToString(i)+": "+ofToString(vi));
			
			qParticlesN.push_back(ofPtr<CustomParticle>(new CustomParticle));
			CustomParticle * p = qParticlesN.back().get();
			p->setPhysics(masaQ, 0.53, frictionQ);
			p->setup(world2d, vi.x, vi.y, radioQuark);
			p->color = colores[i%3];
			p->setDamping(0.1);
			
		}
		
		// now connect each circle with a joint
		for (int i=0; i<qParticlesN.size(); i++) {
			
			int iprev = (i==0)? qParticlesN.size()-1 : i-1;
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
			joint.get()->setup(world2d, qParticlesN[iprev].get()->body, qParticlesN[i].get()->body, 
							   freqQQ, dampQQ);// float frequencyHz=4.f, float damping=.5f
			joint.get()->setLength(lengthJoint);
			jointsN.push_back(joint);
		}
		
		// conexion al anchor
		for (int i=0; i<qParticlesN.size(); i++) {
			ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
			//			joint.get()->setup(world2d, anchor.body, circles[i].get()->body);
			joint.get()->setup(world2d, anchorN.body, qParticlesN[i].get()->body, 
							   freqQA, dampQA);// float frequencyHz=4.f, float damping=.5f
			joint.get()->setLength(lengthJointAnchor);
			jointsAnchorN.push_back(joint);
		}
		
		hasTex = false;
	}
	
	
	
	void setTex(ofTexture _tex) {
		tex = _tex;
		hasTex = true;
	}
	
	void setPosition(float xx, float yy) {
		ofVec2f difPos = ofVec2f(xx,yy)-pos;
		
		// transportar el centro y los gluons
		anchorN.setPosition(xx,yy);
		pos.x = xx;
		pos.y = yy;
		
		// transportar los quarks una distancia difPos
		for(int i=0; i<qParticlesN.size(); i++) {
			ofVec2f posAct = qParticlesN[i].get()->getPosition();
			qParticlesN[i].get()->setPosition(posAct+difPos);
		}
		
		
		
		
	}
	
	
	void tiltNucleon() {
		
		for (int i=0; i<qParticlesN.size(); i++) {
			ofVec2f amt = ofVec2f(ofRandom(1.0), 0);
			amt.rotateRad(ofRandom(TWO_PI));
			amt*=8.0;
			//		ofLogNotice(ofToString(amt));
			//		qParticlesN[i].get()->addImpulseForce(ofVec2f(0,0), amt);
			qParticlesN[i].get()->addForce(amt, 30.0);	
			
			// add un damping para frenar en la direccion de la velocidad
			ofVec2f vel = qParticlesN[i].get()->getVelocity();
			vel.normalize();
//			vel*=-0.05;
			vel*=-1;
			qParticlesN[i].get()->addForce(vel, 1);
		}
		
		// frenar en seco el movimiento giatorio de las particulas cada cierto tiempo rnd
		float rnd = ofRandom(1);
		if(rnd<0.01) {
			for (int i=0; i<qParticlesN.size(); i++) {
				qParticlesN[i].get()->setVelocity(ofVec2f(0,0));
			}
		}
	}
	
	
	void drawNucleon() {
		
		ofPushStyle();
		
		//	anchorN.draw();
		

		// draw bolillos
		// HAY QUE DIRIGIRLOS
		// VARIABLE QUE VA CON TIEMPO.
		// DIBUJAR EL BOLILLO A UNA DISTANCIA PORCENTUAL QUE OSCILA ENTRE 0 Y 1
		// PODRIA DEJAR HUELLAS
		//
		int nBolos = 9 + floor(ofRandom(16));
		for(int i=0; i<nBolos; i++) {
			float rr = radioNucleon*ofNoise(12.3*i+ofGetElapsedTimef()+pos.x);
			float aa = 3*TWO_PI*ofNoise(7.4*i+ofGetElapsedTimef()*0.4+pos.y);
			ofColor cc = antiColores[i%3];
			ofSetColor(cc);

			ofPushMatrix();
			ofTranslate(pos.x, pos.y, 0);
			ofNoFill();
			ofCircle(rr*cos(aa), rr*sin(aa), radioQuark/5.0);
//			if(!hasTex) {
//				ofFill();
//				ofCircle(rr*cos(aa), rr*sin(aa), radioQuark/5.0);
//			}
//			else {
//				ofPushMatrix();
//				ofTranslate(rr*cos(aa), rr*sin(aa), 0);
//				float esc = 5.0;
//				tex.draw(-tex.getWidth()/2/esc, -tex.getHeight()/2/esc, tex.getWidth()/esc, tex.getHeight()/esc);
//				ofPopMatrix();
//				
//			}
			ofPopMatrix();
			
		}

		
		for(int i=0; i<qParticlesN.size(); i++) {
			ofFill();
			ofSetHexColor(0x01b1f2);
			if(!hasTex) {
				qParticlesN[i].get()->draw();
			}
			else {
//				qParticlesN[i].get()->draw(tex);
				ofVec2f ppos = qParticlesN[i].get()->getPosition();
				ofPushMatrix();
				ofTranslate(ppos.x, ppos.y, 0);
				float esc = 1/1.05;
				ofSetColor(qParticlesN[i].get()->color);
				tex.draw(-tex.getWidth()/2/esc, -tex.getHeight()/2/esc, tex.getWidth()/esc, tex.getHeight()/esc);
				ofPopMatrix();
			}
			
			
		}
		
		//		for(int i=0; i<jointsN.size(); i++) {
		//			ofSetHexColor(0x444342);
		//			jointsN[i].get()->draw();
		//		}
		
		
		// frontera nucleón
		ofEnableSmoothing();

		ofNoFill();
		ofSetHexColor(0xFFE416);
		ofSetColor(255,243,37, 140);
		ofSetCircleResolution(60);
		ofSetLineWidth(0.5);
		ofCircle(pos.x, pos.y, radioNucleon);
		ofDisableSmoothing();

		ofPopStyle();
	}
	
	


};





class NucleonPart : public ofxBox2dCircle{
public:
	
	NucleonPart() {}

	ofColor color;
	float rotRad;
	float radius;
	
	void draw() {
		radius = getRadius();
		
		ofPushMatrix();
		ofTranslate(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
		ofNoFill();
		ofCircle(0, 0, radius);	
		
		ofPopMatrix();
		
	}
	
	void draw(ofTexture & _tex) {
		radius = getRadius();
		float esc = 1.3;
		ofPushMatrix();
		//		glTranslatef(getPosition().x-_tex.getWidth()/2*esc , getPosition().y-_tex.getHeight()/2*esc , 0);
		ofTranslate(getPosition().x-radius*esc , getPosition().y-radius*esc , 0);
		
		ofSetColor(color.r, color.g, color.b);

		_tex.draw(0,0, 2*radius*esc, 2*radius*esc);
		
		ofPopMatrix();
		
	}
	
	void ApplyForceToCenter(ofVec2f force) {
		if(body != NULL) {
			//body->ApplyLinearImpulse(b2Vec2(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE), b2Vec2(amt.x, amt.y), true);
			body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
		}
	}	
	
	void tiltNucleon() {
		
		ofVec2f amt = ofVec2f(ofRandom(1.0), 0);
		amt.rotateRad(ofRandom(TWO_PI));
		amt*=3.0;
		ApplyForceToCenter(amt);	
		
	}
	
	
	
};




#endif
