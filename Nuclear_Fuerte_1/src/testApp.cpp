#include "testApp.h"
#include "utilsGeom.cpp"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	// Lista de colores
	setupColores();
	
	// setup geometria del sketch
	zentro = ofVec3f(ofGetWidth()/2.0,W_HEIGHT/2.0,0);

	radioEscena = W_HEIGHT/2*0.95;
	// Borde Negro circular
	borde.clear();
	ofColor ctmp = ofColor::black;//red;//black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(60);
	borde.circle(ofGetWidth()/2, W_HEIGHT/2, radioEscena);	
	
	// borde exterior
	circExt.clear();
	circExt.arc(zentro, radioEscena, radioEscena, 0, 360, true, 30);
	
	float rext = radioEscena;
	float rint = rext*0.95;
	float dAngDeg = 60.0;
	anilloExterior.setCircleResolution(30);
	anilloExterior.setFillColor(ofColor::red);
	anilloExterior.clear();
	anilloExterior.arc(ofVec2f(0,0), rint, rint, 0, dAngDeg, true);
	anilloExterior.arc(ofVec2f(0,0), rext, rext, dAngDeg, 0, false);
	anilloExterior.close();
	
	
	
	// zona central
	centroLab.set(zentro, 50);
	
	emitter.bActivo = false;
	totEmitters = 0;	// Para asignar identificadores a los emisores
	emitter.setId(totEmitters);
	totEmitters++;
	
	
	//
	swMagnetField = false;
	swDifraccion = false;
	
	ratePartic = 20;
	
	bDrawingMode = false;
	bDrawCaminos = true;
	bDrawPtosChoque = false;
	bTiltCamino = false;	
	
	clearEMitters = false;	
	
	setupGUI();
	
	ofBackgroundGradient(ofColor(30), ofColor::black, OF_GRADIENT_CIRCULAR);
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void testApp::update(){
	
	// rest del contador
	centroLab.reset();
	
	// add alguna particula desde un lateral
//	addParticleLateral();
	
	if(clearEMitters) {
		emitters.clear();
		clearEMitters = false;
	}
	
	addParticleFromEmiter(emitter);
	for(int i=0; i<emitters.size(); i++) {
		addParticleFromEmiter(emitters[i]);
	}
	
	
	
	// update partics
	
	// aplicar fuerzas de interaccion part-part

	
	for(int i=0;i<particulas.size();i++) {
		
		// aplicar fuerzas globales
		if(swMagnetField && particulas[i].inside) {
			particulas[i].forceMagnetZ(magnetField);
		}
		
		
		
		// mover 
		particulas[i].update();
		
//		float rnd = ofRandom(1.0);
//		if(rnd<0.02) particulas[i].forceRadAround(zentro, 3.0, 300.0);
		camino = centroLab.circulo;
		
		// COLISION CON CAMINO
		if(camino.size()>0) {
			bool bCruce = false;
			if(GeometryUtils::estaDentro(particulas[i].position, camino)) {
				particulas[i].color = ofColor::red;
				bCruce = particulas[i].setInside(true);	
				
				
				
				// que vaya sumando momentos
				
//				centroLab.contadorPartics++;
				centroLab.addParticle(particulas[i]);
				
				// **** 
				
				
			}
			else {
				particulas[i].color = particulas[i].color_orig; 
				bCruce = particulas[i].setInside(false);	
			}
			
			if(bCruce && swDifraccion) {
				// check si partic choca con shillouette

				ofVec2f ptChoque = GeometryUtils::getIntersectionPoly(particulas[i].position_prev, particulas[i].position, camino);
				if(ptChoque!=ofVec2f(0,0)) {
					ptsChoque.push_back(ptChoque);
					
					// add el punto al camino de la particula
					particulas[i].insertPtChoque(ptChoque);
					
					// recalcular la direccion de la particula.
//					ofVec2f dirI = particulas[i].position - ptChoque;
//					ofVec2f dirI = particulas[i].velocity;
					// rotate random
					float angRotMax = 15;
					float angRot =  ofRandom(-angRotMax, angRotMax);
					ofColor cc = ofColor::fromHsb(ofMap(abs(angRot), 0, angRotMax, 0,255), 255, 255, 100);
					particulas[i].colorExcited = cc;
					particulas[i].velocity.rotate(angRot, ofVec3f(0,0,1));
					
				}
			}
		}
		
		
	}
	
	
	// remove particulas marcadas para borrar (por ej que esten fuera de screen)
	for(int i=particulas.size()-1; i>=0;i--) {
		ofVec2f distZ = ofVec2f(particulas[i].position.x, particulas[i].position.y);
		distZ -= zentro;
		if( distZ.length() > W_HEIGHT ){
			particulas_old.push_back(particulas[i]);
			particulas.erase(particulas.begin()+i);
			
			// limitar a 100 las particulas
			if(particulas_old.size()>100) particulas_old.erase(particulas_old.begin()+i);
		}
	}

	// Update emitter
	emitter.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);

}

void testApp::addParticleLateral() {
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		//		ParticleS( ofVec3f _position , ofColor _color, float mass = 1.0, float charge = 0.0 )
		//		ParticleS( ofVec3f _position , ofVec3f _vel , ofColor _color, float mass = 1.0, float charge = 0.0 )
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		float mass = 1+ofRandom(5);
		float carga = floor(ofRandom(10))-5;	// de -5 a 5
		
		int nColor = floor(ofRandom(6)); // 0,1,2 colores y 3,4,5 anticolores
		ofColor cTmp = coloresAll[nColor];
		
		p.idEmitter = -1;

		ParticleS p = ParticleS(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), cTmp, mass, carga );
		particulas.push_back(p);
	}
}

void testApp::addParticleFromEmiter(Emisor &em) {
	
	if(em.bActivo) {
		if(ofGetFrameNum()%em.ratePartic==0) {
			ParticleData pd = em.getParticleData();
			pd.position += zentro;
			pd.color = coloresAll[pd.nColor];
			ParticleS p = ParticleS(pd);
			
			p.idEmitter = em.idEmisor;
			p.refEmitter = &em;
			
			particulas.push_back(p);
		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){

//	ofBackground(ofColor::black);
//	ofBackgroundGradient(ofColor(30), ofColor::black, OF_GRADIENT_CIRCULAR);
	ofPushStyle();
	ofSetColor(0,20);
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	ofPopStyle();
	
	//
	// PARTICULAS 
	//
	
	ofEnableSmoothing();
	ofEnableAntiAliasing();

	if(bDrawCaminos) {
		ofPushStyle();
		ofSetColor(ofColor::lime, 60);
		ofSetLineWidth(15);
		camino.draw();
	}
	
	for(int i=0;i<particulas.size();i++) {
		particulas[i].draw();
//		particulas[i].drawMemoPath();
	}
	
	if(bDrawPtosChoque) {
		ofPushStyle();
		for(int i=0;i<ptsChoque.size();i++) {
			ofCircle(ptsChoque[i].x, ptsChoque[i].y, 3);
	//		ofDrawBitmapString(ofToString(ptsChoque[i].x)+","+ofToString(ptsChoque[i].y), ptsChoque[i]);
		}
		ofPopStyle();
	}
	
	// 
	// NUCLEAR FUERTE
	// 
	// Escena centrada y simetr’a circular
	centroLab.draw();
	ofPushMatrix();
		ofTranslate(zentro.x, zentro.y, zentro.z);
		// Decorados
		
	
		// Emisores
		emitter.draw();
		for(int i=0; i<emitters.size(); i++) {
			emitters[i].draw();
		}
	
	ofPopMatrix();
	
	
	// Borde Escena
	borde.draw();
	
	// decoraci—n borde Exerior
	ofPushStyle();
	// linea blanca
	ofSetLineWidth(4);
	ofSetColor(255);
	ofNoFill();
	circExt.draw();
	
	ofPushMatrix();
	ofTranslate(zentro.x,zentro.y,0);
	for(int i=0; i<6; i++) {
		ofRotateZ(60);
		anilloExterior.setFillColor(colores[i%3]);
		anilloExterior.draw();
	}
	ofPopMatrix();
	ofPopStyle();
	
	//
	// INFO
	// 
	ofPushStyle();
	ofSetColor(200);
	int hLin = ofGetHeight()-25; int dLin = -15;
	ofDrawBitmapString("Num Partics: " + ofToString(particulas.size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("Num Partics old: " + ofToString(particulas_old.size()), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("o clear emitters", 10,hLin); hLin+=dLin;
	ofDrawBitmapString("Partics dentro: " + ofToString(centroLab.contadorPartics), 10,hLin); hLin+=dLin;
	ofDrawBitmapString("FR: " + ofToString(ofGetFrameRate()), 10,hLin); hLin+=dLin;
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if(key=='d') swDifraccion=!swDifraccion;
	else if(key=='b') swMagnetField=!swMagnetField;
	else if(key=='q') ratePartic++;
	else if(key=='a') ratePartic = (ratePartic>2)? ratePartic-1 : 1;
	else if(key=='p') {
		particulas.clear();
	}
	else if(key=='o') {
		clearEMitters = true;
//		emitters.clear();
	}
	else if(key=='m') bDrawingMode=!bDrawingMode;
	else if(key=='e') bDrawCaminos=!bDrawCaminos;
	else if(key=='r') bDrawPtosChoque=!bDrawPtosChoque;
	else if(key=='t') bTiltCamino=!bTiltCamino;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(button==0) {
		if(bDrawingMode) {		
		}
	}
	else if(button==2) {
		// mover el œltimo emitter si esta en la zona de accion
		ofVec2f posTmp(x,y);		
		posTmp -= zentro;
		if(posTmp.length()<=radioEscena) {
	//		float rhoTmp = posTmp.length();
	//		float angTmp = atan2(posTmp.y, posTmp.x);
	//		emitters[emitters.size()-1].setPos_Radial(rhoTmp, angTmp);
			emitters[emitters.size()-1].setPos_XY(mouseX-zentro.x, mouseY-zentro.y);
		}
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
//	if(camino.getVertices().size()>4) camino.curveTo(x,y);
//	else camino.addVertex(x,y);
	
	if(button==0) {
		if(bDrawingMode) {		
			ofVec2f posTmp(x,y);		
			posTmp -= zentro;
			if(posTmp.length()<=radioEscena) {
			}
		}
		else emitter.bActivo = true;
	}
	else if(button==2) {
		// add nuevo emitter
		// 
		ofVec2f posTmp(x,y);		
		posTmp -= zentro;
		if(posTmp.length()<=radioEscena) {
			Emisor emTmp;
			float rhoTmp = posTmp.length();
			float angTmp = atan2(posTmp.y, posTmp.x);
//			emTmp.setPos_Radial(rhoTmp, angTmp);
			emTmp.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);
		
			ofLogNotice(ofToString(angTmp));
			ofColor cTmp = ofColor::fromHsb(ofMap(angTmp,-PI,PI, 0, 255), 255, 255, 255);
			emTmp.setColor(cTmp);
			emTmp.setId(totEmitters);
			totEmitters++;
			
			emitters.push_back(emTmp);
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	if(button==0) {
	}
	
	emitter.bActivo = false;
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



// - - - - GUI - - - - 
void testApp::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("STRONG NUCLEAR");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
	//    gui1->addSpacer();
	gui1->addLabel("Magnetic Field");
	gui1->addToggle("(b) Magnetic Field ON/OFF", &swMagnetField );
	gui1->addSlider("Magnetic Force", -5, 5.0, &magnetField);
	
	gui1->addSpacer();
	gui1->addToggle("(m) Modo Dibujo ON/OFF", &bDrawingMode );
	gui1->addToggle("(d) Difracci—n ON/OFF", &swDifraccion );
	gui1->addToggle("(e) Draw Camino ON/OFF", &bDrawCaminos );
	gui1->addToggle("(r) Draw Colisiones ON/OFF", &bDrawPtosChoque );
	gui1->addToggle("(t) Tilt Camino ON/OFF", &bTiltCamino );
	gui1->addToggle("(o) Clear Emitters", &clearEMitters );

	
//	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
//	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	//	
	gui1->addSpacer();
	gui1->addIntSlider("(q/a)_Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);	
}

void testApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
//	if(name == "radio_Sol")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		rSol = slider->getScaledValue();
//		sol.setRadius(rSol);
//		setMfromR();
//	}
//	else if(name == "cam_z")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget;
//		zCam = slider->getScaledValue();
//		cam.setDistance(zCam);
//	}
//	else if(name == "TOGGLE_OrthoCamera")
//	{	// swOrtho
//		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
//		swOrtho = toggle->getValue();
//		if(swOrtho) cam.enableOrtho();
//		else cam.disableOrtho();
//	}
	
}


void testApp::exit()
{
//    gui1->saveSettings("gui1Settings.xml");    
//	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	
}

void testApp::setupColores() {
	colores[0] = ofColor::red; 
	colores[1] = ofColor::green;
	colores[2] = ofColor::blue;
	antiColores[0] = colores[0]; antiColores[0].invert();
	antiColores[1] = colores[1]; antiColores[1].invert();
	antiColores[2] = colores[2]; antiColores[2].invert();
	for(int i=0; i<3; i++) {
		coloresAll[i] = colores[i];
		coloresAll[i+3] = antiColores[i];
	}
}

void testApp::tiltCamino() {
	ofPolyline pl = camino;
	camino.clear();
	
	float maxDespl = 2.0;
	for(int i=0; i<pl.size(); i++) {
		camino.addVertex( pl[i]+ofVec2f(ofRandom(-maxDespl, maxDespl), ofRandom(-maxDespl, maxDespl)) );
	}
	camino.close();
}



