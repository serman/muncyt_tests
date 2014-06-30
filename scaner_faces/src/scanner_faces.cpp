#include "scanner_faces.h"

using namespace ofxCv;
using namespace cv;

void scanner_faces::setup() {
	ofSetVerticalSync(true);
	
	// utils
	colores.push_back(ofColor::paleVioletRed);
	colores.push_back(ofColor::lightSeaGreen);
	colores.push_back(ofColor::deepSkyBlue);
	colores.push_back(ofColor::papayaWhip);
	colores.push_back(ofColor::antiqueWhite);
	colores.push_back(ofColor::powderBlue);
	
	//  modos Haar
	modos_haar.push_back("haars/haarcascade_frontalface_alt2.xml");	// 1 (va ok)
	modos_haar.push_back("haars/haarcascade_frontalface_alt_tree.xml");	// 4 (va)
	modos_haar.push_back("haars/haarcascade_eye.xml");						// 5 
	modos_haar.push_back("haars/haarcascade_mcs_eyepair_big.xml");		// 6 À?
	modos_haar.push_back("haars/haarcascade_mcs_mouth.xml");			// 7 no
	modos_haar.push_back("haars/haarcascade_mcs_nose.xml");			// 9 no
	modos_haar.push_back("haars/haarcascade_upperbody.xml");			// 2 (no va muy bien. Creo que hay que hacer tridente)
	modos_haar.push_back("haars/haarcascade_profileface.xml");			// 3 (no va del todo bien Àbarba?)
//	modos_haar.push_back("haars/hogcascade_pedestrians.xml");			// 0 PETA
	// "haars/haarcascade_lefteye_2splits.xml"
	id_modo_haar_act = 0;
	
	setupObjFinders();
	
	// start
	cam.initGrabber(640, 480);

	// setup marco
//	float wMarco = ofGetWidth()*0.7;
//	float hMarco = wMarco*ofGetHeight()/ofGetWidth();
//	float xMarco = (ofGetWidth()-wMarco)/2;
//	float yMarco = (ofGetHeight()-hMarco)/2;
	float wMarco = cam.width*0.7;
	float hMarco = wMarco*cam.height/cam.width;
	float xMarco = (cam.width-wMarco)/2;
	float yMarco = (cam.height-hMarco)/2;
	
	
	marco.setRectExt(ofRectangle(0,0, cam.width, cam.height));
//	marco.setRectExt(ofRectangle(0,0, ofGetWidth(), ofGetHeight()));
	marco.setRect(ofRectangle(xMarco, yMarco, wMarco, hMarco));
	
	// escenas
	setupEscenas();
	setEscena(SCN_WAITING);
	
//	lastTF = ofGetElapsedTimef();
	
	// Grabar imagenes
	nPersonaAct = 0;
	nZonaAct = 0;
	snd_click.loadSound("sounds/click_214632.aiff");
	
	
}

void scanner_faces::setupObjFinders() {
	//finder caras;
//	finder.setup("haars/haarcascade_frontalface_alt2.xml");
	finder.setup(modos_haar[id_modo_haar_act]);
	finder.setPreset(ObjectFinder::Fast);
	
	//finder ojos;
	finderEyes.setup("haars/haarcascade_eye.xml");
	finderEyes.setPreset(ObjectFinder::Fast);
		
}


// - - - - 

void scanner_faces::update() {
	
	cam.update();
	
	
	if(cam.isFrameNew()) {
		bFaceDetected = false;
		finder.update(cam);
		
		if(finder.size()>0) {
			bFaceDetected = true;
			
			faceRect = finder.getObjectSmoothed(0);
			
			// Modifico el Rect
			// quedara en proporcion 3/2 = 15/10 
			//
			float dyUp  = 3*faceRect.height/10.0;	// 
			float dyDwn = 2*faceRect.height/10.0;	// 			
			faceRectAmpl = ofRectangle(faceRect.x, faceRect.y-dyUp, faceRect.width, faceRect.height+ dyUp+dyDwn);
			
			// Paso la imagen al finder de ojos
			faceImage.setFromPixels(cam.getPixels(), cam.width, cam.height, OF_IMAGE_COLOR );
			faceImage.crop(faceRectAmpl.x, faceRectAmpl.y, faceRectAmpl.width, faceRectAmpl.height);
			finderEyes.update(faceImage);
			
			
			
			
			// check si la cara esta dentro de los margenes
			caraOut = marco.isOut(faceRectAmpl);
			   
			   
		}
	}
	
	// Update una cosa u otra segœn el modo
	updateEscena();
	
}


void scanner_faces::draw() {
	ofSetBackgroundColor(ofColor::black);
	// Dibujar capas

//	float newTF = ofGetElapsedTimef();
//	float dt = newTF-lastTF;
//	lastTF = newTF;
//	ofLogNotice("tdraw0: " + ofToString(dt));	
	
	if(scnAct==SCN_WAITING) {
		draw_ScnWait();
	}
	else {
	
		// Camara 
		float hh = ofGetHeight();
		float ww = hh*cam.width/cam.height;
//		cam.draw((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2, ww,hh);
		ofPushMatrix();
		ofTranslate((ofGetWidth()-ww)/2,(ofGetHeight()-hh)/2);
		ofScale(ww/cam.width,hh/cam.height);
		cam.draw(0,0);
		
//		finder.draw();
		if(finder.size()>0) {
//			ofRectangle caraRect = finder.getObjectSmoothed(0);
			ofNoFill();
			ofRect(faceRect);
			// H
			for(int i=1; i<10; i++) {
				ofLine(faceRect.x, faceRect.y+faceRect.height/10*i , faceRect.x+faceRect.width, faceRect.y+faceRect.height/10*i);
			}
			// V
			ofLine(faceRect.x+faceRect.width/2, faceRect.y, faceRect.x+faceRect.width/2, faceRect.y+faceRect.height);
			ofSetColor(200);
			ofLine(faceRect.x+faceRect.width/3, faceRect.y, faceRect.x+faceRect.width/3, faceRect.y+faceRect.height);
			ofLine(faceRect.x+faceRect.width*2/3, faceRect.y, faceRect.x+faceRect.width*2/3, faceRect.y+faceRect.height);
			ofSetColor(150);
			ofLine(faceRect.x+faceRect.width/4, faceRect.y, faceRect.x+faceRect.width/4, faceRect.y+faceRect.height);
			ofLine(faceRect.x+faceRect.width*3/4, faceRect.y, faceRect.x+faceRect.width*3/4, faceRect.y+faceRect.height);
			
			
			
			if(!caraOut) 			ofSetColor(ofColor::blue);
			else					ofSetColor(ofColor::red);
			ofRect(faceRectAmpl);
			
			
			
			ofSetColor(255);
			// Eyes
			ofRectangle caraRect = finder.getObjectSmoothed(0);
			ofPushMatrix();
			ofTranslate(caraRect.x, caraRect.y,0);
			
			if(finderEyes.size()>0) {
				ofPushStyle();
				ofSetColor(ofColor::red);
				ofSetLineWidth(2);
				finderEyes.draw();
				ofPopStyle();
			}
			ofPopMatrix();
		}
		
		marco.draw();
		ofPopMatrix();
		
		
		// Dibujar una cosa u otra segœn el modo
		if(scnAct==SCN_PRESCAN) {
			ofDrawBitmapStringHighlight("SCN_PRESCAN: ", ofGetWidth()-100, ofGetHeight()-40);
			ofPushStyle();
				ofSetColor(255,60);
//				ofFill();
				ofRectangle rTmp = fontMensajes.getStringBoundingBox(mensajes[idMensajeAct], ofGetWidth()/2, ofGetHeight()/2);
				fontMensajes.drawString(mensajes[idMensajeAct], rTmp.x-rTmp.width/2, rTmp.y);
			ofPopStyle();
		}
				
		ofDrawBitmapStringHighlight("(w/s) haar: " + modos_haar[id_modo_haar_act], 10, 100);
		ofDrawBitmapStringHighlight("#objs: " + ofToString(finder.size()), 10, 120);
	}
	ofDrawBitmapStringHighlight("(q) " + nombreEscena, 10,10);
	ofDrawBitmapStringHighlight("fr: " + ofToString(ofGetFrameRate()), 10, 30);
	ofDrawBitmapStringHighlight("numCaras Acum: " + ofToString(contCaras), 10,50);
		
	if(bFaceDetected) {
		ofPushStyle();
		ofSetColor(ofColor::darkGreen);
		ofFill();
		ofRect(ofGetWidth()-30,0,30,30);
		ofPopStyle();
	}
	
}

//--------------------------------------------------------------
void scanner_faces::keyPressed(int key) {

	if(key == 'v') {
		cam.videoSettings();
	}
	else if(key == 't') ofToggleFullscreen();

	// Presets ObjectFinder
	if(key=='f') {
		finder.setPreset(ObjectFinder::Fast);
	}
	else if(key=='g') {
		finder.setPreset(ObjectFinder::Accurate);
	}
	else if(key=='h') {
		finder.setPreset(ObjectFinder::Sensitive);
	}

	if(key=='q') {
		scnAct++;
		scnAct%=4;
		setEscena(scnAct);
	}
	if(key=='a') {
		scnAct--;
		if(scnAct<0) scnAct+=4;
		setEscena(scnAct);
	}
	
	if(key=='w') {
		id_modo_haar_act++;
		id_modo_haar_act%=modos_haar.size();
		finder.setup(modos_haar[id_modo_haar_act]);
	}
	else if(key=='s') {
		id_modo_haar_act--;
		if(id_modo_haar_act<0) id_modo_haar_act+=modos_haar.size();;
		finder.setup(modos_haar[id_modo_haar_act]);
	}
	
	else if(key=='i') {
		// grabar imagen
		if(bFaceDetected) {
			
			saveScanImg(faceImage, nPersonaAct, nZonaAct);
			
			nZonaAct++;
			if(nZonaAct==5) {
				nPersonaAct++;
				nZonaAct=0;
			}
		}
	}
	

}

// - - - 

void scanner_faces::saveScanImg(ofImage &imgScan, int nPers, int nImg) {
	
	string nmImgScan = "images/scan/";
	nmImgScan += ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
				 ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
	nmImgScan += "cara_"+ofToString(nPers,3,'0')+"_"+ofToString(nImg,2,'0')+".png";
	ofLogNotice("grabar:_"+nmImgScan);
	imgScan.saveImage(nmImgScan);
	
	snd_click.play();
	
	
}

// - - - 

void scanner_faces::update_ScnWait() {
//	cout << path_Wait.getCommands().size() << endl;
//	ofLogNotice("update_ScnWait");
	
//	path_Wait.setUseShapeColor(true);
//	path_Wait.setColor(colores.at(floor(ofRandom(colores.size())) ) );
////	path_Wait.setColor(ofColor::black);
	float r = ofRandom(30,80);
	float xx = ofRandom(r, ofGetWidth()-r);
	float yy = ofRandom(r, ofGetHeight()-r);

	ofPath c;
	c.setCircleResolution(60);
	c.setFillColor(ofColor(colores.at(floor(ofRandom(colores.size()))), 100));
	c.circle(xx, yy, r);
	c.close();
//	ofLogNotice("update_ScnWait 1");
	shapes_Wait.push_back(c);
//	ofLogNotice("update_ScnWait 2");
	
	if(shapes_Wait.size()>40) {
		shapes_Wait.erase(shapes_Wait.begin(), shapes_Wait.begin()+3);
	}
//	ofLogNotice("update_ScnWait FIN");

}

void scanner_faces::draw_ScnWait() {
//	ofLogNotice("draw_ScnWait");
	ofPushStyle();
	ofSetColor(ofColor::beige); // beige
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight());
	
	for(int i=0; i<shapes_Wait.size(); i++) {
		shapes_Wait[i].draw();
	}
	
	ofPopStyle();
//	ofLogNotice("draw_ScnWait FIN");
	
}

// Control de secuencia
// listeners:
/*
void BEAU_Viz::tiempo_Sala_Duracion(ofEventArgs &e){
ofRemoveListener(timer.TIMER_REACHED, this, &BEAU_Viz::tiempo_Sala_Duracion);
ofAddListener(timerNoise.TIMER_REACHED, this, &BEAU_Viz::tiempo_Noise);	

	
	// Timer para la duracion del stream en cuadricula
	timer.setup(TIME_IPCAMS_MOSAICO, false);
	ofLogNotice("******** He lanzado el timer de duracion de Mosaico *****");
	
	// capturar evento TIME_IPCAMS_MOSAICO
	ofAddListener(timer.TIMER_REACHED, this, &BEAU_Viz::tiempo_IPCamMosaico);			
	
	// Lanzar sonido de streaming
	lanzaSonidoStreaming();
	
	
	// subir volumen al videoNoise
	videoNoise.setVolume(volume_videoNoise);
	timerNoise.setup(TIME_NOISE, false);
	swVideoNoise = true;
	ofAddListener(timerNoise.TIMER_REACHED, this, &BEAU_Viz::tiempo_Noise);	
	
	// - - - - - - - Control SECUENCIA - - - - - 
	
	int		TIME_IPCAMS;
	int		TIME_IPCAMS_MOSAICO;
	int		TIME_IPCAMS_AMPLIAR;
	int		TIME_NOISE;
	
	
	ofxTimer	timer;
	void		tiempo_IPCamOn(ofEventArgs &e);
	void		tiempo_IPCamMosaico(ofEventArgs &e);
	void		tiempo_IPCamAmpliar(ofEventArgs &e);
	
	ofxTimer	timerNoise;
	void		tiempo_Noise(ofEventArgs &e);
	bool		swVideoNoise;
	
*/

