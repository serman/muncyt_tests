#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofEnableSmoothing();
    
    tuioClient.start(3333);
    
    ofAddListener(tuioClient.cursorAdded,this,&testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved,this,&testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated,this,&testApp::tuioUpdated);

    o1.setup();

    touchElements.addObject(o1);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
}

//--------------------------------------------------------------
void testApp::update(){
    tuioClient.getMessage();
 /*
    jumpingRect.x=jumpingRect.x+ofNoise(jumpingRect.x)*4;
    if(jumpingRect.x>W_WIDTH) jumpingRect.x=0 ;
        
    jumpingRect.y=jumpingRect.y+ofNoise(jumpingRect.y)*3;
    if(jumpingRect.y>W_HEIGHT)jumpingRect.y=0;*/
    touchElements.update();
    //h1.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix();
    ofBackground(40,40,40);
    ofTranslate((ofGetWidth()-W_WIDTH)/2, 0);
    ofSetColor (0,0,0);
    ofRect(0,0,768,768);
    //empezamos aqui el codigo
        ofFill();
    ofSetColor (0);
    ofRect(80,120,300,10);
    
    ofSetColor (200,100,0);

   /* ofRect(slider1.x, slider1.y,slider1.width, slider1.height );

    ofRect(moveRect.x, moveRect.y,slider1.width, slider1.height);

    ofSetColor (jcolor);
    ofRect( jumpingRect.x, jumpingRect.y,jumpingRect.width, jumpingRect.height);
*/
   // tuioClient.drawCursors();
    
    touchElements.draw();
    //h1.draw();
    
    showDebug();
    ofPopMatrix();
}

void testApp::showDebug(){
    ofSetColor(255,255,255);
    ofDrawBitmapString("NUMERO DE CURSORES " + ofToString(tuioClient.cursorList.size()), 200, 200);
    
    ofDrawBitmapString("CUADRADO " + ofToString(o1.x) + " " +ofToString(o1.y), 200, 250);
    
    ofDrawBitmapString("puntos " + ofToString(puntoX ) +
                       " " +ofToString(puntoY), 200, 300);
    
}

void testApp::tuioAdded(ofxTuioCursor &tuioCursor){
    ofLog()<< "tuioAdded" << endl;
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	cout << "Point n" << tuioCursor.getSessionId() << " add at " << tuioCursor.getX() << " " << tuioCursor.getY()<< endl;

    
   // o1.touch(loc.x, loc.y,tuioCursor.getSessionId());
   // h1.touch(loc.x, loc.y,tuioCursor.getSessionId());

    handShadow *h1 = new handShadow();
    h1->setup();
    touchElements.addObject(*h1);
    
    touchElements.notifyTouch(loc.x, loc.y,tuioCursor.getSessionId());
}

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor){
        ofLog()<< "tuioUpdated" << endl;
    int mx = W_WIDTH*tuioCursor.getX();
    int my =    W_HEIGHT*tuioCursor.getY();
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    

    //o1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
   // h1.slide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    touchElements.notifySlide(loc.x, loc.y,tuioCursor.getSessionId(),tuioCursor.getMotionAccel());
    puntoX=tuioCursor.getX();
    puntoY=tuioCursor.getY();
    cout << tuioCursor.getX() << endl;
       
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor){
        ofLog()<< "tuioRemoved" << endl;
	ofPoint loc = ofPoint(tuioCursor.getX()*W_WIDTH,tuioCursor.getY()*W_HEIGHT);
    //cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
   /*
    if(id_slider1==tuioCursor.getSessionId()) id_slider1=NULL;
    if(id_jumpingRect==tuioCursor.getSessionId()) id_jumpingRect=NULL;
    if(id_moverect==tuioCursor.getSessionId()) id_moverect=NULL;*/

    
    touchElements.removeObjectByTuioID(tuioCursor.getSessionId() );
    
    touchElements.notifyTouchUp( tuioCursor.getSessionId() );

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key=='t')
    ofToggleFullscreen();
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}