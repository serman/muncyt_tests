#include "fGravedad.h"

//--------------------------------------------------------------
void fGravedad::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0x000000);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableAntiAliasing();
	
	ofEnableNormalizedTexCoords();

	isKeyPressed = false;
	isMousePressed = false;
	
	bola.loadImage("images/dot.png");
	ladoPart1 = 5;
	ladoPart2 = 10;
	
	coloresBN.push_back(ofColor(0));
	coloresBN.push_back(ofColor(150));
	coloresBN.push_back(ofColor(255));
	
	bDrawWF = false;
	bDrawMeshLines = false;
	
	// setup Espacio
	rMin = 200.0;
	rMax = ofGetHeight()/2.0*0.95;
	nRads = 3;//10+1;	// Num de franjas circulares
	float difAngDeg = 5.0;
	nAngs = floor(360.0/difAngDeg);
	

	setupBorde();
	
	centro = ofVec2f(ofGetWidth()/2,ofGetHeight()/2);
	
	bLights = false;
	luz.setDirectional();
	
    setupGUI();
    gui->loadSettings("guiSettings.xml");	

	setupMesh(rMin, rMax, nRads, nAngs);
	
}

void fGravedad::setupMesh(float rrMin, float rrMax, int nnRads, int nnAngs) {
	
	drr = (rrMax-rrMin)/nnRads;
	
	dAngRads = (float)TWO_PI/nnAngs;
	dAngRadsDeg = RAD_TO_DEG*dAngRads;
		
	espacio.clear();
	espacio.setMode(OF_PRIMITIVE_POINTS);
	for(int jr=0; jr<=nnRads; jr++) {
		float r = rrMin+drr*jr;
		for(int i=0; i<nnAngs; i++) {
			float ang =	i*dAngRads;
			espacio.addVertex(ofVec3f(r*cos(ang), r*sin(ang),0.0));
			//			espacio.addColor(ofFloatColor(123.0/255.0, 1.0, 1.0, 1.0));
//			espacio.addColor(ofColor(123.0, 255.0, 255.0, 100.0));
			espacio.addColor(ofColor(255.0, 255.0, 255.0, 100.0));
		}
	}
	
	// Add indexes para ordenar el modo de dibujo
	// Modo lineas:
//	espacio.setMode(OF_PRIMITIVE_LINES);
//	for(int j=0; j<=nnRads; j++) {
//		for(int i=0; i<nnAngs; i++) {
//			int loc = nnAngs*j + i;
//			int locNxt = (i<nAngs-1)? (loc+1) : nAngs*j;
//			
//			espacio.addIndex(loc);
//			espacio.addIndex(locNxt);
//			if(j<nnRads) {
//				espacio.addIndex(loc);
//				espacio.addIndex(loc+nAngs);
//			}
//		}
//		//		espacio.addIndex(nAngs*j);
//		//		espacio.addIndex(nAngs*(j+1));
//	}	
	
	// Modo Triangulos:
	for (int y=0; y<nnRads-1; y++) {
		for (int x=0; x<nnAngs-1; x++) { 
			int i1 = x + (nnAngs) * y;			// loc
			int i2 = x+1 + (nnAngs) * y;		// loc +1
			int i3 = x + (nnAngs) * (y+1);		// loc + N*y 
			int i4 = x+1 + (nnAngs) * (y+1);	// loc +1 + N*y
			espacio.addTriangle( i1, i2, i3 ); 
			espacio.addTriangle( i2, i4, i3 );
		}
		// cierre del circulo
		int i1 = (nnAngs-1) + (nnAngs) * y; 
		int i2 = 0 + (nnAngs) * y; 
		int i3 = i1 + nnAngs ; 
		int i4 = i2 + nnAngs ; 
		espacio.addTriangle( i1, i2, i3 ); 
		espacio.addTriangle( i2, i4, i3 );
		
	}
	
	setNormals( espacio ); //Set normals 	
}

void fGravedad::setupBorde() {
	borde.clear();
	ofColor ctmp = ofColor::black;
	borde.setFillColor(ctmp);
	//http://www.openframeworks.cc/documentation/graphics/ofPath.html#show_setPolyWindingMode
	borde.setPolyWindingMode(OF_POLY_WINDING_ODD);
	// rectangulo 
	borde.rectangle(0,0,ofGetWidth(),ofGetHeight());
	borde.setCircleResolution(60);
	borde.circle(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/2*0.95);
}


void fGravedad::updateMesh(){
	
	float time = ofGetElapsedTimef();	//Get time
	//Change vertices 
	//	float zMin = 99999;
	//	float zMax = -99999;
	for (int y=0; y<nRads; y++) {
		for (int x=0; x<nAngs; x++) { 
			int i = x + nAngs * y;	//Vertex index 
			ofPoint p = espacio.getVertex( i ); 
			//Get Perlin noise value 
			float value = ofNoise( x * 0.05, y * 0.05, time * 0.5 ); 
			//Change z-coordinate of vertex
			p.z = 0;
			p.z -= value * 30; 
			
			
			// Central Hole
			//			p.z = value * 100; 
			float rho2 = p.x*p.x+p.y*p.y;
			float var = 20+10*sin(time*0.5);
			//			p.z += 100.0*exp(-rho2/(2*var*var));
			
			ofFloatColor cc;
			
			float rho = sqrt(rho2);
			if(rho>=10) {
				p.z -= 10000/rho;
//				float ccv = ofMap(p.z,0,-500, 1.0, 0.0);
//				cc = ofFloatColor(ccv,1-ccv,ccv, 1.0);
				float ccv = ofMap(p.z,30,-500, 1.0, 0.0);
				cc = ofFloatColor(ccv,ccv,ccv, 1.0);
				//				cc = ofFloatColor(1.0,1.0,1.0,1.0);
			}
			else {
				p.z -= 1000;
				cc = ofFloatColor(0,0,0, 1.0);
			}
			
			float dx = p.x-(mouseX-ofGetWidth()/2.0);
			float dy = p.y+(mouseY-ofGetHeight()/2.0);
			
			rho2 = dx*dx+dy*dy;
			p.z -= 150.0*exp(-rho2/(2*var*var));
			
			espacio.setVertex( i, p );
			espacio.setColor(i, cc);
			//Change color of vertex 
			//			mesh.setColor( i, ofColor( value*255, value * 255, 255 ) );
			
			//			if(p.z<zMin) zMin = p.z;
			//			if(p.z>zMax) zMax = p.z;
		}
	}
	
	setNormals( espacio ); // Update normals 
	
	
	
}
			  


//--------------------------------------------------------------
void fGravedad::update() {
	
	idsPtosInteract.clear();
	
	updateMesh();

	
}

void fGravedad::interact(ofVec2f pt) {
	
	// hallar el angulo
	float angDeg = pt.angle(ofVec2f(1.0,0.0));
	float angRad = angDeg*DEG_TO_RAD;
	
	// hallar el radio
	ofVec2f ptCentro = pt - centro;
	
	float rho = ptCentro.length();
	
	// seleccionar los puntos mas cercanos
	int idRad = floor((rho-rMin)/drr);
	int idAng = floor(angRad/dAngRads);
	
	int loc1 = idRad*nAngs+idAng;
	idsPtosInteract.push_back(loc1);
//	loc1 = (idRad<nRads)? 
//	idsPtosInteract.push_back(loc1);
	
}



//--------------------------------------------------------------
void fGravedad::draw() {
	ofSetBackgroundColor(ofColor::black);
	
// Esto igual hace falta
// glEnable(GL_DEPTH_TEST);
// ofEnableDepthTest();
	ofEnableDepthTest();	//Enable z-buffering

	
//	if(bDrawMeshLines) espacio.setMode(OF_PRIMITIVE_LINES);//TRIANGLE_STRIP);
	if(bDrawMeshLines) espacio.setMode(OF_PRIMITIVE_TRIANGLES);
	else			espacio.setMode(OF_PRIMITIVE_POINTS);
//	ofEnableAlphaBlending();
	
	ofPushMatrix();
	cam.begin();
	if(bLights) {
		ofEnableLighting();
		luz.enable();
		luz.draw();
	}
//	ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0, 0.0);
	if(bColorMesh) {
		espacio.enableColors();
	}
	else {
		espacio.disableColors();
		ofSetColor(123.0, 0,0, alphaMesh);
	}
	
	ofNoFill();
	if(bDrawWF) espacio.drawWireframe();
	else		espacio.draw();

	if(bLights) {
		ofDisableLighting();
	}
	
	// Referencia: 
	// dibujar c’rculo azul en el centro del plano XY y a z=0
	ofPushStyle();
	ofNoFill();
	ofSetColor(ofColor::blue, 100);
	ofSetLineWidth(2.0);
	ofCircle(0,0,rMin);
	ofSetColor(ofColor::red, 100);
	ofCircle(0,0,rMax);
	ofPopStyle();
	
	cam.end();
	ofPopMatrix();
	
	ofDisableAlphaBlending();

	
	
	borde.draw();
	
	
	
	string info = "";
//	info += "Press [c] for circles\n";
	info += "(w) draw wireframe: "+ofToString(bDrawWF)+"\n";
	info += "(m) draw triang?: "+ofToString(bDrawMeshLines)+"\n";
	info += "(i) save Screen\n";
//	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
//	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x6EFFAD);
	ofDrawBitmapString(info, ofGetWidth()-180, 30);
	
	
	
}



//--------------------------------------------------------------
void fGravedad::keyPressed(int key) {
	
	if(key == 'c') {
		//bAddCircle=true;
	}
	
	if(key == 'f') ofToggleFullscreen();
	
	else if(key=='w') bDrawWF=!bDrawWF;
	else if(key=='m') bDrawMeshLines = !bDrawMeshLines;
	
	else if(key=='i') {
//		ofSaveFrame();

		string nmImg = "";
		nmImg += ofToString(ofGetYear())+ofToString(ofGetMonth(),2,'0')+ofToString(ofGetDay(),2,'0')+
				 ofToString(ofGetHours(),2,'0')+ofToString(ofGetMinutes(),2,'0');
		nmImg += "_fGravedad.png";
		ofSaveScreen(nmImg);
	}
	
	isKeyPressed = true;
}

//--------------------------------------------------------------
void fGravedad::keyReleased(int key) {
	isKeyPressed = false;
}

//--------------------------------------------------------------
void fGravedad::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void fGravedad::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void fGravedad::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

//--------------------------------------------------------------
void fGravedad::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

//--------------------------------------------------------------
void fGravedad::resized(int w, int h){
	setupBorde();	
}


void fGravedad::exit()
{
    gui->saveSettings("guiSettings.xml");
	delete gui;
//	delete[] buffer;
//    delete img;
}


// - - - - - GUI - - - - -

void fGravedad::setupGUI()
{
//    vector<string> names;
//	names.push_back("RAD1");
//	names.push_back("RAD2");
//	names.push_back("RAD3");
	
	gui = new ofxUISuperCanvas("Gravity Settings");
    gui->addSpacer();
    gui->addLabel("Malla", OFX_UI_FONT_SMALL);
    gui->addSpacer();
	gui->addLabel("grid Radios");
    gui->addSpacer();
	gui->addRangeSlider("radios", 0.0, ofGetHeight()/2.0, &rMin, &rMax);
    gui->addSpacer();	
    gui->addSpacer();
	gui->addLabel("grid Divisiones");
    gui->addSpacer();
	gui->addIntSlider("DivsRad", 2, 36, &nRads);
	gui->addIntSlider("DivsAng", 2, 180, &nAngs);
    gui->addSpacer();
	gui->addSpacer();
    gui->addLabel("Draw");
	gui->addSpacer();
	gui->addToggle("Lighting", &bLights);	
	gui->addSpacer();
	gui->addToggle("Draw WireFrame", &bDrawWF);
	gui->addToggle("Draw Lines", &bDrawMeshLines);
	gui->addSpacer();
	gui->addLabel("Color Mesh");
    gui->addSpacer();
	gui->addToggle("use Color Mesh", &bColorMesh);
	gui->addSlider("Alpha Manual", 0, 255.0, &alphaMesh);

	
//    gui->addSpacer();
//	gui->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL); //OFX_UI_ORIENTATION_VERTICAL)
    
//    gui->addSpacer();
//    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
//	gui->addButton("BUTTON", false);
//	gui->addToggle( "TOGGLE", false);
    
    gui->autoSizeToFitWidgets();
	ofAddListener(gui->newGUIEvent,this,&fGravedad::guiEvent);
}


void fGravedad::guiEvent(ofxUIEventArgs &e)
{
	string name = e.getName();
	int kind = e.getKind();


//	if(name == "BUTTON")
//	{
//		ofxUIButton *button = (ofxUIButton *) e.getButton();
//		bdrawGrid = button->getValue();
//	}
//	else if(name == "TOGGLE")
//	{
//		ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
//		bdrawGrid = toggle->getValue();
//        if(textInput != NULL)
//        {
//            textInput->setFocus(bdrawGrid);
//        }
//	}
//    else if(name == "RADIO VERTICAL")
//    {
//        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
//        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl; 
//    }
	
	if(name=="DivsRad") {
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		nRads = slider->getValue();
		setupMesh(rMin, rMax, nRads, nAngs);
		
	}
	else if(name=="DivsAng") {
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		nAngs = slider->getValue();
		setupMesh(rMin, rMax, nRads, nAngs);
	}
	else if(name=="radios") {
		ofxUIRangeSlider *rgSlider = (ofxUIRangeSlider *) e.widget;
		cout << "rMin: " << rgSlider->getValueLow() << endl;
		cout << "rMax: " << rgSlider->getValueHigh() << endl;
		rMin = rgSlider->getValueLow();
		rMax = rgSlider->getValueHigh();
//		float getScaledValueLow();
//		float getScaledValueHigh();
		setupMesh(rMin, rMax, nRads, nAngs);
	}
}



