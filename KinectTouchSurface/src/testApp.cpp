#include "testApp.h"


using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void testApp::setup() {

	//Depth camera setup
	
	/// print the device list
	kinect.listDevices();
	// enable depth->video image calibration
	kinect.setRegistration(true);
	kinect.init(false,true);  // no-IR, si-RGB
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();		// opens first available kinect
	
	if(kinect.isConnected()) {
		ofLogNotice() << "Kinect Connected!!!" << endl;
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}	
	
	// zero the tilt on startup (modelos  <= 1414)
	float angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	wk = kinect.getWidth();
	hk = kinect.getHeight();
	ofLogNotice() << "Dims kinet: " << wk << "," << hk;
	
	modo = 0;

	// MODO 0
	//Set up drawing variables
	W = wk; //1024;					//Desired screen size
	H = hk; //768;
	outPixels.allocate(		W, H, OF_IMAGE_GRAYSCALE );
	
	calibrating = true;		//Set calibrating mode at start
	shift = ofPoint( 320, 20 );	//The value of shifting
	//camera's image from the corner of the screen
	
	rectCalib = ofRectangle(shift.x, shift.y, wk, hk);
	ofLogNotice("rectCalib: " + ofToString(rectCalib));
	
	minV = 30;		//Minimal distance in mm
	maxV = 150; 		//Maximal distance in mm	

	
	
	// 
	// MODO 1
	// 
	// imagenes openCV para blob_substraction
	grayImage.allocate(wk, hk);
	grayThreshNear.allocate(wk, hk);
	grayThreshFar.allocate(wk, hk);
	
	colorImg.allocate(wk, hk, OF_IMAGE_COLOR);  
	tempGrayImage.allocate(wk, hk);  

	// backgroundImage
	backgroundImage.allocate(wk, hk);  
	
	thresholdLow = 20;
	thresholdHigh = 200;
	threshold = 100;
	bLearnBakground = true;
	blurcv = 1;
	
	// ContourFinder
	max_blobs = 20;
	min_blob_size = 100;
	max_blob_size = 50;	// % del area de kinect
	bFindHoles = false;
	
	// warp
	entrada[0] = ofPoint(0,0);
	entrada[1] = ofPoint(wk,0);
	entrada[2] = ofPoint(wk,hk);
	entrada[3] = ofPoint(0,hk);
	
	destino_camera_warp[0] = ofPoint(0,0);
	destino_camera_warp[1] = ofPoint(wk,0);
	destino_camera_warp[2] = ofPoint(wk,hk);
	destino_camera_warp[3] = ofPoint(0,hk);
	
	// calibr centros
	// habra que a–adir algo para cargar los valores
	bSetZentros = false;
	bZentro = false;
	bZentroDepth = false;
	
	wk1 = 320;// 400;
	hk1 = 240;// 300;

	rect_RGB = ofRectangle(shift.x, shift.y, wk1, hk1);
	rect_Depth = ofRectangle(shift.x+wk1+10, shift.y, wk1, hk1);
	rect_Bckgnd = ofRectangle(shift.x,shift.y+hk1+20, wk1, hk1);
	rect_Blobs = ofRectangle(shift.x+wk1+10, shift.y+hk1+20, wk1, hk1);
	
	// 
	// BT BlobTracker
	// wait for half a frame before forgetting something
	contourFinderX.getTracker().setPersistence(15);
	// an object can move up to 32 pixels per frame
	contourFinderX.getTracker().setMaximumDistance(32);
	
	
	bWarp = true;
	
	setupGUI();
	
	gui1->loadSettings("gui1Settings.xml");
	
	bLearnBakground = true;
	
	contourFinderX.setMinAreaRadius((float)min_blob_size);
	contourFinderX.setMaxAreaRadius((float)max_blob_size*wk/100.0);
	contourFinderX.setFindHoles(bFindHoles);
	
	bt = true;
	
	// TUIO
	
	myTUIO.setup("localhost",3333);
	
	
	bPeque = false;
	
}

//--------------------------------------------------------------
void testApp::update(){

	kinect.update();		//Update depth camera
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		if(modo==0) {
			updateModo0();
		}
		else if(modo==1) {
			updateModo1();
		}
	}
}
	


void testApp::updateModo0() {
	// Cargar imagen en ShriPixels
	
	// restar background: backgroundDepth
	
	
	
	// Check zero values
	
	
	// l’mite min-max
	
	
	
	
	
	if ( !calibrating && corners.size() == 4 ) {
		//Analyze depth
		
		//Get current depth image
		//		ofShortPixels &input = openNIDevice.getDepthRawPixels();
		ofShortPixels &input = kinect.getRawDepthPixelsRef();
		
		//Process pixels
		int w = input.getWidth();
		int h = input.getHeight();
		for (int Y=0; Y<H; Y++) {
			for (int X=0; X<W; X++) {
				//Process pixel (X, Y)
				
				//Transform screen coordinates (X, Y) 
				//to depth image coordinates (x, y)
				float a = float(X) / W;
				float b = float(Y) / H;
				ofPoint p =
				(1-a) * (1-b) * corners[0]
				+ a * (1-b) * corners[1]
				+ a * b * corners[2]
				+ (1-a) * b * corners[3];
				
				int x = int( p.x );
				int y = int( p.y );
				
				if ( x >= 0 && x < w && y >= 0 && y < h ) {
					//Getting depth values
					int inputZ = input.getPixels()[ x + w * y ];	
					int backgroundZ = backgroundDepth.getPixels()[ x + w * y ];
					
					int delta;
					//Check for zero values - it means that depth camera
					//does not compute distance in the pixel
					if ( inputZ != 0 && backgroundZ != 0 ) {
						delta = backgroundZ - inputZ;
					}
					else {
						delta = -1;
					}
					//Output value
					if ( ofInRange( delta, minV, maxV ) ) {	
						int value = ofMap( delta, minV, maxV, 0, 255, true );
						outPixels.getPixels()[ X + W * Y ] = value;
					}
					else
						outPixels.getPixels()[ X + W * Y ] = 0;
						
						}
				
				
			}
		}
		outImage.setFromPixels( outPixels );
	}
}

void testApp::updateModo1() {
	// Leer Camara
	// load grayscale depth image from the kinect source
	//		grayImage.setFromPixels(kinect.getDepthPixels(), wk, hk);
	//		colorImg.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());  
	colorImg.setFromPixels(kinect.getPixelsRef()); //.getDepthPixels());  
	tempGrayImage.setFromPixels(kinect.getDepthPixelsRef()); //.getDepthPixels());		// GrayScale
	//		tempGrayImage.setFromPixels(kinect.getRawDepthPixelsRef()); //.getDepthPixels());  // Short
	//		monoPixels.setFromPixels(tempGrayImage.getPixels(), 320,240, 1);  
	
	if (bLearnBakground == true){  
		//			backgroundImage.setFromPixels(tempGrayImage.getPixels(), 320,240);	// solo vale para ver la bckgrnd image
		backgroundImage = tempGrayImage;// solo vale para ver la bckgrnd image
		bLearnBakground = false;  
	}  
	
	// substraci—n openCV.
	//
	tempGrayImage-=backgroundImage;
	
	grayThreshNear = tempGrayImage;
	grayThreshFar  = tempGrayImage;
	grayThreshNear.threshold(thresholdHigh, true);
	grayThreshFar.threshold(thresholdLow);
	cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), tempGrayImage.getCvImage(), NULL);
	
	// update the cv images
	tempGrayImage.flagImageChanged();
	
	// WARP
	if(bWarp) 	grayImage.warpIntoMe(tempGrayImage, entrada, destino_camera_warp);  
	else		grayImage = tempGrayImage;


	// NO SE SI VA MEJOR EL THREHOLD ANTES O DESPUES DEL BLUR
	if(bt) grayImage.threshold(threshold);  
	grayImage.blur(blurcv);  
	
	contourFinderX.setThreshold(threshold);	
	contourFinderX.findContours(grayImage);
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground( 0 );	//Set white background
	ofSetColor( 255, 255, 255 );

	if(modo==0) {
		if ( calibrating ) {	

			//Draw color and depth image
			ofSetColor( 255, 255, 255 );
			kinect.draw( shift.x, shift.y );
			kinect.drawDepth( rectCalib.x+rectCalib.width-wk/2, rectCalib.y+rectCalib.height+10, wk/2, hk/2 );

			//Draw corners
			ofSetColor( 255, 0, 0 );
			ofFill();
			int n = corners.size();
			for (int i=0; i<n; i++) {
				ofCircle( corners[i] + shift, 10 );
			}
			if ( n == 4 ) {
				for (int i=0; i<n; i++) {
					ofLine( corners[i] + shift,
						corners[(i+1)%n] + shift );
				}
			}
		}
		else {
			ofBackground( 0 );	//Set white background
			//Show current drawing
			ofSetColor( 255, 255, 255 );
			outImage.draw( 0, 0 );
		}
	}
	else if(modo==1) {
		// draw from the live kinect
		// RGB
		colorImg.draw(rect_RGB);
//		kinect.draw( shift.x, shift.y );
		drawMarco(rect_RGB);
		ofDrawBitmapString("RGB Kinect", rect_RGB.x, rect_RGB.y+hk1+10);
		
		// Depth
		kinect.drawDepth(rect_Depth);
		drawMarco(rect_Depth);
		ofDrawBitmapString("DEPTH Kinect", rect_Depth.x, rect_Depth.y+hk1+10);
		
		// background
		backgroundImage.draw(rect_Bckgnd);
		drawMarco(rect_Bckgnd);
		ofDrawBitmapString("Background (b)", rect_Bckgnd.x,rect_Bckgnd.y+hk1+10);

		// Contour Finder
		grayImage.draw(rect_Blobs);
		grayImage.draw(rect_Blobs);
		drawBlobs(wk1, hk1);
		drawMarco(rect_Blobs);
		ofDrawBitmapString("DEPTH Kinect", rect_Blobs.x, rect_Blobs.y+hk1+10);
	
		
		// Zentros
		ofPushStyle();
		if(bZentro) {
			ofPushMatrix();
			ofTranslate(rect_RGB.x, rect_RGB.y, 0);
			ofSetColor(ofColor::magenta);
			ofNoFill();
			ofEllipse(zentro.x*rect_RGB.width, zentro.y*rect_RGB.height, 5,5);
			ofPopMatrix();
		}
		if(bZentroDepth) {
			ofPushMatrix();
			ofTranslate(rect_Depth.x, rect_Depth.y, 0);
			ofSetColor(ofColor::magenta);
			ofNoFill();
			ofEllipse(zentroDepth.x*rect_Depth.width, zentroDepth.y*rect_Depth.height, 5,5);
			ofPopMatrix();
			
			// por ahora tambien lo dibujo en la zona de blobs, pero habra que hacerlo warped
			ofPushMatrix();
			ofTranslate(rect_Blobs.x, rect_Blobs.y, 0);
			ofSetColor(ofColor::magenta);
			ofNoFill();
			ofEllipse(zentroDst.x*rect_Blobs.width, zentroDst.y*rect_Blobs.height, 5,5);
			ofPopMatrix();
			
			
		}
		ofPopStyle();
	}
	
	else if(modo==2) {
		// MODO CALIBRACION
		
		
	}
	
	ofSetColor(255);
	ofDrawBitmapString("fr: " +ofToString(ofGetFrameRate()), 10,ofGetHeight()-20);
	
}

void testApp::drawBlobs(int wk1, int hk1) {

	// preparar vector de BlobX
	blobs.clear();
	
	// tracker
	RectTracker& tracker = contourFinderX.getTracker();
	
	ofPushMatrix();
	ofTranslate(rect_Blobs.x, rect_Blobs.y, 0);
	ofScale((float)wk1/wk, (float)hk1/hk, 1);
	contourFinderX.draw();
	
	int n = contourFinderX.size();
	for(int i=0; i<n; i++) {
		// smallest rectangle that fits the contour
		ofSetColor(cyanPrint);
		ofPolyline minAreRect = toOf(contourFinderX.getMinAreaRect(i));
		minAreRect.draw();
		
		// ellipse that best fits the contour
		ofSetColor(magentaPrint);
		cv::RotatedRect ellipse = contourFinderX.getFitEllipse(i);
		ofPushMatrix();
		ofVec2f ellipseCenter = toOf(ellipse.center);
		ofVec2f ellipseSize = toOf(ellipse.size);
		ofTranslate(ellipseCenter.x, ellipseCenter.y);
		ofRotate(ellipse.angle);
		ofNoFill();
		ofEllipse(0, 0, ellipseSize.x, ellipseSize.y);
		ofPopMatrix();
		
		// convex hull of the contour
		ofSetColor(yellowPrint);
		ofPolyline convexHull = toOf(contourFinderX.getConvexHull(i));
		convexHull.draw();		
		
		
		// Label
		ofPoint center = toOf(contourFinderX.getCenter(i));
		ofPushMatrix();
		ofSetColor(255,0,0);
		ofTranslate(center.x, center.y);
		int label = contourFinderX.getLabel(i);
		string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
		ofDrawBitmapString(msg, 0, 0);		
		ofPopMatrix();
		
		// preparar vector de BlobX
		if(bTUIO) {
//		int label;
//		ofVec2f pos;
//		ofVec2f vel;	// ult desplazamiento
//		float   maccel;	// veloc = despl/dt
//		ofVec2f	boundingRect;		
		
			BlobX blobTmp;
			blobTmp.label = contourFinderX.getLabel(i);	// Por ahora env’o el centroid
			ofVec2f posB = toOf(contourFinderX.getCenter(i));
			posB.x/=wk;
			posB.y/=hk;
			blobTmp.pos = posB;	// Por ahora env’o el centroid
			ofVec2f velB = toOf(contourFinderX.getVelocity(i));
			velB.x/=wk;
			velB.y/=hk;
			blobTmp.vel = velB;	// Por ahora env’o el centroid
			blobTmp.maccel = blobTmp.vel.length();	// Aœn puedo dividir para dt (millis)
			
			blobs.push_back(blobTmp);
		}
	}
	
	// enviar TUIO con los blobs
	if(bTUIO) 	myTUIO.sendTUIO(blobs);
	
	ofPopMatrix();	
}

void testApp::drawMarco(ofRectangle rect) {
	drawMarco(rect.x, rect.y, rect.width, rect.height);
}

void testApp::drawMarco(float xx, float yy, float ww, float hh) {
	ofPushStyle();
	ofSetColor(0,0,150);
	ofNoFill();
	ofRect(xx,yy,ww,hh);
	ofPopStyle();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' ') {
		bPeque=!bPeque;
		
		if(bPeque) {
			// hacer ventana peque–a
			
			
		}
		else {
			// ventana grande
			
		}
		
	}
	if ( key == 'm' ) {
		calibrating = !calibrating;
		if ( !calibrating ) {	//Store background
//			backgroundDepth = openNIDevice.getDepthRawPixels();
			backgroundDepth = kinect.getRawDepthPixelsRef();
		}
	}
	else if(key=='b') {
//			backgroundDepth = openNIDevice.getDepthRawPixels();
		backgroundDepth = kinect.getRawDepthPixelsRef();
		bLearnBakground = true;
	}
	else if(key=='c') {
		calibrating = true;
		corners.clear();
	}
	
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


	if(modo==0) {
		if ( calibrating && corners.size() < 4 ) {
			if(rectCalib.inside(x,y) ) {
				corners.push_back( ofPoint( x, y ) - shift );
			}
		}
	}
	else if(modo==1 && bSetZentros) {
		if(rect_RGB.inside(x,y)) {
			zentro = ofVec2f((x-rect_RGB.x)/rect_RGB.width, (y-rect_RGB.y)/rect_RGB.height);
			bZentro = true;
		}
		if(rect_Depth.inside(x,y)) {
			zentroDepth = ofVec2f((x-rect_Depth.x)/rect_Depth.width, (y-rect_Depth.y)/rect_Depth.height);
			bZentroDepth = true;
			
			// por ahora tambien lo dibujo en la zona de blobs, pero habra que hacerlo warped
			zentroDst = zentroDepth;
		}
		
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


// - - - - GUI - - - - 
void testApp::setupGUI() {
	
	gui1 = new ofxUISuperCanvas("Surface Painting");
    gui1->addSpacer();
	//    gui1->addLabel("Press 'h' to Hide GUIs", OFX_UI_FONT_SMALL);
	gui1->addLabel("SELECTOR MODO");
	gui1->addIntSlider("Modo", 0, 2, &modo);
    
	//    gui1->addSpacer();
	gui1->addLabel("MODO 0 - BASICO");
	gui1->addLabel("Rango Valores");
	gui1->addRangeSlider("Rango", 10.0, 200.0, &minV, &maxV);
	
	gui1->addSpacer();
	gui1->addToggle("(m) Modo Calibracion", &calibrating );
    gui1->addButton("(c) Reset Calibration", false);
    gui1->addButton("(b) Background", false);

	gui1->addSpacer();
	
	
	gui1->addLabel("MODO 1 - Background Substraction");
	gui1->addToggle("(b) Set Bckgrnd", &bLearnBakground);
	
	gui1->addRangeSlider("rango resta imagenes L/H", 0.0, 20.0, &thresholdLow, &thresholdHigh);
	gui1->addSlider("contraste", 0.0, 255.0, &threshold);
	gui1->addIntSlider("blur", 0, 11, &blurcv);

	gui1->addLabel("Contour Finder");
	gui1->addSlider("blob_min_radio", 0.0, 200, &min_blob_size);
	gui1->addSlider("blob_max_radio_%W", 0.0, 100.0, &max_blob_size);
	gui1->addIntSlider("max_blobs", 0, 20, &max_blobs);
	gui1->addToggle("FindHoles", &bFindHoles);
	gui1->addSpacer();
	gui1->addToggle("bt", &bt);
	gui1->addToggle("doWarp", &bWarp);
	
	gui1->addSpacer();
	gui1->addToggle("set zentros", &bSetZentros);
	gui1->addSpacer();
	gui1->addToggle("Send TUIOs", &bTUIO);
	
	
	
    gui1->autoSizeToFitWidgets();
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);	
}

void testApp::guiEvent(ofxUIEventArgs &e) {
	string name = e.getName();
	int kind = e.getKind();
	//	cout << "got event from: " << name << endl;
	if(name == "(m) Modo Calibracion")
	{
		if ( !calibrating ) {	// Store background
//			backgroundDepth = openNIDevice.getDepthRawPixels();
			backgroundDepth = kinect.getRawDepthPixelsRef();
		}		
	}
	else if(name == "(c) Reset Calibration")
	{
		calibrating = true;
		corners.clear();
	}
	else if(name == "(b) Background")
	{
//			backgroundDepth = openNIDevice.getDepthRawPixels();
		backgroundDepth = kinect.getRawDepthPixelsRef();
	}
	else if(name == "blob_min_radio") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		min_blob_size = slider->getScaledValue();
		contourFinderX.setMinAreaRadius((float)min_blob_size);
	}	
	else if(name == "blob_max_radio_%W") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		max_blob_size = slider->getScaledValue();
		contourFinderX.setMaxAreaRadius((float)max_blob_size*wk/100.0);
	}
	
//	contourFinderX.setMinArea(min_blob_size);
//	contourFinderX.setFindHoles(bFindHoles);
	
}


void testApp::exit()
{
	
//	openNIDevice.stop();
	kinect.close();
	
	gui1->saveSettings("gui1Settings.xml");    
	delete gui1;
	
	//    float *buffer;
	//    ofImage *img;
	
	//	delete[] buffer;
	//    delete img;
	
	// borrar
	// - emitters
	// - particles
	
}




