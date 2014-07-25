#include "facesSlitScan.h"
#include <algorithm>    // std::min

//--------------------------------------------------------------
void facesSlitScan::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetWindowTitle("Faces_SlitScan");

    ofBackground(0);
	
	leer_ImagesNames();
	
	cargarNImages(20);
	
	// fuentes
	fontFutured.loadFont("fonts/Futured.ttf", 30,true,true);
	fontVerdana.loadFont("fonts/verdana.ttf", 14,true,true);
	fontVerdanaPeq.loadFont("fonts/verdana.ttf", 10,true,true);

	// Visor Sequencias
	swSeqActiva = true;
	fr = 1.0;

	wRender = 200;
	hRender = 300;
	
	// SlitScan
	ofLogNotice("setup: llamada a setupSlitScan()");
	setupSlitScan();
	
	bWarpBlending = warp.isBlending();
	
	ofLogNotice("setup: llamada a setupGUI()");
	setupGUI();
	
}

void facesSlitScan::setupGUI() {
    gui = new ofxUICanvas();
    
    gui->addLabel("Faces SlitScan", OFX_UI_FONT_LARGE);

    gui->addToggle("Modo GL", &swmodoGL);
	gui->addToggle("Blending", bWarpBlending);
	
    gui->addToggle("Activar Seq", &swSeqActiva);

	gui->addSlider("FrameRate Sequence", 0.1, 10, fr);
	
	gui->addIntSlider("Map Image", 0, sampleMaps.size()-1, &currentSampleMapIndex);

//    gui->autoSizeToFitWidgets(); 
	
    ofAddListener(gui->newGUIEvent, this, &facesSlitScan::guiEvent); 
//    gui->loadSettings("settings.xml");	
	
	
	gui->getRect()->x = ofGetWidth() - gui->getRect()->getWidth();
}


void facesSlitScan::setupSlitScan() {
	WIDTH_SS = wRender;
	HEIGHT_SS = hRender;
	
	capacity = 12;
	
	setupMaps();
	
	changeCapacity();
	
	warp.setBlending(true);
	warp.setDelayMap(*(sampleMaps[0]));
	
	
	// Imagenes de apoyo
    colorImg.allocate(WIDTH_SS,HEIGHT_SS, OF_IMAGE_COLOR);
	previewImage.allocate(WIDTH_SS, HEIGHT_SS, OF_IMAGE_COLOR);

	// Para meter las ofTextures de la ImageSequence
	warpImage.allocate(WIDTH_SS, HEIGHT_SS, OF_IMAGE_COLOR);
	
	imgSeqPixels.allocate(WIDTH_SS, HEIGHT_SS, OF_IMAGE_COLOR);
	
	
	// - - - GL - - - 
	
	
	ofLogNotice("setupSlitScan: setup SlitScanGL()");
	// SLIT_SCAN_GL
	scanner.setup(WIDTH_SS, HEIGHT_SS);
	scanner.setDelayMap(*(sampleMaps[0]));//->getTextureReference());
	swmodoGL = false;
	
	ofLogNotice("setupSlitScan: allocate fboAux");
	fboAux.allocate(WIDTH_SS, HEIGHT_SS);
	
	fboAux.begin();
		ofClear(0,0,0,255);	
	fboAux.end();

}

void facesSlitScan::setupMaps() {
	// DELAY MAPS 
	string dir = "maps_200x300";
	sampleMapStrings.clear();
	sampleMapStrings.push_back(dir+"/video_delay.png");
	sampleMapStrings.push_back(dir+"/left_to_right.png");
	sampleMapStrings.push_back(dir+"/right_to_left.png");
	sampleMapStrings.push_back(dir+"/up_to_down.png");
	sampleMapStrings.push_back(dir+"/down_to_up.png");
	sampleMapStrings.push_back(dir+"/hard_noise.png");
	sampleMapStrings.push_back(dir+"/soft_noise.png");
	sampleMapStrings.push_back(dir+"/random_grid.png");
	sampleMapStrings.push_back(dir+"/franjas_h2.png");
	sampleMapStrings.push_back(dir+"/franjas_h3.png");
	sampleMapStrings.push_back(dir+"/franjas_h5.png");
	sampleMapStrings.push_back(dir+"/franjas_h29.png");
	sampleMapStrings.push_back(dir+"/franjas_h38.png");
	sampleMapStrings.push_back(dir+"/franjas_v2.png");
	sampleMapStrings.push_back(dir+"/franjas_v3.png");
	sampleMapStrings.push_back(dir+"/franjas_v5.png");
	sampleMapStrings.push_back(dir+"/franjas_v25.png");
	sampleMapStrings.push_back(dir+"/franjas_v34.png");
	
	for(int i = 0; i < sampleMapStrings.size(); i++){
		ofImage* map = new ofImage();
		map->allocate(WIDTH_SS, HEIGHT_SS, OF_IMAGE_GRAYSCALE);
		map->loadImage(sampleMapStrings[i]);
		sampleMaps.push_back( map );
	}
	
	currentSampleMapIndex = 0;
}

void facesSlitScan::changeCapacity()
{
	if(warp.isSetup()){
		ofLogNotice("---changeCapacity: llamada a setCapacity()");
		warp.setCapacity(capacity);
	}
	else{
		ofLogNotice("---changeCapacity: warp no en setup");
		warp.setup(WIDTH_SS, HEIGHT_SS, capacity, OF_IMAGE_COLOR);
		ofLogNotice("---changeCapacity: warp no en setup #### setDelayMap");
		ofLogNotice("paso sampleMap de w,h: "+ofToString(sampleMaps[currentSampleMapIndex]->getWidth()));
		warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);
	}
	float leftClamp = 0.0;
	float rightClamp = 1.0;
	ofLogNotice("---changeCapacity: llamada a setTimeDelayAndWidth()");
	warp.setTimeDelayAndWidth(int( leftClamp * warp.getCapacity() + .5), 
							  int( (rightClamp - leftClamp) * warp.getCapacity() + .5) );

	// Visualizador de strip de fotogramas
//	framesToShow = MIN(warp.getCapacity(), 19);
//	heightPerFrame = 49;
//	widthPerFrame  = 1.0 * WIDTH / HEIGHT * heightPerFrame;
//	filmStripWidth = widthPerFrame*framesToShow;
	
}



void facesSlitScan::leer_ImagesNames() {
	// lista de nombres de im‡genes de c‡mara
	
	swImagesLoaded = false;
	
    ofDirectory dir;
    string nmDirectorio = "images/caras";
    int nFiles = dir.listDir(nmDirectorio);
    if(nFiles) {
        for(int i=0; i<dir.numFiles(); i++) { 
            // Guardamos los nombres de todas las im‡genes disponibles
			string filePath = dir.getPath(i);
			imagesNames.push_back(filePath);
        }
        swImagesLoaded = true;
    } 
    else printf("Could not find folder\n");

	ofLogNotice("Num de Imagenes de Caras: " + ofToString(nFiles) + " cargadas: " + ofToString(imagesNames.size()) );

}

void facesSlitScan::cargarNImages(int nn) {
	ofLogNotice("cargarNImages");

	// Seleccionar nn imagenes
	vector<string> seleccionNames;
	
	// Ordenar aleatoriamnte la lista de nombres
	// y coger para cargar las nn primeras
	random_shuffle(imagesNames.begin(), imagesNames.end());
	
	int nTot = MIN(nn, imagesNames.size());
	ofLogNotice("cargarNImages. nTot: "+ofToString(nTot));
	for(int i=0; i<nTot; i++) {
		seleccionNames.push_back(imagesNames[i]);
	}

	// Descargar el imgSequence
	sequenceActiva.unloadSequence();	

	// Cargar el imgSequence
	sequenceActiva.loadSequence_Files(seleccionNames);
//	sequenceActiva.preloadAllFrames();	//this way there is no stutter when loading frames
	sequenceActiva.setFrameRate(fr);
	
	swSeqActiva = true;	
}



//--------------------------------------------------------------
void facesSlitScan::update(){
	ofBackground(0x333333);
	
	if(swSeqActiva) {
		// Aqui hay que meter mas logica
		// pero por ahora a–ado siempre el frame de la sequencia activa
		
		
		// I - Obtener una ofImage de la ofTextura de la imgSequence
		sequenceActiva.getFrameForTime(ofGetElapsedTimef())->readToPixels(imgSeqPixels);
		warpImage.setFromPixels(imgSeqPixels);
		if(warpImage.width != warp.getWidth() || warpImage.height != warp.getHeight()) {
			
			if(!swmodoGL) {
				warpImage.resize(warp.getWidth(), warp.getHeight());
				warp.addImage( warpImage );
			}
			else {
				
				// Test para evitar resize de images, pero aqui hay que utilizar un fboTmp
				ofFbo fboTmp;
				fboTmp.allocate(scanner.getWidth(), scanner.getHeight());
				fboTmp.begin();
					warpImage.draw(0,0, fboTmp.getWidth(), fboTmp.getHeight());
				fboTmp.end();
				
				//			scanner.addFrameTex( sequenceActiva.getTextureReference() );
				scanner.addFrameTex( fboTmp.getTextureReference() );
				//ofLogNotice("num Frames: " + ofToString(scanner.getNumFrames()));
				scanner.update();
			}
		}
		else {
			if(!swmodoGL) {
				warp.addImage( warpImage );
			}
			else {
				// II - La opci—n de funcionamiento m‡s r‡pido ser’a hacer un SlitScan que trabaje en GPU (shader sencillo de mezcla)
				scanner.addFrameTex( sequenceActiva.getTextureReference() );
	//			scanner.addFrameTex( warpImage.getTextureReference() );
				//ofLogNotice("num Frames: " + ofToString(scanner.getNumFrames()));
				scanner.update();
			}
		}
	}
}

//--------------------------------------------------------------
void facesSlitScan::draw()
{

	// GUI
	// 
	// Elementos a mostrar:
	// - numero de imagenes total
	// - info de alarmas y camaras
	// - numero de imagenes seleccionadas
	// - secuencia animada de todas las imagenes en la seleccion
	// - tiras con la seleccion: imagen+info time. Ancho de 200px.
	// - GUI interaccion: teclas para la seleccion
	
//	ofSetWindowTitle("ImgSequence+SlitScan á fr:"+ofToString(ofGetFrameRate()));
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	if(swSeqActiva) {
		sequenceActiva.getFrameForTime(ofGetElapsedTimef())->draw(10, ofGetHeight()/2);
	}
	
	// Panel de	visor de SlitScan (warp, mapa, secuencia de SlitScan)
	// Posicionar la imagen slitScan
	float margen = 10.0;
	// ofRectangle rectSlit = ofRectangle(panelSlitScan.x+(panelSlitScan.width-WIDTH_SS)/2.0,
	ofRectangle rectSlit = ofRectangle(margen,
									   margen,
									   WIDTH_SS, HEIGHT_SS);
		
		
	ofSetHexColor(0xffffff);
	if(!swmodoGL) {
		warp.getOutputImage().draw(rectSlit.x, rectSlit.y, rectSlit.width, rectSlit.height);
	}
	else {
		scanner.getOutputTex().draw(rectSlit.x, rectSlit.y, rectSlit.width, rectSlit.height);
	}
				
	ofNoFill();
	ofSetHexColor(0xcccccc);
	ofRect(rectSlit);

	
	// Info del slitScan
	fontVerdanaPeq.drawString("Capacity: "+ofToString(warp.getCapacity()), rectSlit.x+10, rectSlit.y+rectSlit.height+25);
	fontVerdanaPeq.drawString("FrameRate App: "+ofToString(ofGetFrameRate()), rectSlit.x+10, rectSlit.y+rectSlit.height+25+20);
	fontVerdanaPeq.drawString("Blend (b): "+ofToString(warp.isBlending()), rectSlit.x+10, rectSlit.y+rectSlit.height+25+20*2);
	fontVerdanaPeq.drawString("GLMode (g): "+ofToString(swmodoGL), rectSlit.x+10, rectSlit.y+rectSlit.height+25+20*3);
	
	
	
	// Posicionar la imagen map
	float wwMap = WIDTH_SS;
	float hhMap = HEIGHT_SS;		
	ofRectangle rectMap = ofRectangle(rectSlit.x+rectSlit.width+margen, rectSlit.y,
									  wwMap, hhMap);			
	
//			warp.getDelayMap().draw(rectMap);
	if(!swmodoGL) 			warp.getDelayMap().getTextureReference().draw(rectMap);
	else					scanner.getDelayMapTex().draw(rectMap);
	ofDrawBitmapString("DelayMap Activo", rectMap.x, rectMap.y+rectMap.height+15);

	ofNoFill();
	ofSetHexColor(0xcccccc);
	ofRect(rectMap);
	
	
	// dibujar el GUI a un lado
	// ### mal: no responde automaticamente a la posicion del mouse
//	ofTranslate(rectMap.x + rectMap.width + 10 , rectMap.y);
	
}

//--------------------------------------------------------------
void facesSlitScan::keyPressed(int key){

}



//--------------------------------------------------------------
void facesSlitScan::keyReleased(int key){

}

//--------------------------------------------------------------
void facesSlitScan::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void facesSlitScan::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void facesSlitScan::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void facesSlitScan::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void facesSlitScan::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void facesSlitScan::guiEvent(ofxUIEventArgs &e)
{
    if(e.getName() == "FrameRate Sequence")
    {
        ofxUISlider *slider = e.getSlider();
		fr = slider->getScaledValue();
		sequenceActiva.setFrameRate(fr);
    }
	else if(e.getName() == "Map Image") {
//		ofLogNotice("Map Image");
 //       ofxUISlider *slider = e.getSlider();
		warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);			
		scanner.setDelayMap(sampleMaps[currentSampleMapIndex]->getTextureReference());
	}
		
	
    else if(e.getName() == "Blending")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		warp.toggleBlending();
		bWarpBlending = warp.isBlending();
    }
    else if(e.getName() == "FULLLSCREEN")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		ofSetFullscreen(toggle->getValue());   
	}
	
}

void facesSlitScan::exit()
{
    gui->saveSettings("guiSettings.xml");
    
	delete gui;
//	delete[] buffer;
//    delete img;
}


