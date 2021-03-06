#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(34, 34, 34);
	ofSetVerticalSync(true);
	
	ofEnableAlphaBlending();
	
	ofSetCircleResolution(60);
	
	zentro = ofVec2f(ofGetWidth()/2.0, W_HEIGHT/2.0);

	//
	// Borde
	//
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
	borde.circle(zentro.x, zentro.y, radioEscena);	
	
	// Imagen textura
	imgTex.loadImage("imagen.jpg");
	bUseTexture = true;
	
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
	font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	
	// Meshes
	// superficie
	setupMeshSuperf();
	
	swWireFrame = false;
	
	// Light
	light.setDirectional();
	light.setPosition(0,0,50);
	light.setOrientation( ofVec3f(180,0, 0) );
//	ofFloatColor ambient_color(0.0,0.0,0.0,1.0);
//	ofFloatColor diffuse_color(1.0,0.0,0.0, 0.2);
//	ofFloatColor specular_color(1.0,1.0,1.0, 0.1);	
//	light.setAmbientColor(ambient_color);
//	light.setDiffuseColor(diffuse_color);
//	light.setSpecularColor(specular_color);
	swLight = true;
	light.enable();
	swOrtho = false;
	swWeb = false;
	
	float wFBOs = W_HEIGHT;
	
	// FBO	
	imgMix.allocate(wFBOs,wFBOs, GL_RGBA);
	imgMix.begin();
	ofClear(0,0,0, 0);
	imgMix.end();
	
	ofPushStyle();
	imgDyn.allocate(wFBOs,wFBOs, GL_RGBA);
	imgDyn.begin();
		ofClear(0,0,0, 0);
	imgDyn.end();
	
	
	imgWeb.allocate(wFBOs,wFBOs, GL_RGBA);
	imgWeb.begin();
		ofClear(0,0,0, 0);
	imgWeb.end();

	// Dibujar malla
	imgWeb.begin();
	ofBackground(200);
		ofSetColor(255,255,255);
//		ofEnableAntiAliasing();
		ofEnableSmoothing();
		ofSetLineWidth(1.5);
		
		for(int i=0; i<350; i++) {
			float yAct = (float)i*imgDyn.getHeight()/50.0;
			float xAct = (float)i*imgDyn.getWidth()/50.0;
			ofLine(xAct,0, xAct, imgDyn.getHeight());
			ofLine(0, yAct, imgDyn.getWidth(), yAct);		
		}
		
		ofDisableSmoothing();
//		ofDisableAntiAliasing();
	imgWeb.end();
	ofPopStyle();	

	
	// control fuerza atraccion
	kk = 100.0;
	gg = 10000.0;
	
	masaTUIO = 10000;
	
	nivelNoise = 7;// 60.0;
	noiseAuto = true;
	
	zCam = 446.81;
//	cam.setAutoDistance(false);
	
//	lon = 0;
//	lat = 90;
		

	ratePartic = 30;

    setupGUI();
    gui1->loadSettings("gui1Settings.xml");
	
	sol.init(INIT_MASA_SOL, ofVec3f(0,0,0), ofVec2f(W_WIDTH/2.0, W_HEIGHT/2.0) );
//	initSol(7000);
	
	cam.setDistance(zCam);
}



void testApp::setupMeshSuperf() {

	int skip = 10;///2;	// this controls the resolution of the mesh
	
	superfW = W_WIDTH;
	superfH = W_HEIGHT;
	
	nW = floor(superfW/skip);
	nH = floor(superfH/skip);
	
	//Set up vertices and colors 
	for (int y=0; y<nH; y++) {
		for (int x=0; x<nW; x++) { 
			
			superf.addVertex(ofPoint( (x - nW/2) * skip, (y - nH/2) * skip, 0 ) );
			superf.addColor( ofColor( 255,255,255 ) );

			// add texture points
			float xn = (float)x*skip/superfW;
			float yn = (float)y*skip/superfH;
			superf.addTexCoord(ofVec2f(xn, yn));
			
		}
	}
	
	//Set up triangles' indices 
	for (int y=0; y<nH-1; y++) {
		for (int x=0; x<nW-1; x++) { 
			int i1 = x + nW * y; 
			int i2 = x+1 + nW * y; 
			int i3 = x + nW * (y+1); 
			int i4 = x+1 + nW * (y+1); 
			superf.addTriangle( i1, i2, i3 ); 
			superf.addTriangle( i2, i4, i3 );
		}
	}
	
	setNormals( superf ); //Set normals 
	
	
	// Guardamos una copia del mesh original
	superfOrig = superf;
	
	
	// Material
	mat1.setShininess(64);
	
    mat1Color.setBrightness(250.f);
    mat1Color.setSaturation(200);
	mat1.setAmbientColor(mat1Color);
	
	
}

//--------------------------------------------------------------
void testApp::update(){

	// aplicar noise y deformaciones por sol y hands
	updateMeshSuperf();

	// particulas
	updateParticlesX();
	
	// Sol
	sol.update();
	
	// dibujar particulas en textura
	// preparar mezcla de imagenes
	imgMix.begin();
	{
		ofClear(0);
		//		ofEnableBlendMode( OF_BLENDMODE_ADD );
		ofDisableDepthTest();

		// rejilla
		if(swWeb) {
			imgWeb.draw(0,0);
		}
		else {
			// fondo blanco
			ofSetColor(150);
			ofRect(0,0, imgMix.getWidth(), imgMix.getHeight());
		}
		
		ofEnableDepthTest();
		
		// dibujar particulas y cosas
		updateFBO();
		
		drawParticlesX();
		
	}
	imgMix.end();	


	// eliminar las particulas que ya han chocado contra el sol o que se han ido muy lejos
	
}

void testApp::updateMeshSuperf(){
	
	if(noiseAuto) nivelNoise = ofMap(sol.masa, INIT_MASA_SOL, MAX_MASA_SOL, 0,100);
	
	float time = ofGetElapsedTimef();	//Get time
	
	superf = superfOrig;
	
	for (int y=0; y<nH; y++) {
		for (int x=0; x<nW; x++) { 
			int i = x + nW * y;	//Vertex index 
			ofPoint p = superf.getVertex( i ); 
			ofPoint pO = superfOrig.getVertex( i ); 
			
			//Get Perlin noise value 
			float value = ofNoise( x * 0.05, y * 0.05, time * 0.5 ); 
			
			//Change z-coordinate of vertex
			p.z = -value * nivelNoise; 
			//p.z = -value; //37.0;
			
			//
			// - - - - ATRACCION SOL - - - - 
			//
//			p.z = value * 100; 
			float rho2 = p.x*p.x+p.y*p.y;
			float var = 20+5*sin(time*0.5);
//			float var = 20;
//			p.z += 100.0*exp(-rho2/(2*var*var));
			
			// asignar altura 
			// y color segun la altura
			ofFloatColor cc;
			float rho = sqrt(rho2);
			if(rho>=10) {
//				p.z -= 10000/rho;
				p.z -= gg/rho;
				float ccv = ofMap(p.z,0,-500, 1.0, 0.0);
//				cc = ofFloatColor(ccv,1-ccv,ccv, 1.0);
				cc = ofFloatColor(abs(1-ccv),abs(1-ccv),ccv, 1.0);
				
//				float angDir = atan2(p.y, p.x);	// radianes
//				p.x -= 100*kk / rho * cos(angDir);
//				p.y -= 100*kk / rho * sin(angDir);				
			}
			else {
				p.z -= gg/10;
			}
			
			
			//
			// - - - - - INTERACCIONES - - - - - 
			//
			// Evaluar el desplazamiento por los ptos de interaccion (mouse, TUIO...)
			
			// MOUSE
			// Pasamos las coordenadas del pto de interaccion al marco de referencia de la malla
			ofVec2f ptInteract = ofVec2f(mouseX-zentro.x, mouseY-zentro.y);
			
			float dx = p.x-ptInteract.x;
			float dy = p.y+ptInteract.y;
			
			var = 80;
			
			float rhoZ2 = dx*dx+dy*dy;
			p.z -= 150.0*exp(-rhoZ2/(2*var*var));
			
			float dxO = pO.x-ptInteract.x;
			float dyO = pO.y+ptInteract.y;
			
			float d2 = dxO*dxO+dyO*dyO;
			float angDir = atan2(dyO, dxO);	// radianes
			
			float dd = sqrt(d2);
			if(dd>10) {
				p.x -= kk / sqrt(d2) * cos(angDir);
				p.y -= kk / sqrt(d2) * sin(angDir);
			}
				
			// SET VERTEX
			superf.setVertex( i, p );
			superf.setColor(i, cc);
			//Change color of vertex 
//			mesh.setColor( i, ofColor( value*255, value * 255, 255 ) );
			
		}
	}
	
	setNormals( superf ); // Update normals 
	
}

//void testApp::updateFBO(vector<ofVec2f> ptsInter) {
void testApp::updateFBO() {
	
	ofPushStyle();
	{
		// lineas alrededor de la bola central. 
		// Varian segun el radio que tenga el circulo == radio^2 / dist
		
		ofColor cLin = ofColor(39,81,227);
//		ofLogNotice() << cLin.getBrightness();
//		ofLogNotice() << cLin.getSaturation();
		cLin.setBrightness(255);
		cLin.setSaturation(0);
		ofSetColor(cLin);	// 0x2751E3 (AZUL GUAY)
		ofSetLineWidth(1.0);
		ofNoFill();
		
		float baseSol = sol.radio/10000.0;
		ofPushMatrix();
		
		ofTranslate(imgDyn.getWidth()/2.0, imgDyn.getHeight()/2.0, 0 );
		//		int lim = ofGetHeight()/2.0*floor(
		// L�neas Circulares
		for(float i=0.1; i<10.0; i+=0.3) {
			float vPot = i*baseSol;
			float rPot = 1/vPot;
			
			ofEnableSmoothing();
			ofCircle(0,0, rPot);
			ofDisableSmoothing();
		}
		
		// L�neas Radiales
		for(int i=0; i<16; i++) {
			float ang = (float) TWO_PI/32.0*i;
			float cang = cos(ang)*imgDyn.getWidth();
			float sang = sin(ang)*imgDyn.getWidth();
			ofLine(-cang, -sang, cang, sang);
		}
		
		ofPopMatrix();
		
		
		// lineas alrededor de los pts de interaccion (mouse, TUIOs):	vector<ofVec2f> ptsInter

		
	}
	
	ofPopStyle();
	
}

//--------------------------------------------------------------
void testApp::draw(){

	
// De ejemplo de OF MeshFromCamera
//	float rotateAmount = ofMap(ofGetMouseY(),0,ofGetHeight(), -90,90);
//	ofVec3f rotateDir = ofVec3f(1,0,0);
	float rotateAmount = 0;
	ofVec3f rotateDir = ofVec3f(1,0,0);
	if(ofGetMousePressed(0)) {
	
		rotateAmount = 10; //ofMap(ofGetMouseY(),0,ofGetHeight(), -90,90);
		ofVec3f posTouch = ofVec3f(ofGetMouseX()-zentro.x, ofGetMouseY()-zentro.y, 0.0);
		posTouch.normalize();
		rotateDir = ofVec3f(0,0,1).cross(posTouch);
	}

	float extrusionAmount = 300.0;
	
	// Situar la camara
	ofVec3f camDirection(0,0,1);
	ofVec3f centre = sol.pos3D;
//	ofVec3f camDirectionRotated = camDirection.rotated(rotateAmount, ofVec3f(1,0,0));
	ofVec3f camDirectionRotated = camDirection.rotated(rotateAmount, rotateDir);
	ofVec3f camPosition = centre + camDirectionRotated * extrusionAmount;
	
	camera.setPosition(camPosition);
	camera.lookAt(centre);
	
//	if(swOrtho) {
//		ofEnableDepthTest();
//	}
	
	
	
	
//	cam.begin();
	camera.begin();

//	if(!swOrtho) cam.setDistance(zCam);
//	else	cam.enableOrtho();
	if(swOrtho) camera.enableOrtho();
	else camera.disableOrtho();
		

	ofEnableAlphaBlending();
	ofEnableDepthTest();
	if(swLight) ofEnableLighting();
		
	ofPushMatrix();
	if(swOrtho) {
		ofTranslate(zentro.x, zentro.y,0);
	}
	else {
		ofTranslate(0,0, -260);		
	}
//	ofTranslate(zentro.x, zentro.y,0);

	mat1.begin();
	
	if(bUseTexture) {
		ofEnableNormalizedTexCoords();
		imgMix.getTextureReference().bind();
	}
		
	if(swLight) {
		light.enable();
//		light.draw();
	}
	if(swWireFrame) {
		superf.drawWireframe();
	}
	else {
		superf.draw();
	}
	
	if(bUseTexture) {
		imgMix.getTextureReference().unbind();
		ofDisableNormalizedTexCoords();
	}
	
	// Dibujar bolas de las part�culas:
	ofPushMatrix();
	ofTranslate(-(W_WIDTH)/2.0, -ofGetHeight()/2.0, -60);
//	ofTranslate(-ofGetMouseX(), -ofGetHeight()/2.0, -60);
	for(int i=0; i<particulas.size(); i++) {
		particulas[i].draw3D();		
	}
	ofPopMatrix();
	
	// Dibujar esfera central
	ofDisableAlphaBlending();
	sol.draw();
	ofEnableAlphaBlending();
	
	
	if(swLight) 	light.disable();
	
	mat1.end();	
	
//	if(swOrtho) {
	ofPopMatrix();
//	}
	
	
	ofDisableAlphaBlending();
	if(swLight) ofDisableLighting();
	
//	cam.end();
	camera.end();
//	if(swOrtho) ofDisableDepthTest();

	ofDisableDepthTest();
	
	borde.draw();
	
	
	ofPushStyle();
	ofSetColor(255,0,0);
	ofDrawBitmapString("fov: " + ofToString(camera.getFov()), 10, ofGetHeight()-110);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10, ofGetHeight()-90);
	ofDrawBitmapString("masa Sol: " + ofToString(sol.masa), 10, ofGetHeight()-70);
	ofDrawBitmapString("num Partics: " + ofToString(particulas.size()), 10, ofGetHeight()-50);
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == 's' ){
		gui1->saveSettings("gui1Settings.xml");
	}	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float px = ofMap(x,0.0,ofGetWidth(), -1.0, 1.0);
	float py = ofMap(y,0,W_HEIGHT, -1.0, 1.0);
//    light.setPosition(x-ofGetWidth()/2.0, y-ofGetHeight()/2.0,  ofMap(y,0,ofGetHeight(), -100.0, 100.0));
//    light.setPosition(x-ofGetWidth()/2.0, 0,  ofMap(x,0,ofGetHeight(), -100.0, 100.0));
	
    light.setPosition(0, 0, 300.0*px);
		
//	ofFloatColor ambient_color((py+1.0)/2.0,0.0,1.0, 0.5);
//	light.setAmbientColor(ambient_color);
//	light.setOrientation( ofVec3f(py*180,0, 0) );

	
//    light.setPosition(x-ofGetWidth()/2.0, y,  300);
	
//	ofLogNotice("moiseMoved x:" + ofToString(x)+"  px:" + ofToString(px));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

// - - - - GUI - - - - 
void testApp::setupGUI() {

	gui1 = new ofxUISuperCanvas("GRAVITY F");
    gui1->addSpacer();
//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
//    gui1->addSpacer();
	gui1->addLabel("Noisy waves");
	gui1->addToggle("Noise Auto", &noiseAuto);
	gui1->addSlider("nivel_Noise", 0, 100.0, &nivelNoise);
	
	gui1->addSpacer();
	gui1->addLabel("Sol");
	gui1->addSlider("radio_Sol", 10, 250.0, &sol.radio);
	gui1->addSlider("masa_Sol", 1000, MAX_MASA_SOL*1.5, &sol.masa);
    
	gui1->addSpacer();
	gui1->addSlider("fuerza_Sol", 0, 20000.0, &gg);
	gui1->addSlider("fuerza_User", 0, 1000.0, &kk);
	gui1->addSlider("masa TUIO", 0, 20000.0, &masaTUIO);
	gui1->addSpacer();
	
	gui1->addToggle( "TOGGLE_WireFrame", &swWireFrame);
	gui1->addToggle( "TOGGLE_Light", &swLight);
	gui1->addToggle( "TOGGLE_OrthoCamera", &swOrtho);
	gui1->addToggle( "TOGGLE_Texture", &bUseTexture);
	gui1->addToggle( "TOGGLE_Tex Web", &swWeb);
	
	gui1->addSpacer();
	gui1->addLabel("Camara");
	gui1->addSlider("cam_z", 100, 1000.0, &zCam);

	gui1->addSpacer();
	gui1->addIntSlider("Rate_partics", 1,120, &ratePartic);
	gui1->addSpacer();
	gui1->addLabel("fr: " + ofToString(ofGetFrameRate()));
	
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);	
}

void testApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
//	cout << "got event from: " << name << endl;
	if(name == "radio_Sol")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;		
		sol.setRadio(slider->getScaledValue());
	}
	else if(name == "cam_z")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		zCam = slider->getScaledValue();
		cam.setDistance(zCam);
	}
	else if(name == "TOGGLE_OrthoCamera")
	{	// swOrtho
		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
		swOrtho = toggle->getValue();
		if(swOrtho) {
			cam.enableOrtho();
			camera.enableOrtho();			
		}
		else {
			cam.disableOrtho();
			camera.disableOrtho();
		}
	}
	
}

void testApp::exit()
{
    gui1->saveSettings("gui1Settings.xml");
    
	delete gui1;
	
//    float *buffer;
//    ofImage *img;
	
//	delete[] buffer;
//    delete img;
}



