//
//  movingObj.h
//  example
//
//  Created by Sergio Galan on 7/8/14.
//
//
#include "consts.h"
#ifndef example_movingObj_h
#define example_movingObj_h


class  movingObj : public tangibleObject{
public:
    ofVec2f speed=ofVec2f(2, 2);
    long lastCollision=0;
    void setup(){
        cursor_id=-1;
        m_id=-1;
        mcolor=ofColor::pink;
        set(70,400,70,70);
    }
    
    void draw(){
        ofSetColor (mcolor);
        ofRect( x, y,width, height);
    };
    
    void update(){
        if(x>W_WIDTH|| x<0) speed.x*=-1;
        x+=speed.x;

        if(y>W_HEIGHT || y<0) speed.y*=-1;
        y+=speed.y;
    }
    
    void touch(float x, float y, int s_id){
        if(inside(x, y)){
            ofLog(OF_LOG_VERBOSE)<< "clicked";
            mcolor.set(ofRandom(200), ofRandom(200), ofRandom(200));
            colisionAction();
        }
        
    }
    
    void slide(float x, float y, int s_id, int acc){
        if(inside(x, y)){
            mcolor.set(200, 0, 0);
            colisionAction();
        }
        
    }
    void touchUp(int s_id){
        return;
    }
    
    void colisionAction(){
        if(ofGetElapsedTimeMillis()-lastCollision>400){
            speed.y*=-1;
            lastCollision=ofGetElapsedTimeMillis();
            ofLog(OF_LOG_VERBOSE)<< "Collision";
        }
    }
    
};



#endif
