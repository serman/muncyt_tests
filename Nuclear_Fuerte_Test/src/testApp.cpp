#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	zentro = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);

//	colores[3] = {ofColor::red , ofColor::green , ofColor::blue};

	colores[0] = ofColor::red; 
	colores[1] = ofColor::green;
	colores[2] = ofColor::blue;
	antiColores[0] = colores[0]; antiColores[0].invert();
	antiColores[1] = colores[1]; antiColores[1].invert();
	antiColores[2] = colores[2]; antiColores[2].invert();
	
	imgPunto.loadImage("images/dot.png");
	
	// allocate fbo
	nucleonFbo.allocate(300,300,GL_RGBA);

	setupBox2d();
	setupBox2dX();
	
	setupNucleonesSystem();
	
	
	setupBox2dQ();
	
	setupGUI();
	
}

void testApp::setupBox2d() {
	box2d.init();
	box2d.setGravity(0, 0);
	//	box2d.createGround();
	box2d.setFPS(40.0);
	box2d.registerGrabbing();
}

void testApp::setupBox2dX() {
	box2dX.init();
	box2dX.setGravity(0, 0);
//	box2dX.createGround();
	box2dX.setFPS(30.0);
	box2dX.registerGrabbing();
	
	// Acotar
	float lado = ofGetHeight();
	box2dX.createBounds(ofRectangle(ofGetWidth()/2-lado/2, 0, lado, lado));
	
	
}

void testApp::setupBox2dQ() {
	box2dQ.init();
	box2dQ.setGravity(0, 0);
	//	box2dX.createGround();
	box2dQ.setFPS(60.0);
//	box2dQ.registerGrabbing();
	
	// Acotar
	float lado = ofGetHeight();
	box2dQ.createBounds(ofRectangle(ofGetWidth()/2-lado/2, 0, lado, lado));
	
}


void testApp::setupNucleonesSystem() {
	
	
	// crear Nucleones
	
	nNucleones = 20;
	for(int i=0; i<nNucleones; i++) {
		

		nucleones.push_back(ofPtr<Nucleon>(new Nucleon));
		Nucleon * nTmp = nucleones.back().get();
		//		Nucleon nTmp;
//		float radTmp = 50+ofRandom(50);
		float radTmp = 70;
		ofVec2f posTmp = ofVec2f(ofRandom(ofGetHeight()/2-radTmp),0);
		posTmp.rotate(ofRandom(360));
		ofLogNotice("pos "+ofToString(i)+": "+ofToString(posTmp));
		nTmp->crearNucleon(box2d.getWorld(), zentro+posTmp, radTmp);
		nTmp->setTex(imgPunto.getTextureReference());

	}
	
	
	// Ahora a crear el sistema de Nucleones interactuantes
	anchorZ.setup(box2dX.getWorld(), zentro.x, zentro.y, 2);
	for(int i=0; i<nNucleones; i++) {
				
		nucleonesP.push_back(ofPtr<NucleonPart>(new NucleonPart));
		
		NucleonPart * nPTmp = nucleonesP.back().get();
		Nucleon * nuclTmp = nucleones.back().get();
		
		nPTmp->setPhysics(3, 0.53, 0.5);
		nPTmp->setup(box2dX.getWorld(), nuclTmp->pos.x, nuclTmp->pos.y, nuclTmp->radioNucleon*0.8);
		nPTmp->color = colores[i%3];
		nPTmp->setDamping(0.1);		
		
		// Unirlos a un anchor central.
		// conexion al anchor
		ofPtr<ofxBox2dJoint> joint = ofPtr<ofxBox2dJoint>(new ofxBox2dJoint);
		//			joint.get()->setup(world2d, anchor.body, circles[i].get()->body);
		joint.get()->setup(box2dX.getWorld(), anchorZ.body, nPTmp->body, 0.4f, .9f);
//						   freqQA, dampQA);// float frequencyHz=4.f, float damping=.5f
		joint.get()->setLength(100);
		jointsNucleonesP.push_back(joint);
		
		
	}

}

// quarks
void testApp::crearQuark(float x, float y) {

	ofLogNotice("Crear Quark");
	ofPtr <QuarkParticle> qP = ofPtr <QuarkParticle>(new QuarkParticle);

	int tipo = 1+quarksP.size()%4;
	float lado = 30.0;
	qP.get()->setupTipo(box2dQ.getWorld(), tipo, lado);
	
	ofLogNotice("Previo a AddVertexes. num vertx: " + ofToString(qP.get()->poly.getVertices().size()));
	qP.get()->addVertexes(qP.get()->poly);
	ofLogNotice("POST a AddVertexes.");
	qP.get()->setPhysics(1.0, 0.3, 0.3);
	qP.get()->create(box2dQ.getWorld());
	ofLogNotice("POST CREATE.");
	qP.get()->setPosition(x,y);
	
	quarksP.push_back(qP);
	
}

void testApp::updateQuarks() {

	for(int i=0; i<quarksP.size(); i++) {
		quarksP[i].get()->update();
	}
	
	box2dQ.update();
}

void testApp::drawQuarks() {
	
	for(int i=0; i<quarksP.size(); i++) {
		quarksP[i].get()->draw();
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
	

	for(int i=0; i<nucleonesP.size(); i++) {
		nucleonesP[i].get()->tiltNucleon();
	}
	for(int i=0; i<nucleones.size(); i++) {
		nucleones[i].get()->tiltNucleon();
		ofVec2f newPos = nucleonesP[i].get()->getPosition();
		nucleones[i].get()->setPosition(newPos.x, newPos.y);
		//		ofLogNotice(ofToString(i));
		//		ofLogNotice("pos "+ofToString(i)+": "+ofToString(nucleones.back().get()->pos));
	}
	
	box2d.update();
	
	box2dX.update();
	
	updateQuarks();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(ofColor::black);
	
	ofPushMatrix();
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
//	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	for(int i=0; i<nucleones.size(); i++) {
		nucleones[i].get()->drawNucleon();
	}

//	for(int i=0; i<nucleonesP.size(); i++) {
//		nucleonesP[i].get()->draw();
//	}

	ofDisableBlendMode();


	for(int i=0; i<quarksP.size(); i++) {
		quarksP[i].get()->draw();
	}
	
	ofPopStyle();
	ofPopMatrix();
	
	ofSetColor(0,0,0, 30);
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	
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
//	nucleones[0].get()->setPosition(x,y);
	
	crearQuark(x,y);

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


//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------


void testApp::setupGUI() {
	
	
}

