#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableAntiAliasing();

	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(30.0);

	isKeyPressed = false;
	isMousePressed = false;
	
	// add objects
	for (int i=0; i<200; i++) {
		// circulos
		addCircle(ofPoint(ofGetWidth()/2+ofRandom(100), ofGetHeight()/2+ofRandom(100)));
				  
		// rectangulos
		addBox(ofPoint(ofGetWidth()/2+ofRandom(100), ofGetHeight()/2+ofRandom(100)));
	}
	bAddCircle = false;
	bAddBox = false;
	
	ptoMed_circles = ptoMedio(circles);
	ptoMed_boxes = ptoMedio(boxes);
	
	bola.loadImage("images/dot.png");
	ladoPart1 = 5;
	ladoPart2 = 10;
	
	coloresBN.push_back(ofColor(0));
	coloresBN.push_back(ofColor(150));
	coloresBN.push_back(ofColor(255));
	
	bDraw4Forces = true;
	
}

void testApp::setupSectores() {
	ofVec2f centroScreen = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	float rrr = ofGetHeight()/2.0;
	
	for(int i=0; i<4; i++) {
		float ang = i*HALF_PI;
		ofPolyline pl;
		pl.addVertex(centroScreen.x,centroScreen.y,0);
		pl.addVertex(rrr*cos(ang),rrr*sin(ang),0);
		pl.arc(centroScreen, rrr, rrr, DEG_TO_RAD*ang, DEG_TO_RAD*(ang+HALF_PI), true);
		pl.close();
		
	}
	
}

void testApp::addCircle(ofPoint _pos) {
	ofLogNotice("AddCircle");
	float r = ofRandom(3, 6);		
	ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
	circle.get()->setPhysics(3.0, 0.53, 0.9);
	circle.get()->setup(box2d.getWorld(), _pos.x, _pos.y, r);
	circle.get()->setFriction(0.9);
	circles.push_back(circle);
	ofLogNotice("AddCircle-Exit");	
}

void testApp::addBox(ofPoint _pos) {
	float w = ofRandom(4, 8);	
	float h = ofRandom(w, 12);	
	ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
	rect.get()->setPhysics(1.5, 0.53, 0.9);
	rect.get()->setup(box2d.getWorld(), _pos.x, _pos.y, w, h);
	boxes.push_back(rect);
}
				  


//--------------------------------------------------------------
void testApp::update() {
	
	ofLogNotice("Update");
	if(bAddCircle) {
		ofLogNotice("Update-AddCircle");
		addCircle(ofPoint(ofGetMouseX(),ofGetMouseY()));
	}
	if(bAddBox) {
		addBox(ofPoint(ofGetMouseX(),ofGetMouseY()));
	}
	
	box2d.update();	
	
	if(!bAddCircle && !bAddBox) {
		ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
		float minDis = ofGetMousePressed() ? 300 : 200;

		ofVec2f center(ofGetWidth()/2.0, ofGetHeight()/2.0);
		float distConf = ofGetHeight()/2.0*0.9;

		float minDisInt = 25;
		int ncircles = circles.size();
		int nboxes = boxes.size();
		int ntot = ncircles+nboxes;
		
		// = Fuerzas entre particulas
		// Se puede hacer estadistico por ahora
		// Tambien se puede hacer por densidades:
		// - calcular pto medio de cada tipo de particula y carga total
		// - calcular la fuerza ejercida sobre cada particula por todos los grupos de particulas
		ptoMed_circles = ptoMedio(circles);
		ptoMed_boxes = ptoMedio(boxes);
		if(true) {
			for(int i=0; i<circles.size(); i++) {
				float dis1 = ptoMed_circles.distance(circles[i].get()->getPosition());
				float dis2 = ptoMed_boxes.distance(circles[i].get()->getPosition());
				if(dis1 > minDisInt) boxes[i].get()->addRepulsionForce(ptoMed_circles, 2*(ncircles/ntot)/dis1);
				if(dis2 > minDisInt) boxes[i].get()->addAttractionPoint(ptoMed_boxes, 2*(nboxes/ntot)/dis2);
			}
			for(int i=0; i<boxes.size(); i++) {
				float dis1 = ptoMed_boxes.distance(boxes[i].get()->getPosition());
				float dis2 = ptoMed_circles.distance(boxes[i].get()->getPosition());
				if(dis1 > minDisInt) boxes[i].get()->addRepulsionForce(ptoMed_boxes, 2*(nboxes/ntot)/dis1);
				if(dis2 > minDisInt) boxes[i].get()->addAttractionPoint(ptoMed_circles, 2*(ncircles/ntot)/dis2);
			}
		}
		
		
		// = Interaccion mouse/TUIOs:
		// Solo se da si el mouse esta en la zona de confinamiento
		// y depende de si el mouse is Pressed o no:
		
		// pressed: 
		//  - REPULSION circulo y ATRACCION box: si dist-mouse < distMinima
		//  - ATRACCION circulo y REPULSION box: si dist-mouse > distMaxima
		// 
		
		// o poner los tuios impares con atraccion y los impares con repulsion
		float fFuerza = 5.0;
		if(mouse.distance(center)<distConf) {
			if(isMousePressed) {
				for(int i=0; i<circles.size(); i++) {
					float dis = mouse.distance(circles[i].get()->getPosition());
					if(dis < minDis) circles[i].get()->addRepulsionForce(mouse, 0.2f*fFuerza/dis);//3, 9);
					else circles[i].get()->addAttractionPoint(mouse, 4.2*fFuerza/dis);//4.0);
				}
				for(int i=0; i<boxes.size(); i++) {
					float dis = mouse.distance(boxes[i].get()->getPosition());
					if(dis < minDis) boxes[i].get()->addRepulsionForce(mouse, 1.8*fFuerza/dis);
					else boxes[i].get()->addRepulsionForce(mouse, 1.9*fFuerza/dis);//4.0);
				}
			}
			else {
				for(int i=0; i<circles.size(); i++) {
					float dis = mouse.distance(circles[i].get()->getPosition());
					if(dis < minDis) circles[i].get()->addAttractionPoint(mouse, 1.5*fFuerza/dis);//3, 9);
					else circles[i].get()->addRepulsionForce(mouse, 0.9*fFuerza/dis);//4.0);
				}
				for(int i=0; i<boxes.size(); i++) {
					float dis = mouse.distance(boxes[i].get()->getPosition());
					if(dis < minDis) boxes[i].get()->addAttractionPoint(mouse, 1.8*fFuerza/dis);
					else boxes[i].get()->addAttractionPoint(mouse, 1.9*fFuerza/dis);//4.0);
				}
			}
		}
		
		
		// = Fuerza de confinamiento
		for(int i=0; i<circles.size(); i++) {
			float dis = center.distance(circles[i].get()->getPosition());
			if(dis > distConf) circles[i].get()->addAttractionPoint(center, 0.9);//3, 9);
		}
		for(int i=0; i<boxes.size(); i++) {
			float dis = center.distance(boxes[i].get()->getPosition());
			if(dis > distConf) boxes[i].get()->addAttractionPoint(center, 0.9);//3, 9);
		}
	
	}
	
	bAddCircle=false;
	bAddBox=false;

}

ofVec2f testApp::ptoMedio(vector <ofPtr<ofxBox2dCircle> > shapes) {
	ofVec2f ptMed;
	for(int i=0; i<shapes.size(); i++) {
		ptMed+=shapes[i].get()->getPosition();
	}
	ptMed/=shapes.size();
	return ptMed;
}
ofVec2f testApp::ptoMedio(vector <ofPtr<ofxBox2dRect> > shapes) {
	ofVec2f ptMed;
	for(int i=0; i<shapes.size(); i++) {
		ptMed+=shapes[i].get()->getPosition();
	}
	ptMed/=shapes.size();
	return ptMed;
}


//--------------------------------------------------------------
void testApp::draw() {
	if(bDraw4Forces) {
		draw4Forces();
	}
	else {
		ofEnableAlphaBlending();
		for(int i=0; i<circles.size()-1; i++) {
			ofFill();
	//		ofSetHexColor(0xf6c738);
	//		circles[i].get()->draw();
			
			ofVec2f pos = circles[i].get()->getPosition();
			float rr = 3*circles[i].get()->getRadius();
			ofPushMatrix();
			ofPushStyle();
			ofTranslate(pos.x,pos.y,0);
			ofSetColor(ofColor::royalBlue, 200);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopStyle();
			ofPopMatrix();
		}
		
		for(int i=0; i<boxes.size(); i++) {
			ofFill();
	//		ofSetHexColor(0xBF2545);
	//		boxes[i].get()->draw();
			
			ofVec2f pos = boxes[i].get()->getPosition();
			float rr = 3*boxes[i].get()->getHeight();
			ofPushMatrix();
			ofPushStyle();
			ofTranslate(pos.x,pos.y,0);
			ofSetColor(ofColor::red, 180);
			bola.draw(-rr,-rr,2*rr,2*rr);
			ofPopStyle();
			ofPopMatrix();
			
		}
		
		// ptos Medios
		ofPushStyle();
		ofSetLineWidth(4);
		ofSetColor(ofColor::royalBlue);
		ofLine(ptoMed_circles.x-10, ptoMed_circles.y, ptoMed_circles.x+10, ptoMed_circles.y);
		ofLine(ptoMed_circles.x, ptoMed_circles.y-10, ptoMed_circles.x, ptoMed_circles.y+10);
		ofSetColor(ofColor::red);
		ofLine(ptoMed_boxes.x-10, ptoMed_boxes.y, ptoMed_boxes.x+10, ptoMed_boxes.y);
		ofLine(ptoMed_boxes.x, ptoMed_boxes.y-10, ptoMed_boxes.x, ptoMed_boxes.y+10);
		ofPopStyle();
		
		// draw the ground
		//box2d.drawGround();
		
		ofDisableAlphaBlending();

	}

	ofPath borde;
	borde.clear();
	ofColor ctmp = ofColor::blue;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.circle(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2*0.8);	
	borde.draw();
	
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "(4) draw 4Forces: "+ofToString(bDraw4Forces)+"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

void testApp::draw4Forces() {
	
	ofEnableAlphaBlending();
	for(int i=0; i<circles.size()-1; i++) {		
		ofVec2f pos = circles[i].get()->getPosition();
		float rr = circles[i].get()->getRadius();
		drawBola4(pos, rr);		
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofVec2f pos = boxes[i].get()->getPosition();
		float rr = boxes[i].get()->getHeight();
		drawBola4(pos, rr);		
	}	
	
	
	ofDisableAlphaBlending();
}

void testApp::drawBola4(ofVec2f pos, float radius) {
	ofVec2f vx = ofVec2f(1,0);
	ofVec2f centroScreen = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	ofVec2f posCentro =  pos-centroScreen;
	float angulo = vx.angle(posCentro);
	while(angulo<0) {angulo+=360.0;}
	
	ofPushMatrix();
	ofPushStyle();
	ofFill();
	ofTranslate(pos.x,pos.y,0);
	if(angulo>=0 && angulo<90) {
		float rr = 2*radius;
		ofSetColor(ofColor::royalBlue, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofTranslate(-posCentro.x,-posCentro.y,0);
		ofSetColor(ofColor::royalBlue, 80);
		ofSetLineWidth(0.2);
		ofLine(0,0,  posCentro.length()*cos(angulo*DEG_TO_RAD), posCentro.length()*sin(angulo*DEG_TO_RAD));
		ofPolyline arco;
		arco.arc(ofPoint(0,0), posCentro.length(), posCentro.length(),0, 90);
		arco.draw();
		
	}
	
	else if(angulo>=90 && angulo<180) {
		float rr = 6*radius;
		ofSetColor(ofColor::green, 200);
		bola.draw(-rr,-rr,2*rr,2*rr);
	}
	else if(angulo>=180 && angulo<270) {
		//			ofSetColor(ofColor::deepPink, 200);
		//			bola.draw(-rr,-rr,2*rr,2*rr);
		ofSetColor(150 + ofRandom(155));
		ofCircle(0,0,radius/8);
	}
	else if(angulo>=270 && angulo<360) {
		float rr = 2*radius;
		ofSetColor(ofColor::red, 150);
		bola.draw(-rr,-rr,2*rr,2*rr);
		
		ofPushMatrix();
		ofTranslate(-pos.x, -pos.y, 0);
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
		float rot = 2*(315-angulo);
		ofRotate(rot);
		ofTranslate(posCentro.x, posCentro.y,0);
		ofSetColor(ofColor::darkRed  , 150);
		rr = 2*radius;
		bola.draw(-rr,-rr,2*rr,2*rr);
		ofPopMatrix();
		
	}
	
	ofPopStyle();
	ofPopMatrix();	
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
		//addCircle(ofPoint(mouseX, mouseY));
		bAddCircle=true;
	}
	
	if(key == 'b') {
		//addBox(ofPoint(mouseX, mouseY));
		bAddBox=true;
	}
	
	if(key == 't') ofToggleFullscreen();
	
	if(key == '4') bDraw4Forces = !bDraw4Forces;
	
	isKeyPressed = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	isKeyPressed = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

