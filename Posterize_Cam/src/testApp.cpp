#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	cout << "Opencv version " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << endl;

	image.loadImage("lenna.png");

	Posterizer p;
//	poster = p.posterize(image, 8);
//	cout << "testApp::setup got posterized" << endl;
	
	
	cam.initGrabber(640/3, 480/3);
	
}

//--------------------------------------------------------------
void testApp::update(){
	cam.update();
	if(cam.isFrameNew()) {
		image.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
		Posterizer p;

		poster = p.posterize(image, 8);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
//	image.draw(0,0);
//	poster.draw(image.width, 0);
	
	image.draw(0,0);
	poster.draw(image.width, 0);
	poster.draw(640, 0,640,480);
	ofSetColor(255);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10,10);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
