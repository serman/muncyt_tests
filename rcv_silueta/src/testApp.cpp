#include "testApp.h"
#include "ofxMsgPacker.h"
#include "ofxMsgUnPacker.h"
#include "MsgPackMyClass.h"

bool bRecord = false;
bool bPlay = false;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(25);
    ofSetVerticalSync(true);
    rcv.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString("i : empieza thread; \n e: termina thread y conexion. \n s: si has pulsado e pulsa s para reabrir socket",100,100);
    if(rcv.isThreadRunning())
        rcv.getRemoteContour().draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
         case 's':
            rcv.setup();
            break;
        case 'i':
            rcv.start();
            break;
        case 'e':
            rcv.finish();
        default:
            break;
            
    }
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