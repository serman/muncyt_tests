#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(34, 34, 34);
	ofSetVerticalSync(false);
	
	ofEnableAlphaBlending();
	
	zentro = ofVec2f(ofGetWidth()/2.0, W_HEIGHT/2.0);
	
	imgTex.loadImage("imagen.jpg");
	bUseTexture = false;
	
	//we load a font and tell OF to make outlines so we can draw it as GL shapes rather than textures
	font.loadFont("type/verdana.ttf", 100, true, false, true, 0.4, 72);
	
	// Shader Setup
	#ifdef TARGET_OPENGLES
		shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
	#else
	if(ofGetGLProgrammableRenderer()){
		shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
	}else{
		shader.load("shaders/noise.vert", "shaders/noise.frag");
	}
	#endif

	doShader = true;	
	
	if(true) {
				
		// Meshes
		// puntos en cuadr’cula
		setupMesh();
		
		// superficie
		setupMeshSuperf();
		
		swWireFrame = false;
		
		// Light
		light.setDirectional();
		light.setPosition(0,0,50);
		light.setOrientation( ofVec3f(180,0, 0) );
		
		ofFloatColor ambient_color(0.0,0.0,0.0,1.0);
		ofFloatColor diffuse_color(1.0,0.0,0.0, 0.2);
		ofFloatColor specular_color(1.0,1.0,1.0, 0.1);
	//	ofFloatColor specular_color(1.0,0.0,0.0, 0.5);
		
	//	light.setAmbientColor(ambient_color);
	//	light.setDiffuseColor(diffuse_color);
	//	light.setSpecularColor(specular_color);
		
		swLight = true;
		light.enable();
		
		swOrtho = false;
	}
	
	
	float wFBOs = ofGetHeight();
	
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

	imgWeb.begin();
		ofSetColor(255,255,255);
//		ofEnableAntiAliasing();
	ofEnableSmoothing();
		ofSetLineWidth(1.5);
		
		for(int i=0; i<50; i++) {
			float yAct = (float)i*imgDyn.getHeight()/50.0;
			float xAct = (float)i*imgDyn.getWidth()/50.0;
//			cout << xAct << "," << yAct << "\n";
			ofLine(xAct,0, xAct, imgDyn.getHeight());
			ofLine(0, yAct, imgDyn.getWidth(), yAct);		
		}
		
//		ofSetColor(100,200,100, 80);
//		ofFill();
//		ofRect(50,50,400,400);
	ofDisableSmoothing();
//		ofDisableAntiAliasing();
	imgWeb.end();
	ofPopStyle();	

	// control fuerza atraccion
	kk = 100.0;
	
	rSol = 20.0;
	sol.setRadius(rSol);
	
	nivelNoise = 60.0;
	
	zCam = 446.81;
//	cam.setAutoDistance(false);
	
    setupGUI();
    gui1->loadSettings("guiSettings.xml");
	
	cam.setDistance(zCam);
}




void testApp::setupMesh() {
	//void fGravedad::setupMesh(float rrMin, float rrMax, int nnRads, int nnAngs) {
			
	float ww = ofGetWidth();
	float hh = ofGetHeight();
	int nnW = 180;
	int nnH = 60;
	
	float dhh = hh/nnH;
	float dww = ww/nnW;
	
	espacio.clear();
	espacio.setMode(OF_PRIMITIVE_POINTS);
	for(int jr=0; jr<=nnH; jr++) {
		float hh = 0+dhh*jr;
		for(int i=0; i<nnW; i++) {
			float xx =	0+i*dww;
//			espacio.addVertex(ofVec3f(r*cos(ang), r*sin(ang),0.0));
			espacio.addVertex(ofVec3f(xx, hh, 0.0));
//			espacio.addColor(ofFloatColor(123.0/255.0, 1.0, 1.0, 1.0));
			espacio.addColor(ofColor(255.0, 255.0, 255.0, 200.0));
		}
	}
	
	// Add indexes para ordenar el modo de dibujo
//	espacio.setMode(OF_PRIMITIVE_LINES);
//	for(int j=0; j<=nnH; j++) {
//		for(int i=0; i<nnW; i++) {
//			int loc = nnH*j + i;
//			int locNxt = (i<nnH-1)? (loc+1) : nnH*j;
//			
//			espacio.addIndex(loc);
//			espacio.addIndex(locNxt);
//			if(j<nnW) {
//				espacio.addIndex(loc);
//				espacio.addIndex(loc+nnH);
//			}
//		}
//		//		espacio.addIndex(nAngs*j);
//		//		espacio.addIndex(nAngs*(j+1));
//	}	
	
	
}


void testApp::setupMeshSuperf() {

	int skip = 10/2;	// this controls the resolution of the mesh
	
	superfW = 600;
	superfH = 600;
	
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
	
	
}

//--------------------------------------------------------------
void testApp::update(){

	updateMeshSuperf();
	
//	updateFBO();
	
}

void testApp::updateMeshSuperf(){
//	ofLogNotice("updateMeshSuperf");
	
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
			p.z = 0;
			p.z -= value * nivelNoise; 
			
			
			// ATRACCION PARTICULA CENTRAL
			
//			p.z = value * 100; 
			float rho2 = p.x*p.x+p.y*p.y;
			float var = 20+10*sin(time*0.5);
//			p.z += 100.0*exp(-rho2/(2*var*var));
			
			ofFloatColor cc;
			
			float rho = sqrt(rho2);
			if(rho>=10) {
				p.z -= 10000/rho;
				float ccv = ofMap(p.z,0,-500, 1.0, 0.0);
//				cc = ofFloatColor(ccv,1-ccv,ccv, 1.0);
				cc = ofFloatColor(ccv,ccv,ccv, 1.0);
//				cc = ofFloatColor(1.0,1.0,1.0,1.0);
			}
			else {
				p.z -= 1000;
			}
			
			
			// Evaluar el desplazamiento por los ptos de interaccion (mouse, TUIO...)
			
			// MOUSE
			ofVec2f ptInteract = ofVec2f(mouseX-ofGetWidth()/2.0, mouseY-ofGetHeight()/2.0);
			
			float dx = p.x-ptInteract.x;
			float dy = p.y+ptInteract.y;
			
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
//	imgDyn.begin();
	{
//		ofClear(0,0,0, 0);
		
		
		// lineas alrededor de la bola central. 
		// Varian segun el radio que tenga el circulo == radio^2 / dist
		
		ofSetColor(255,0,0);
		ofSetLineWidth(1.0);
		ofNoFill();
		
		float baseSol = rSol/10000.0;
		ofPushMatrix();
		
		ofTranslate(imgDyn.getWidth()/2.0, imgDyn.getHeight()/2.0, 0 );
		//		int lim = ofGetHeight()/2.0*floor(
		for(float i=0.1; i<10.0; i+=0.3) {
			float vPot = i*baseSol;
			float rPot = 1/vPot;
			
			ofEnableSmoothing();
			ofCircle(0,0, rPot);
			ofDisableSmoothing();
			
		}
		
		
		// lineas alrededor de los pts de interaccion (mouse, TUIOs):	vector<ofVec2f> ptsInter
		//		for(int i=0; i<10; i++) {
		//			
		//			
		//		}
		ofPopMatrix();
	}
//	imgDyn.end();
	
	ofPopStyle();
	
}

//--------------------------------------------------------------
void testApp::draw(){


	if(true) {
		ofSetColor(245, 58, 135);
		ofFill();

		if( doShader ){
			shader.begin();
			//we want to pass in some varrying values to animate our type / color 
			shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
			shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
			
			//we also pass in the mouse position 
			//we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped. 
			shader.setUniform2f("mouse", mouseX - ofGetWidth()/2, ofGetHeight()/2-mouseY );

		}
		espacio.draw();
			//finally draw our text
		//font.drawStringAsShapes("openFrameworks", 90, 260);
		if( doShader ){
			shader.end();
		}
		espacio.draw();	
	}
	
	
	
	if(false) {
		
		// preparar mezcla de imagenes
		imgMix.begin();
			ofClear(0);

			
//			ofEnableBlendMode( OF_BLENDMODE_ADD );
		ofDisableDepthTest();
			imgWeb.draw(0,0);
//			imgDyn.draw(0,0);
		
		ofPushStyle();
		ofSetColor(0,255,0);
		ofFill();
		ofEllipse(200,200, 100,100);
		ofPopStyle();
		ofEnableDepthTest();
		
			updateFBO();
//			ofDisableBlendMode();
		imgMix.end();
		
//		ofLogNotice("***"+ofToString(superf.getMode()));

//		imgWeb.draw(ofGetWidth()-200,0,200,200);
//		imgDyn.draw(ofGetWidth()-200,300,200,200);
//		imgMix.draw(ofGetWidth()-200,ofGetHeight()-200,200,200);
		
		cam.begin();
//			if(!swOrtho) cam.setDistance(zCam);
		//	cam.enableOrtho();
			ofEnableAlphaBlending();
			if(swLight) ofEnableLighting();
			ofEnableDepthTest();
			
			mat1.begin();
			
			if(swOrtho) ofTranslate(ofGetWidth()/2.0,ofGetHeight()/2.0,0);
			
			//img.bind();
		if(bUseTexture) {
			ofEnableNormalizedTexCoords();
//			imgWeb.getTextureReference().bind();
//			imgDyn.getTextureReference().bind();
			imgMix.getTextureReference().bind();
		}
			
		if(swLight) {
			light.enable();
			light.draw();
		}
		if(swWireFrame) {
			superf.drawWireframe();
		}
		else {
			superf.draw();
		}
		//img.unbind();
		if(bUseTexture) {
//				imgWeb.getTextureReference().unbind();
//				imgDyn.getTextureReference().unbind();
			imgMix.getTextureReference().unbind();
			ofDisableNormalizedTexCoords();
		}
		
		
//		superfOrig.drawWireframe();
		
		// Dibujar esfera central
//			ofSphere(0,0, radioEsfera);	// deprecated
		ofPushStyle();
			ofSetColor(255,255,0,  80+120*ofNoise(ofGetElapsedTimef() / 10.0) );
			sol.setPosition(0,0,0);
			ofPushMatrix();
		ofRotateX(90);
			sol.draw();
			ofPopMatrix();
		ofPopStyle();
		
		if(swLight) 	light.disable();
		
		mat1.end();
		
		ofDisableAlphaBlending();
		if(swLight) ofDisableLighting();
		
		cam.end();

	}
	
//	imgWeb.draw(ofGetWidth()-200,0,200,200);
//	imgDyn.draw(ofGetWidth()-200,300,200,200);
//	imgMix.draw(ofGetWidth()-200,ofGetHeight()-200,200,200);
	
	
	ofPushStyle();
	ofSetColor(255);
	ofDrawBitmapString("fr: " + ofToString(ofGetFrameRate()), 10, ofGetHeight()-30);
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == 's' ){
		gui1->saveSettings("gui1Settings.xml");
	}	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float px = ofMap(x,0.0,ofGetWidth(), -1.0, 1.0);
	float py = ofMap(y,0,ofGetHeight(), -1.0, 1.0);
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

// - - - - GUI - - - - 
void testApp::setupGUI() {

	gui1 = new ofxUISuperCanvas("GRAVITY F");
    gui1->addSpacer();
//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
    
//    gui1->addSpacer();
	gui1->addLabel("Noisy waves");
	gui1->addSlider("nivel_Noise", 0, 100.0, &nivelNoise);
	gui1->addLabel("Sol");
	gui1->addSlider("radio_Sol", 10, 250.0, &rSol);
    
	gui1->addSpacer();
	gui1->addSlider("fuerza_Grav", 0, 1000.0, &kk);
	gui1->addSpacer();
	
	gui1->addToggle( "TOGGLE_Shader", &doShader);
	gui1->addToggle( "TOGGLE_WireFrame", &swWireFrame);
	gui1->addToggle( "TOGGLE_Light", &swLight);
	gui1->addToggle( "TOGGLE_OrthoCamera", &swOrtho);
	gui1->addToggle( "TOGGLE_Texture", &bUseTexture);
	
	gui1->addSpacer();
	gui1->addLabel("Camara");
	gui1->addSlider("cam_z", 100, 1000.0, &zCam);

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
		rSol = slider->getScaledValue();
		sol.setRadius(rSol);
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
		if(swOrtho) cam.enableOrtho();
		else cam.disableOrtho();
	}
	
	
	
	//    else if(name == "nivel_Noise")
//    {
//		ofxUISlider *slider = (ofxUISlider *) e.getSlider();
//		nivelNoise = slider->getNormalizedValue();
//    }
	//	else if(name == "TOGGLE")
//	{
//		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
//		bdrawGrid = toggle->getValue();
//        if(textInput != NULL)
//        {
//            textInput->setFocus(bdrawGrid);
//        }
//	}
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



