/*
 *  Particle.h
 *  example-ptsMoviles
 *
 *  Created by guillermo casado on 09/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

//
//  ParticleS.h
//  PixelForces
//
//  Created by Ben McChesney on 7/2/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//
//  A Simple Particle Class
//
// evol: peripecio 2014
//
// Particle + mass, q (charge), spin)
//

#ifndef _PARTICLE
#define _PARTICLE

#define MAX_SPEED 30.f
#define MAX_STEER 0.9f

#define NPOS_MAX 10;

#include "ofMain.h"


class Particle
{
    public :
	
	int id;
	
	float m;	// mass
	float q;	// carga
    ofVec3f position , velocity ;
    ofVec3f position_prev;	
	vector<ofVec3f> positions;
    ofVec3f acceleration ;          //smoothing applied to velocity
    ofVec3f spawnPoint ;            //original location to line up the picture
    ofColor color, color_orig, colorExcited, colorTail ;
    char recentlyUsed;
	
	int	tipoPart;
	float tLife;
	
	float angle;
	
	// para chequear colision con ofPolyline
	bool inside;
	
	// esto crea una linea cuando este dentro de un polyline
	vector<ofPolyline> memoPaths;
	
	// esto acumula todos los puntos por los que pasa
	ofPolyline	lifePath;
	
	bool	bDrawLife;
	bool	bDrawMemoPath;
	
	float	longTail;
	
	
    Particle() ;
    
    Particle( ofVec3f _position, ofColor _color, float mass = 1.0, float charge = 0.0 )
    {
        position = _position ;
        color = _color ;
		color_orig = color;
		colorExcited = color;
		colorTail = ofColor(255);
		m = mass;
		q = charge;
		
		position_prev = position;
		
		positions.push_back(position);
		
		angle = 0;
		
        velocity = ofVec3f ( ofRandom ( -2 , 2 ) , ofRandom ( -2 , 2 ) , ofRandom ( -2 , -2 ) ) ;
        spawnPoint = _position ;
        recentlyUsed=0;
		
		tLife = 100.0;
		
		setTipoPart(floor(ofRandom(3)));
		
    };
	
    Particle( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
    {
        position = _position ;
        color = _color ;
		color_orig = color;
		colorExcited = color;
		m = mass;
		q = charge;
		
		position_prev = position;
		angle = 0;
		positions.push_back(position);
		
        velocity = _vel;
		
        spawnPoint = _position ;
        recentlyUsed=0;
		
		tLife = 100.0;
		
		setTipoPart(floor(ofRandom(3)));
		
    }
	
	void setTipoPart(int tp) {
		tipoPart = tp;
	}
	
    void update(){
		position_prev = position;
		
    	velocity += acceleration/m;
		
		// muy graciosete:
		//	https://sites.google.com/site/ofauckland/examples/curly-moving-particles		
		//		angle += ofSignedNoise(position.x, position.y)*PI;
		//		velocity.rotate(angle, ofVec3f(0,0,1));		
		
        position += velocity;
		
		
		// acumular puntos
		positions.push_back(position);
		if(positions.size() > longTail) {	
			positions.erase(positions.begin());
		}
		
		// todos los puntos por donde pasa (igual consume mucho)
		lifePath.addVertex(position);
		
		// puntos dentro de un polyline
		if(inside) {
			memoPaths[memoPaths.size()-1].addVertex(position);
		}
		
        acceleration=acceleration*0;
    }	
	
	
	// Los modos de dibujo variarán dependiendo del tipo	
	void draw() {
		ofPushStyle();
		
		ofSetColor(colorTail);
		ofNoFill();
		ofSetLineWidth(1);
		
		if(bDrawLife) {
			lifePath.draw();			
		}
		
		if(bDrawMemoPath) {
			drawMemoPath();
		}		
		
		if(!inside) {
			ofBeginShape();
			for( int i=0; i<positions.size(); i++)  {
				ofVertex(positions[i].x, positions[i].y);
			}
			ofEndShape(false);
			//ofLine(position_prev.x, position_prev.y, position.x, position.y);
		}	
		
		ofPushStyle();
		ofSetColor(color);
		//		ofLogNotice(ofToString(color));
		ofFill();
		if(tipoPart==0) {
			ofEllipse(position.x, position.y, 4,4);
		}
		else if(tipoPart==1) {
			ofPushMatrix();
			ofTranslate(position.x, position.y);
			ofRotate(ofGetElapsedTimeMillis());	// degrees
			ofSetLineWidth(1);
			ofLine(-4, 0, 4, 0);
			ofPopMatrix();
		}
		else {
			ofRect(position.x-3, position.y-3, 4,4);
		}
		ofPopStyle();
		
		if(inside) {
			ofPushStyle();
			ofNoFill();
			ofEllipse(position.x, position.y, 4,4);			
			ofPopStyle();
		}
		
		//		ofPopMatrix();
		ofPopStyle();
	}
	
	void drawMemoPath() {
		ofPushStyle();
		ofSetLineWidth(2);
		//		ofSetColor(colorExcited);
		ofColor ce = colorExcited;
		ce.a = 100.0;
		ofSetColor(ce	);
		for(int i=0; i<memoPaths.size(); i++) {
			memoPaths[i].draw();
		}
		ofPopStyle();
	}
	
	ofVec3f getMomentoP() {
		
		return m*velocity;
	}
	
	float getEnergyK() {
		return m*velocity.lengthSquared()/2.0;
	}
	
	
    
	bool setInside(bool bInside) {	// devuelve si ha cambiado de estado in/out u out/in
		bool bChange = false;
		bool inside_prev = inside;
		inside = bInside;
		if(inside) { 
			if(!inside_prev) {
				// nuevo camino
				ofPolyline memopl;
				memopl.addVertex(position);				
				memoPaths.push_back(memopl);
				
				bChange = true;
			}
		}
		else {
			if(inside_prev) {
				// sale de polyline
				// ofLogNotice("cierro");
				bChange = true;
			}
		}
		return bChange;
	}
	
	void insertPtChoque(ofVec2f ptChoque) {
		lifePath.insertVertex(ptChoque, lifePath.size()-1);
		
		int nn = memoPaths.size();
		if(memoPaths[nn-1].size()>0) {
			// si ya hay paths, creados
			// coger el ultimo y poner el punto en el penultimo punto
			int mm = memoPaths[nn-1].size();
			memoPaths[nn-1].insertVertex(ptChoque, mm-1);
		}
	}
	
    void applyForce(float a_x, float a_y, float a_z) {
        acceleration.x += a_x/m;
        acceleration.y += a_y/m;
        acceleration.z += a_z/m;
    };
    
    void applyForce(ofVec3f a_loc) {
        acceleration += a_loc/m;
    }
    
    void stopAcc() {
        acceleration.x = 0;
        acceleration.y = 0;
        acceleration.z = 0;
    };
    
    
	//ready made forces
    void steer(ofVec3f& a_target, bool a_slowdown, float a_scale, float a_minDist ) {
        ofVec3f steer;
        ofVec3f desired = a_target - position;
        float d = desired.length();
        
        // If the distance is greater than 0, calc steering (otherwise return zero vector)
        if (d > 0.f) {
            desired.normalize();
            if(a_minDist == 0) a_slowdown = false;
            // Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
            if ((a_slowdown) && (d < a_minDist)) {desired *= (MAX_SPEED * (d/a_minDist));} // This damping is somewhat arbitrary
            else {desired *= (MAX_SPEED); }
            
            steer = (desired - velocity);
            steer.limit(MAX_STEER);
            steer *= a_scale;
        } else {
            steer.set(0.f, 0.f, 0.f);
        }
        acceleration += steer;
    }
    
    void gravityTowards(ofVec3f& a_target,float a_minDist, float speed){
		ofVec3f diff =  a_target - position;
		int sqdistance =  diff.length();
		if(sqdistance>a_minDist){
			diff.normalize();
            int force=sqdistance/speed;
            diff *= force;
            applyForce( diff );
		}//espejo
        else{
			diff.normalize();
			int force=sqdistance/speed;
			diff = diff*-1;
			diff= diff*force;
			applyForce(diff);
        }
    }
    
    bool sandDown(int acc,int floor){
        ofVec3f dPoint = ofVec3f( floor,position.y,position.z);
        ofVec3f diff =  dPoint - position;
        if(position.x<floor){ //no point can be "below our floor
        	position.x=floor;
            return false;
        }        
        
        if(abs(diff.x)>1){
        	diff.normalize();
        	applyForce(diff*acc);
            return true;
        }else{
            velocity*=0;
            acceleration*=0;
        	return false;
        }
    }
    // Añade una fuerza en al direccion de las agujas del reloj (si str>0) respecto de un punto a_target
    void forceRadAround(ofVec3f& a_target, float fVal, float a_minDist){
        ofVec3f dir = position - a_target;
        float d = dir.lengthSquared();
        if (d < (a_minDist*a_minDist) && d > 0) {
			
			//			velocity.rotate(dirGiro*90.0, ofVec3f(0,0,1.0));
			
        }
    }
	
	void forceMagnetZ(float val_bMagnet){
		ofVec3f bMag = ofVec3f(0,0,val_bMagnet);
		forceMagnet(bMag);
	}
	
    void forceMagnet(ofVec3f& bMagnet){
		ofVec3f fMag = q*bMagnet.cross(velocity);
		applyForce(fMag);
		
	}
};
#endif