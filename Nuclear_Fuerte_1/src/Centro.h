/*
 *  Centro.h
 *  Nuclear_Fuerte_1
 *
 *  Created by guillermo casado on 04/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 
 Centro sirve para:
 - detectar qué Emisores estan lanzando partículas 
 - obtener la direccion neta resultado del choque de los emisores
 
 - Interesante cuando la neta = 0 
 - Hay que tener en cuenta que la energia de cada rayo sea lo suficientemente alta (distancia al centro)
 
*/

#include "ParticleS.h"

class Centro {
public:
	ofVec2f pos;
	float radio;
	
	ofPolyline	circulo;
	ofPath		circuloPath;	// Para fill
	
	bool	bOcupado;
	ofVec2f dirNeta;	
	
	bool	bVariosEmitters;
	int		contadorPartics;
	vector	<int> idsEmitters;
	vector <ParticleS> particsInside;
	
	ofVec2f	pTot;	// momento total por frame = SUMA masa*veloc
	ofVec2f pMean; // momento total / contadorPartics
	float	energy;	// energia por frame = SUMA masa*veloc^2
	
	Centro() {
		dirNeta = ofVec2f(0,0);
		set(ofVec2f(0,0), 30);
		bOcupado = false;
	}
	
	void set(ofVec2f _pos, float r) {
		pos = _pos;
		radio = r;
		
		circulo.clear();
		circulo.arc(pos, radio, radio, 0, 360, true, 60);
		circulo.close();
		
		circuloPath.clear();
		circuloPath.arc(pos, radio, radio, 0, 360);
		circuloPath.close();
		
	}

	void reset() {
		pTot = ofVec2f(0,0);
		energy = 0.0;
		
		bVariosEmitters = false;
		
		contadorPartics = 0;		
		particsDentro.clear();
	}
	
	void addParticle(ParticleS _p) {
		// mete particulas dentro:
		//		suma momentos de las particulas
		//		y energía
		// analiza si vienen de varios emitters
		if(!bVariosEmitters) {
			// chequea solo si aun no se han detectado varios emisores
			for(int i=0; i<particsInside.size(); i++) {
				if(_p.idEmitter != particsInside[i].idEmitter) {
					bVariosEmitters = true;
					break;
				}
			}
		}
		particsInside.push_back(_p);
		
		pTot += _p.getMomentoP(); // m*_p.velocity;
		energy += _p.getEnergyK();
		
		contadorPartics++;
		
		pMean = pTot/contadorPartics;
		
	}
	
	
	void draw() {
		if(contadorPartics>0) bOcupado=true;
		
		ofPushStyle();
		ofEnableSmoothing();
		if(bOcupado) {
			float alpha = ofMap(contadorPartics, 0, 8, 35, 255);
			alpha = ofWrap(alpha,0,255);
			circuloPath.setFillColor(ofColor(255,224,23, alpha));
			circuloPath.setFilled(true);
//			ofSetColor(255,224,23, ofMap(contadorPartics, 0, 6, 35, 255));
			circuloPath.draw();
		}
		ofSetColor(200);
//		ofNoFill();
//		ofSetLineWidth(6);
//		circulo.draw();
		ofDisableSmoothing();
		ofPopStyle();
		
		bOcupado = false;
	}
	
};