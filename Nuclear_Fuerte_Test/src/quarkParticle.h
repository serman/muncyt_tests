/*
 *  quarkParticle.h
 *  Nuclear_Fuerte_Test
 *
 *  Created by guillermo casado on 14/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

class QuarkParticle : public ofxBox2dPolygon {
	
public:
	b2World * world2d;
	
	ofColor color;
	int		tipo;	// 1: circle, 2: line, 3: triangle, 4: square
	bool	isSetup;
	float	masa, carga, spin;
	float	lado;
	int		qcolor;
	
	ofPolyline	poly;

	QuarkParticle() {
		masa = 1.0;
		carga = 0.0;
		spin = 1/2.0;
		qcolor = floor(ofRandom(3));
		
		lado = 1.0;
		
		color = ofColor(0x76E5FF);	// azulillo
		
		isSetup = false;
		tipo = -1;
	}
	
	
	void setupTipo(b2World * _world2d, int _tipo, float _lado) {

		tipo = _tipo;
		lado = _lado;
		
		int numLados = 0;
		ofLogNotice("quarkParticle->setupTipo   tipo " + ofToString(tipo));
		
		if(tipo==1) {
			// circulo
			numLados = 60;
			poly.arc(ofPoint(0,0),lado,lado,0,360);
			poly.close();
			ofLogNotice("quarkParticle->setupTipo tp1 fin");
			
		}
		else if(tipo==2) {
			// linea
			numLados = 2;
			poly.addVertex(-lado/2, -lado/2*0.2);
			poly.addVertex( lado/2, -lado/2*0.2);
			poly.addVertex( lado/2,  lado/2*0.2);
			poly.addVertex(-lado/2,  lado/2*0.2);
			poly.close();
			
		}
		else if(tipo==3) {
			// triangulo
			numLados = 3;
			ofVec2f vert(lado,0);
			for(int i=0; i<numLados; i++) {
				poly.addVertex(vert.x, vert.y);
				vert.rotate(120);
			}
			
//			poly.addVertex(vert.x, vert.y);
//			vert.rotate(120);
//			poly.addVertex(vert.x, vert.y);
//			vert.rotate(120);
//			poly.addVertex(vert.x, vert.y);
			poly.close();
			ofLogNotice("tipo 3.  poly: " + ofToString(poly.getVertices()));
			
		}
		else if(tipo==4) {
			// cuadrado
			numLados = 4;
			poly.addVertex(-lado/2, -lado/2);
			poly.addVertex( lado/2, -lado/2);
			poly.addVertex( lado/2,  lado/2);
			poly.addVertex(-lado/2,  lado/2);
			poly.close();
			ofLogNotice("tipo 4.  poly: " + ofToString(poly.getVertices()));
			
		}

		ofLogNotice("quarkParticle->setupTipo final");
		
		world2d = _world2d;
		
		isSetup = true;
		
	}
	
	void draw() {
		float radius = lado;
		
		ofPushMatrix();
		ofTranslate(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
		ofFill();
		ofCircle(0, 0, 4);	
		poly.draw();
		ofPopMatrix();
		
//		ofLogNotice("quark_Draw. Position " + ofToString(getPosition()));
	}
};
