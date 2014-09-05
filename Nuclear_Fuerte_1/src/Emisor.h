/*
 *  Emisor.h
 *  nuclear_Fuerte
 *
 *  Created by guillermo casado on 03/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EMISOR
#define _EMISOR

#include "testApp.h"


struct ParticleData {
	float m;
	float q;
    ofVec3f position;
    ofVec3f velocity;
    ofColor color;
	int		tpPartic;
};

class Emisor {

public:
	// posicion
	float rho;	// controlara la energia de las particulas emitidas
	float ang;
	ofVec2f posXY;
	
	// hacia donde van las particulas
	ofVec2f pointTo;
	ofVec2f dir;
	
	// Tipo Particula
	int		tpPartic;
	
	ParticleData	pData;
	
	// para dibujar
	float		lado;
	ofVboMesh	icono;
	ofColor		c;

	Emisor() {
		setPos_Radial(0,0);
		
		tpPartic = 0;

		c = ofColor::red;

		// preparar forma a dibujar
		setupIcono();
	}
	
	Emisor(float _rho, float _ang, int _tpPartic = 0) {
		
		setPos_Radial(_rho, _ang);
		
		tpPartic = _tpPartic;

		c = ofColor::red;
		
		// preparar forma a dibujar
		setupIcono();
		
	}
	
	void setColor(ofColor cc) {
		c = cc;
		setupIcono();
	}
	
	void setupIcono() {
		// También puede ser un png!
		
		lado = 30.0;
		float altura = lado*sin(PI/3.0);
		icono.clear();
		icono.addVertex(ofVec3f(-lado/2.0, -0.2*altura, 0.0));
		icono.addColor(ofColor::fromHsb(c.getHue(), c.getSaturation(), c.getBrightness()*0.2));
		icono.addVertex(ofVec3f(+lado/2, -0.2*altura, 0.0));
		icono.addColor(ofColor::fromHsb(c.getHue(), c.getSaturation(), c.getBrightness()*0.2));
		icono.addVertex(ofVec3f(0,0.8*altura,0.0));
		icono.addColor(c);
	}
	
	void setPos_Radial(float _rho, float _ang) {
		rho = _rho;
		ang = _ang;	
		calcPosXY();
	}
	void setPos_XY(float _x, float _y) {
		posXY = ofVec2f(_x, _y);
		
		rho = posXY.length();
		ang = atan2(posXY.y, posXY.x);
//		ofLogNotice(ofToString(ang));
	}
	
	ofVec2f calcPosXY() {
		posXY = ofVec2f(rho*cos(ang), rho*sin(ang));
		return posXY;
	}	
	
	void draw() {

		ofPushStyle();
		// linea de union
		ofSetColor(c);
		ofLine(0,0, posXY.x, posXY.y);
		
		// dibuja el icono
		ofPushMatrix();
		// translate
		ofTranslate(posXY.x, posXY.y);
		// rotate
		ofRotateZ(90+RAD_TO_DEG*(ang));
		
		// draw
		icono.draw();
		ofPopMatrix();
		ofPopStyle();
		
	}
	
	
	// Da valores de particulas que emite
	ParticleData getParticleData() {
		return pData;
	}
	
	
	
};

#endif
