#include "nuclear_fuerte.h"
#include "utilsGeom.cpp"

//--------------------------------------------------------------
void nuclear_fuerte::setup(){

	ofSetVerticalSync(true);
	
	// Lista de colores
	setupColores();
	
	// imagen png
	imgPunto.loadImage("images/dot.png");

	// setup geometria del sketch
	zentro = ofVec3f(ofGetWidth()/2.0,W_HEIGHT/2.0,0);

	radioEscena = W_HEIGHT/2;
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
	circExt.arc(zentro, radioEscena, radioEscena, 0, 360, true, 60);
	
	float rext = radioEscena;
	float rint = rext*0.97;
	float dAngDeg = 60.0;
	anilloExterior.setCircleResolution(60);
	anilloExterior.setFillColor(ofColor::red);
	anilloExterior.clear();
	anilloExterior.arc(ofVec2f(0,0), rint, rint, 0, dAngDeg, true);
	anilloExterior.arc(ofVec2f(0,0), rext, rext, dAngDeg, 0, false);
	anilloExterior.close();
	
	anilloExteriorLineas.setFilled(false);
	anilloExteriorLineas.setStrokeColor(ofColor::black);
	anilloExteriorLineas.setStrokeWidth(0.5);
	int nDivs = 120;
	for(int i=0; i<nDivs; i++) {
		anilloExteriorLineas.moveTo(rext*cos((float)TWO_PI/nDivs*i), rext*sin((float)TWO_PI/nDivs*i), 0);
		anilloExteriorLineas.lineTo(rint*cos((float)TWO_PI/nDivs*i), rint*sin((float)TWO_PI/nDivs*i), 0);
		anilloExteriorLineas.close();
	}
	
	
	// zona central
	radioZentro = 50;
	centroLab.set(zentro, radioZentro);
	
	r1 = radioZentro+(rint-radioZentro)/3.0;
	r2 = radioZentro+(rint-radioZentro)*2.0/3.0;

	ofMesh	mesh1, mesh2;
	
	nDivs = 180;
	for(int i=0; i<nDivs; i++) {
		float ang = TWO_PI/nDivs*i;
		mesh1.addVertex(ofVec3f(zentro.x+r1*cos(ang), zentro.y+r1*sin(ang),0));
		mesh1.addColor(ofColor(100,80));
		mesh2.addVertex(ofVec3f(zentro.x+r2*cos(ang), zentro.y+r2*sin(ang),0));
		mesh2.addColor(ofColor(100,80));
	}
	anilloFondo1.setMesh(mesh1, GL_STATIC_DRAW);
	anilloFondo2.setMesh(mesh2, GL_STATIC_DRAW);
	
	ofSetBackgroundAuto(bSetBack);
	
	setupGUI();

	init_Scena();
	
}

void nuclear_fuerte::init_Scena() {

	gui1->loadSettings("gui1Settings.xml");
	ofBackgroundGradient(ofColor(40), ofColor::black, OF_GRADIENT_CIRCULAR);

	// Emisores
	// Activar el emitter del mouse
	emitter.bActivo = false;
	totEmitters = 0;	// Para asignar identificadores a los emisores
	emitter.setId(totEmitters);	
	totEmitters++;
	
	emitter.ratePartic = ratePartic;

}


void nuclear_fuerte::exit_Scena() {
	gui1->saveSettings("gui1Settings.xml");    
	//	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	particulas.clear();
	emitters.clear();
	particulas_old.clear();
	
	particulas_out.clear();
	particulas_out_old.clear();
	
}


//--------------------------------------------------------------
void nuclear_fuerte::update(){
	
	// Variar MagnetField Auto
	if(changeMagneticField) {
		if(ofRandom(1.0)<0.01) {
			magnetField = ofRandom(2.0)-1.0;
		}
		
	}
	
	// rest del contador
	centroLab.reset();
	
	// add alguna particula desde un lateral
//	addParticleLateral();
	
	if(clearEMitters) {
		emitters.clear();
		clearEMitters = false;
	}
	
	// Emitir particulas
	if(ofGetMousePressed(0)) {
		emitter.ratePartic = ratePartic;
		addParticleFromEmiter(emitter);
	}
	for(int i=0; i<emitters.size(); i++) {
		addParticleFromEmiter(emitters[i]);
	}
	
	//
	// update partics
	//
	
	// aplicar fuerzas de interaccion part-part
	
	for(int i=0;i<particulas.size();i++) {
//		ofLogNotice("Update partic " + ofToString(i));
		
		// aplicar fuerzas globales
//		if(swMagnetField && particulas[i].inside) {
//		if(swMagnetField && (particulas[i].idEmitter==-10) && particulas[i].swActivaB) {
		if(swMagnetField && particulas[i].swActivaB) {
			particulas[i].forceMagnetZ(magnetField);
		}
		
		// interaccion spin?
		
		
		// mover 
//		ofLogNotice("Update partic " + ofToString(i) + "  --> update");
		particulas[i].update();
//		ofLogNotice("Update partic " + ofToString(i) + "  --> update Fin");
		
//		float rnd = ofRandom(1.0);
//		if(rnd<0.02) particulas[i].forceRadAround(zentro, 3.0, 300.0);
		camino = centroLab.circulo;
		
		// COLISION CON CAMINO
		if(camino.size()>0) {
			bool bCruce = false;
			if(GeometryUtils::estaDentro(particulas[i].position, camino)) {
				// si la partic esta dentro:
				particulas[i].color = ofColor::red;
				bCruce = particulas[i].setInside(true);	
				
				
				// que solo a�ada las particulas que entran, no las del emisor central
				if(particulas[i].idEmitter != -10) {
					centroLab.addParticle(particulas[i]);
				}
				
			}	
			else {
				particulas[i].color = particulas[i].color_orig; 
				bCruce = particulas[i].setInside(false);
				
				// Activamos el campo magnet cuando la partic sale del camino
				if(bCruce) particulas[i].swActivaB = true;
			}
			
			if(bCruce) {
				// encontrar el punto de choque con el camino
				ofVec2f ptChoque = GeometryUtils::getIntersectionPoly(particulas[i].position_prev, particulas[i].position, camino);
				if(ptChoque!=ofVec2f(0,0)) {
					if(bDrawPtosChoque) ptsChoque.push_back(ptChoque);
					
					// add el punto al camino de la particula
					particulas[i].insertPtChoque(ptChoque);
					
					if (swDifraccion) {
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
		
		
	}
	
	
	// remove particulas marcadas para borrar 
	for(int i=particulas.size()-1; i>=0;i--) {
		ofVec2f distZ = ofVec2f(particulas[i].position.x, particulas[i].position.y);
		distZ -= zentro;
		// Borrar las que esten fuera de la zona de interes
		if( distZ.length() > W_HEIGHT/2 ){
			particulas_old.push_back(particulas[i]);
			particulas.erase(particulas.begin()+i);
			continue;
		}
		
		// Borrar las que hayan superado el tiempo de vida
		if(particulas[i].tLife < 0) {
			particulas_old.push_back(particulas[i]);
			particulas.erase(particulas.begin()+i);
			continue;
		}
		
		// Borrar las que hayan pasado el punto central
		if( particulas[i].idEmitter != -10 && 
		   centroLab.bVariosEmitters && 
//		if( emitters.size()>1 && 
		    (distZ.dot(particulas[i].velocity)>0) ) // && alguna caracteristica!
		{	
			particulas_old.push_back(particulas[i]);
			particulas.erase(particulas.begin()+i);
			continue;
		}
		
		
		
	}
	
	// limitar a 100 las particulas
	if(particulas_old.size()>100) particulas_old.erase(particulas_old.begin()+(particulas_old.size()-100) );
	

	// update centro
	if(centroLab.updateEnd( ratePartic )) {
		// emitir particulas del centro
		for(int i=0; i<centroLab.newPartics.size(); i++) {
//			if(ofGetFrameNum()%ratePartic==0) {
			ParticleData pd = centroLab.newPartics[i];	
			// la devuelve con un tipo de particula, position, velocity
			pd.position += zentro;
			pd.color = coloresAll[pd.nColor];

			ParticleS p = ParticleS(pd);
			p.idEmitter = -10;
			p.inside = false;
			p.setInside(true);
			particulas.push_back(p);
//			}
		}
	}
	
	// Update emitter
	emitter.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);
	
	

}

void nuclear_fuerte::addParticleLateral() {
	//	float rnd = ofRandom(1.0);
	//	if(rnd<0.15) {
	if(ofGetFrameNum()%ratePartic==0) {
		float v = ofMap(mouseX,0,ofGetHeight(), 4, 10);
		float mass = 1+ofRandom(5);
		float carga = floor(ofRandom(10))-5;	// de -5 a 5
		
		int nColor = floor(ofRandom(6)); // 0,1,2 colores y 3,4,5 anticolores
		ofColor cTmp = coloresAll[nColor];
		

		ParticleS p = ParticleS(ofVec3f(0,ofRandom(ofGetHeight()),0), ofVec3f(v,0,0), cTmp, mass, carga );
		p.idEmitter = -1;
		particulas.push_back(p);
	}
}

void nuclear_fuerte::addParticleFromEmiter(Emisor &em) {
//	ofLogNotice("addParticleFromEmiter INI");
	if(em.bActivo) {
		if(ofGetFrameNum()%em.ratePartic==0) {
			ParticleData pd = em.getParticleData();	
			// la devuelve con un tipo de particula, position, velocity
			pd.position += zentro;
			// asigno el resto de valores
			pd.setTipoPart(pd.tpPartic);
			pd.color = coloresAll[pd.nColor];
//			ofLogNotice("addParticleFromEmiter 5: " + ofToString(pd.nColor));
			
			ParticleS p = ParticleS(pd);			
			p.idEmitter = em.idEmisor;			
			particulas.push_back(p);
			
			em.setColor(pd.color);
		}
	}
//	ofLogNotice("addParticleFromEmiter FIN");
	
}

void nuclear_fuerte::addEmisor(ofVec2f posTmp) {
	Emisor emTmp;
	float rhoTmp = posTmp.length();
	float angTmp = atan2(posTmp.y, posTmp.x);
	//			emTmp.setPos_Radial(rhoTmp, angTmp);
	//			emTmp.setPos_XY(mouseX-zentro.x, mouseY-zentro.y);
	emTmp.setPos_XY(posTmp.x, posTmp.y);
	
//	ofLogNotice(ofToString(angTmp));
	ofColor cTmp = ofColor::fromHsb(ofMap(angTmp,-PI,PI, 0, 255), 255, 255, 255);
	emTmp.setColor(cTmp);
	emTmp.setId(totEmitters);
	emTmp.ratePartic = ratePartic;
	totEmitters++;
	
	emitters.push_back(emTmp);
}

//--------------------------------------------------------------
void nuclear_fuerte::draw(){

	if(bSetBack) {
//	ofBackground(ofColor::black);
//		ofBackgroundGradient(ofColor(30), ofColor::black, OF_GRADIENT_CIRCULAR);
		ofBackgroundGradient(ofColor(40), ofColor::black, OF_GRADIENT_CIRCULAR);
	}
	else {
		ofPushStyle();
		ofSetColor(0,alpha_Back); //20);
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofPopStyle();
	}
	
	// Fondo Lab
	ofPushStyle();
		ofSetColor(255,100);
		ofSetCircleResolution(60);
		ofNoFill();
		ofSetLineWidth(0.5);
	
		anilloFondo1.draw(GL_LINES, 0, anilloFondo1.getNumVertices());
		anilloFondo2.draw(GL_LINES, 0, anilloFondo2.getNumVertices());
	
		ofSetLineWidth(0.3);
		for(int i=0; i<12; i++) {
			float ang=TWO_PI/12*i;
			ofLine(zentro.x+radioZentro*cos(ang), zentro.y+radioZentro*sin(ang), 
				   zentro.x+radioEscena*cos(ang), zentro.y+radioEscena*sin(ang));
		
		}
	
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
	

	ofEnableAlphaBlending();	
	// Dibujar part�culas
	for(int i=0;i<particulas.size();i++) {
		particulas[i].draw();
//		particulas[i].drawMemoPath();
	}
	ofDisableAlphaBlending();
	
	if(bDrawPtosChoque) {
		ofPushStyle();
//		ofSetColor(255,0,0);
//		ofFill();
		for(int i=0;i<ptsChoque.size();i++) {
			ofCircle(ptsChoque[i].x, ptsChoque[i].y, 3);
	//		ofDrawBitmapString(ofToString(ptsChoque[i].x)+","+ofToString(ptsChoque[i].y), ptsChoque[i]);
		}
		ofPopStyle();
	}
	
	// 
	// NUCLEAR FUERTE
	// 
	// Escena centrada y simetr�a circular
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
	
	// decoraci�n borde Exterior
	// hay que animarla un poco
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
		ofColor cc = colores[i%3];
		float nois = ofNoise((float)ofGetElapsedTimeMillis()*i/3000.0 );
//		ofLogNotice() << nois;
		cc.setBrightness(cc.getBrightness()*(1-0.8*nois));
		anilloExterior.setFillColor(cc);
		anilloExterior.draw();
	}
	anilloExteriorLineas.draw();
	ofPopMatrix();
	ofPopStyle();

	
	centroLab.drawStats(ofRectangle(ofGetWidth()-200,0,200,200));
	
	
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
void nuclear_fuerte::keyPressed(int key){

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
	else if(key=='r') {
		bDrawPtosChoque=!bDrawPtosChoque;
		if(!bDrawPtosChoque) ptsChoque.clear();
	}
	else if(key=='t') bTiltCamino=!bTiltCamino;
}

//--------------------------------------------------------------
void nuclear_fuerte::keyReleased(int key){

}

//--------------------------------------------------------------
void nuclear_fuerte::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void nuclear_fuerte::mouseDragged(int x, int y, int button){
	if(button==0) {
		if(bDrawingMode) {		
		}
	}
	else if(button==2) {
		// mover el �ltimo emitter si esta en la zona de accion
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
void nuclear_fuerte::mousePressed(int x, int y, int button){
	
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
			addEmisor(posTmp);
		}
	}
}

//--------------------------------------------------------------
void nuclear_fuerte::mouseReleased(int x, int y, int button){

	if(button==0) {
	}
	
	emitter.bActivo = false;
}

//--------------------------------------------------------------
void nuclear_fuerte::windowResized(int w, int h){

}

//--------------------------------------------------------------
void nuclear_fuerte::gotMessage(ofMessage msg){

}



// - - - - GUI - - - - 
void nuclear_fuerte::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("STRONG NUCLEAR");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
	//    gui1->addSpacer();
	gui1->addLabel("Magnetic Field");
	gui1->addToggle("(b) Magnetic Field ON/OFF", &swMagnetField );
	gui1->addToggle("Variar Auto MagnetF ON/OFF", &changeMagneticField );
	gui1->addSlider("Magnetic Force", -1.0, 1.0, &magnetField);
	
	gui1->addSpacer();
	gui1->addToggle("(m) Modo Dibujo ON/OFF", &bDrawingMode );
	gui1->addToggle("(d) Difracci�n ON/OFF", &swDifraccion );
	gui1->addToggle("(e) Draw Camino ON/OFF", &bDrawCaminos );
	gui1->addToggle("(r) Draw Colisiones ON/OFF", &bDrawPtosChoque );
	gui1->addToggle("(t) Tilt Camino ON/OFF", &bTiltCamino );
	gui1->addToggle("(o) Clear Emitters", &clearEMitters );
	gui1->addToggle("BackGround New", &bSetBack );
	gui1->addSlider("Alpha background", 0, 255, &alpha_Back);	
	gui1->addSpacer();
	gui1->addButton("Save", bSave);
	gui1->addButton("Load", bSave);
	
//	ofDrawBitmapString("'c' clear camino", 10,hLin); hLin+=dLin;
//	ofDrawBitmapString("p clear partics", 10,hLin); hLin+=dLin;
	//	
	gui1->addSpacer();
	gui1->addIntSlider("(q/a)_Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&nuclear_fuerte::guiEvent);	
}

void nuclear_fuerte::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
	if(name == "(r) Draw Colisiones ON/OFF")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		bDrawPtosChoque = toggle->getValue();
		if(!bDrawPtosChoque) ptsChoque.clear();
	}
	else if(name=="Save") {
		ofLogNotice("GRABAR");
		gui1->saveSettings("gui1Settings.xml");    
	}
	else if(name=="Load") {
		ofLogNotice("CARGAR");
		gui1->loadSettings("gui1Settings.xml");    
	}
	
}


void nuclear_fuerte::exit()
{
    gui1->saveSettings("gui1Settings.xml");    
//	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	particulas.clear();
	emitters.clear();
	particulas_old.clear();
	
}

void nuclear_fuerte::setupColores() {
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

void nuclear_fuerte::tiltCamino() {
	ofPolyline pl = camino;
	camino.clear();
	
	float maxDespl = 2.0;
	for(int i=0; i<pl.size(); i++) {
		camino.addVertex( pl[i]+ofVec2f(ofRandom(-maxDespl, maxDespl), ofRandom(-maxDespl, maxDespl)) );
	}
	camino.close();
}





