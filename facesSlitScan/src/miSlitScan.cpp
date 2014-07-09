#include "miSlitScan.h"

//--------------------------------------------------------------
void miSlitScan::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetWindowTitle("Mi SlitScan");

    ofBackground(0);
	
	leer_savedImagesNames();
	
	// fuentes
	fontFutured.loadFont("fonts/Futured.ttf", 30,true,true);
	fontVerdana.loadFont("fonts/verdana.ttf", 14,true,true);
	fontVerdanaPeq.loadFont("fonts/verdana.ttf", 10,true,true);
	
	// GUI
	setupPanelesGUI();

	// Visor Sequencias
	swSeqActiva = false;
	fr = 22.0;

	wRender = 640;
	hRender = 480;
	
	// SlitScan
	setupSlitScan();
	
	
	
}

void miSlitScan::setupSlitScan() {
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
	
	
	// SLIT_SCAN_GL
	scanner.setup(WIDTH_SS, HEIGHT_SS);
	scanner.setDelayMap(*(sampleMaps[0]));//->getTextureReference());
	swmodoGL = true;
	
	fboAux.allocate(WIDTH_SS, HEIGHT_SS);
	
	fboAux.begin();
		ofClear(0,0,0,255);	
	fboAux.end();

}

void miSlitScan::setupMaps() {
	// DELAY MAPS 
	sampleMapStrings.clear();
	sampleMapStrings.push_back("maps/video_delay.png");
	sampleMapStrings.push_back("maps/left_to_right.png");
	sampleMapStrings.push_back("maps/right_to_left.png");
	sampleMapStrings.push_back("maps/up_to_down.png");
	sampleMapStrings.push_back("maps/down_to_up.png");
	sampleMapStrings.push_back("maps/hard_noise.png");
	sampleMapStrings.push_back("maps/soft_noise.png");
	sampleMapStrings.push_back("maps/random_grid.png");
	sampleMapStrings.push_back("maps/franjas_h2.png");
	sampleMapStrings.push_back("maps/franjas_h3.png");
	sampleMapStrings.push_back("maps/franjas_h5.png");
	sampleMapStrings.push_back("maps/franjas_h29.png");
	sampleMapStrings.push_back("maps/franjas_h38.png");
	sampleMapStrings.push_back("maps/franjas_v2.png");
	sampleMapStrings.push_back("maps/franjas_v3.png");
	sampleMapStrings.push_back("maps/franjas_v5.png");
	sampleMapStrings.push_back("maps/franjas_v25.png");
	sampleMapStrings.push_back("maps/franjas_v34.png");
	
	for(int i = 0; i < sampleMapStrings.size(); i++){
		ofImage* map = new ofImage();
		map->allocate(WIDTH_SS, HEIGHT_SS, OF_IMAGE_GRAYSCALE);
		map->loadImage(sampleMapStrings[i]);
		sampleMaps.push_back( map );
	}
	
	currentSampleMapIndex = 0;
}

void miSlitScan::changeCapacity()
{
	if(warp.isSetup()){
		warp.setCapacity(capacity);
	}
	else{
		warp.setup(WIDTH_SS, HEIGHT_SS, capacity, OF_IMAGE_COLOR);
		warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);
	}
	float leftClamp = 0.0;
	float rightClamp = 1.0;
	warp.setTimeDelayAndWidth(int( leftClamp * warp.getCapacity() + .5), 
							  int( (rightClamp - leftClamp) * warp.getCapacity() + .5) );

	// Visualizador de strip de fotogramas
//	framesToShow = MIN(warp.getCapacity(), 19);
//	heightPerFrame = 49;
//	widthPerFrame  = 1.0 * WIDTH / HEIGHT * heightPerFrame;
//	filmStripWidth = widthPerFrame*framesToShow;
	
	
}



void miSlitScan::leer_savedImagesNames() {
	// lista de nombres de im‡genes de c‡mara
	//		cam_N_alarm_M_YYYYMMDDHHMMSS.jpg
	//		N: id de camara
	//		M: id de alarma
	// vector <string> savedImagesNames;
	
	swImagesLoaded = false;
	
    ofDirectory dir;
    string nmDirectorio = "images/camaras";
    int nFiles = dir.listDir(nmDirectorio);
	
    if(nFiles) {
        for(int i=0; i<dir.numFiles(); i++) { 
            // Guardamos los nombres de todas las im‡genes disponibles
			string filePath = dir.getPath(i);
			savedImagesNames.push_back(filePath);
        }
        swImagesLoaded = true;
    } 
    else printf("Could not find folder\n");

	// Calcular estadisticas
	stats_savedImages();
	
	// Crear botones GUI
	float wPanel = 300.0;
	float margen = 10.0;
	float wElem = 28.0;
	float hElem = 20.0;
	float sepElemX = 4.0;	
	float sepElemY = 2.0;	
	crearBotones(wPanel, margen, wElem, hElem, sepElemX, sepElemY);
	
}

void miSlitScan::stats_savedImages() {
	// obtener los diferentes 
	// -ids de camaras
	// -ids de alarmas (cada alarma lleva asociada una sola camara)
	
	int t0 = ofGetElapsedTimeMillis();
	
	if(swImagesLoaded) {
		
		// Vectores para acumular los resultados. Esto sepodr‡ meter en un struct global.
		idsAlarms.clear();
		statsAlarms.clear();
		idsCams.clear();
		statsCams.clear();
		
		for(int i=0; i<savedImagesNames.size(); i++) {
			
			// Quitar el nombre del directorio
			vector<string> fileNameParts = ofSplitString(savedImagesNames[i], "/", true, true);
			string fileName = fileNameParts[fileNameParts.size()-1];
			vector<string> imgnameSplit = ofSplitString(fileName, "_", true, true);
//			if(imgnameSplit[0]=="cam" && imgnameSplit[2]=="alarm") {
			if(imgnameSplit[0]=="cam") {
				// Acumular numeros de cam y de alarm
				int ncam   = ofToInt(imgnameSplit[1]);
				int nalarm = 0;// ofToInt(imgnameSplit[3]);
				
				// unsigned int	ofFind(<#const vector *values#>, <#const T target#>);
				// - si no esta en el vector, devuelve el numero de objetos que hay en el vector
				// - si esta en el vector, devuelve le indice de la posicion en la que esta
				int posLista;
				
				posLista = ofFind(idsCams, ncam);
				if(posLista==idsCams.size()) {
					// No esta en la lista: A–adirlo
					idsCams.push_back(ncam);
					statsCams.push_back(1);
				}
				else {
					statsCams[posLista]++;
				}

				posLista = ofFind(idsAlarms, nalarm);
				if(posLista==idsAlarms.size()) {
					// No esta en la lista: A–adirlo
					idsAlarms.push_back(nalarm);
					statsAlarms.push_back(1);
				}
				else {
					statsAlarms[posLista]++;
				}				
				
			}
			else ofLogVerbose("MAL " + ofToString(i)+ " " + savedImagesNames[i]);
			
		}

		// Informe por consola
		ofLogVerbose("* * * * * * * * * * * * * * * * * * * *");
		ofLogVerbose("TOTAL IMAGES");
		ofLogVerbose("Total de imagenes cargadas: "+ofToString(savedImagesNames.size()));
		ofLogVerbose("");
		
		ofLogVerbose("CAMS STATS");
		ofLogVerbose("#Total Cams: "+ofToString(idsCams.size()));
		for(int i=0; i<idsCams.size(); i++) {
			ofLogVerbose("cam id: " + ofToString(idsCams[i])+" --> " + ofToString(statsCams[i]));
		}
		
		ofLogVerbose("ALARMS STATS");
		ofLogVerbose("#Total Alarms: "+ofToString(idsAlarms.size()));
		for(int i=0; i<idsAlarms.size(); i++) {
			ofLogVerbose("alarm id: " + ofToString(idsAlarms[i])+" --> " + ofToString(statsAlarms[i]));
		}
		
	}
	else {
		ofLogVerbose("**** stats_savedImages Error. No hay cargadas referencias a imagenes");
	}

	ofLogVerbose("* * * * * * * * * * * * * * * * * * * *");
	
	int t1 = ofGetElapsedTimeMillis()-t0;
	ofLogVerbose("Tiempo: " + ofToString(t1) );

}

void miSlitScan::crearBotones(float wPanel, float margen, float wElem, float hElem, float sepElemX, float sepElemY) {
	// genera los botones de seleccion de Camara y de Alarma. 
	
	// Camaras
	btnIdCamaras.clear();
	
	int nElemsXLine = floor((wPanel-2*margen) / (wElem+sepElemX));
	for(int i=0; i<idsCams.size(); i++) {
		int pX = margen + (i%nElemsXLine)*(wElem+sepElemX) - sepElemX;
		int pY = margen + floor(i/nElemsXLine)*(hElem+sepElemY) - sepElemY;
		ofRectangle rr = ofRectangle(pX, pY, wElem, hElem);
		
		RectBtnID btnid;
		btnid.rect = rr;
		btnid.id = idsCams[i];
		btnid.colorRect = ofColor::fromHex(0x006633);
		btnid.colorFont = ofColor::fromHex(0xFFFFFF);
		
		btnIdCamaras.push_back(btnid);
		
	}
	
	
	// Alarmas
	btnIdAlarmas.clear();
	for(int i=0; i<idsAlarms.size(); i++) {
		int pX = margen + (i%nElemsXLine)*(wElem+sepElemX) - sepElemX;
		int pY = margen + floor(i/nElemsXLine)*(hElem+sepElemY) - sepElemY;
		ofRectangle rr = ofRectangle(pX, pY, wElem, hElem);

		RectBtnID btnid;
		btnid.rect = rr;
		btnid.id = idsAlarms[i];
		btnid.colorRect = ofColor::fromHex(0x006633);
		btnid.colorFont = ofColor::fromHex(0xFFFFFF);
		
		btnIdAlarmas.push_back(btnid);
	}	
	
}

void miSlitScan::setupPanelesGUI() {

	float margen = 10.0;
	float margenInterno = 5.0;
	float anchoAlarmas = 300.0;
	string tituloTxt = "Mi SlitScan";
	ofRectangle bounds = fontFutured.getStringBoundingBox(tituloTxt, 0, 0);
	
	float ultX = margen+margen;
	float ultY = 0;
	
	panelTitulo = ofRectangle(margen,margen, ofGetWidth()-20, (bounds.height+15+15));

	// LATERAL con stats y seleccion de camaras o alarmas
	ultY = panelTitulo.y+panelTitulo.height+ margenInterno;
	float altoBloque = (ofGetHeight()-ultY-margen)*1/3.0;
	
	panelCamaras = ofRectangle(margen,ultY,anchoAlarmas,altoBloque);
	
	ultY=panelCamaras.y+panelCamaras.height;
	panelAlarmas  = ofRectangle(panelCamaras.x, ultY, panelCamaras.width, altoBloque);// ofGetHeight()-ultY-margen);
	
	panelVisorSeq = ofRectangle(panelAlarmas.x, panelAlarmas.y+panelAlarmas.height,
								panelAlarmas.width, 
								altoBloque);
								//ofGetHeight()-(panelAlarmas.y+panelAlarmas.height+margenInterno));
	
	panelSlitScan = ofRectangle(panelAlarmas.x+panelAlarmas.width+margenInterno, panelTitulo.y+panelTitulo.height+margenInterno,
								ofGetWidth()-margen-(panelAlarmas.x+panelAlarmas.width)-margenInterno, 
								ofGetHeight()-margen-margenInterno-panelTitulo.y-panelTitulo.height);
}

void miSlitScan::cargarImagesSeleccion() {

}

void miSlitScan::cargarImagesCamara(int idcam) {

	// Seleccionar las imagenes
	vector<string> seleccionNames;
	
	for(int i=0; i<savedImagesNames.size(); i++) {
		// Quitar el nombre del directorio
		vector<string> fileNameParts = ofSplitString(savedImagesNames[i], "/", true, true);
		string fileName = fileNameParts[fileNameParts.size()-1];
		vector<string> imgnameSplit = ofSplitString(fileName, "_", true, true);
//		if(imgnameSplit[0]=="cam" && imgnameSplit[2]=="alarm") {
		if(imgnameSplit[0]=="cam") {
			// Acumular numeros de cam y de alarm
			int ncam   = ofToInt(imgnameSplit[1]);
//			int nalarm = ofToInt(imgnameSplit[3]);
			if(ncam==idcam) {	// si esta en la lista add a seleccionNames
				seleccionNames.push_back(savedImagesNames[i]);
			}
		}
	}

	// Descargar el imgSequence
	sequenceCam.unloadSequence();	

	// Cargar el imgSequence
	sequenceCam.loadSequence_Files(seleccionNames);
//	sequenceCam.preloadAllFrames();	//this way there is no stutter when loading frames
	sequenceCam.setFrameRate(fr);
	
	swSeqActiva = true;	
}

void miSlitScan::cargarImagesAlarma(int idalarm) {

	// Seleccionar las imagenes
	vector<string> seleccionNames;
	
	for(int i=0; i<savedImagesNames.size(); i++) {
		// Quitar el nombre del directorio
		vector<string> fileNameParts = ofSplitString(savedImagesNames[i], "/", true, true);
		string fileName = fileNameParts[fileNameParts.size()-1];
		vector<string> imgnameSplit = ofSplitString(fileName, "_", true, true);
		if(imgnameSplit[0]=="cam" && imgnameSplit[2]=="alarm") {
			// Acumular numeros de cam y de alarm
			int ncam   = ofToInt(imgnameSplit[1]);
			int nalarm = ofToInt(imgnameSplit[3]);
			if(nalarm==idalarm) {	// si esta en la lista add a seleccionNames
				seleccionNames.push_back(savedImagesNames[i]);
			}
		}
	}

	// Descargar el imgSequence
	sequenceCam.unloadSequence();	

	// Cargar el imgSequence
	sequenceCam.loadSequence_Files(seleccionNames);
//	sequenceCam.preloadAllFrames();	//this way there is no stutter when loading frames
	sequenceCam.setFrameRate(fr);
	
	swSeqActiva = true;	
	
}


//--------------------------------------------------------------
void miSlitScan::update(){
	ofBackground(0x333333);
	
	if(swSeqActiva) {
		// Aqui hay que meter mas logica
		// pero por ahora a–ado siempre el frame de la sequencia activa
		
		
		// I - Obtener una ofImage de la ofTextura de la imgSequence
		sequenceCam.getFrameForTime(ofGetElapsedTimef())->readToPixels(imgSeqPixels);
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
				
				//			scanner.addFrameTex( sequenceCam.getTextureReference() );
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
				scanner.addFrameTex( sequenceCam.getTextureReference() );
	//			scanner.addFrameTex( warpImage.getTextureReference() );
				//ofLogNotice("num Frames: " + ofToString(scanner.getNumFrames()));
				scanner.update();
			}
		}
	}
}

//--------------------------------------------------------------
void miSlitScan::draw()
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

	// TITULO
	float ultX = 10+10;
	float ultY = 0;
	ofPushStyle();
		string tituloTxt = "Mi SlitScan";
		ofRectangle bounds = fontFutured.getStringBoundingBox(tituloTxt, 0, 0);
		ofSetHexColor(0x3333cc);
		ofFill();
		ofRect(panelTitulo);
		// ofRect(10,10, ofGetWidth()-20, bounds.height+15+15);
		ofSetHexColor(0xffffff);
		fontFutured.drawString(tituloTxt, (ofGetWidth()-bounds.width)/2.0, 10+15+bounds.height);
	ofPopStyle();
	ultY += 10 + bounds.height+15+15 + 5;
	
	// LATERAL con stats y seleccion de camaras o alarmas
	ofRectangle panel = ofRectangle(10,ultY,300,ofGetHeight()-ultY-10);
	panelCamaras = ofRectangle(10,ultY,300,(ofGetHeight()-ultY-10)/2.0);
	ofPushStyle();
		// Camaras
		ofSetHexColor(0x3333cc);
		ofFill();
		ofRect(panelCamaras);
		ultY=panelCamaras.y+25;
		ofSetHexColor(0xffffff);
		fontVerdana.drawString("CAMARAS", ultX, ultY);
		ofPushMatrix();
			ofTranslate(panelCamaras.x, ultY, 0);
			for(int i=0; i<btnIdCamaras.size(); i++) {
				btnIdCamaras[i].draw(&fontVerdanaPeq);
			}
		ofPopMatrix();
	
		// Alarmas
		ofSetHexColor(0x3333cc);
		ofFill();
		ofRect(panelAlarmas);
		ultY=panelAlarmas.y+25;
		ofSetHexColor(0xffffff);
		fontVerdana.drawString("ALARMAS", ultX, ultY);
		ofPushMatrix();
			ofTranslate(panel.x, ultY, 0);
			for(int i=0; i<btnIdAlarmas.size(); i++) {
				btnIdAlarmas[i].draw(&fontVerdanaPeq);
			}
		ofPopMatrix();
	
		// visor de sequencias
		ofPushStyle();
			ofSetHexColor(0x3333cc);
			ofFill();
			ofRect(panelVisorSeq);
	
			ultY=panelVisorSeq.y+25;
			ofSetHexColor(0xffffff);
			fontVerdana.drawString("VISOR DE SECUENCIAS", ultX, ultY);

//			fontVerdanaPeq.drawString("seq fr: " + ofToString(sequenceCam.getFrameRate()), ultX, ultY+15);
//			string infoSeq = "seq. #images: " + ofToString( sequenceCam.getNumTextures() ) + "  - fr: " + ofToString(fr);

//	int ffr = sequenceCam.getFrameRate();
		string infoSeq = "seq. #images: " + ofToString( sequenceCam.getNumTextures() )
						+ "  - fr: " + ofToString(fr) + "/" + ofToString( sequenceCam.getFrameRate() );
			fontVerdanaPeq.drawString(infoSeq, ultX, panelVisorSeq.y);
	
			if(swSeqActiva) {
				sequenceCam.getFrameForTime(ofGetElapsedTimef())->draw(panelVisorSeq.x+10, panelVisorSeq.y+10,
																	   panelVisorSeq.width-20,
																	   panelVisorSeq.height-20
																	   );
			}
	
	
	
		ofPopStyle();
	
		// Panel de	visor de SlitScan (warp, mapa, secuencia de SlitScan)
		ofPushStyle();
		{
			//ofSetHexColor(0x0000EE);
			ofSetHexColor(0x3333cc);
			ofFill();
			ofRect(panelSlitScan); 
			
			
			// Posicionar la imagen slitScan
			float margen = 10.0;
			// ofRectangle rectSlit = ofRectangle(panelSlitScan.x+(panelSlitScan.width-WIDTH_SS)/2.0,
			ofRectangle rectSlit = ofRectangle(panelSlitScan.x+margen,
											   panelSlitScan.y+margen,
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
			float wwMap = panelSlitScan.width-(rectSlit.width+margen+margen+margen);
			float hhMap = float(HEIGHT_SS)/WIDTH_SS*wwMap;			
			ofRectangle rectMap = ofRectangle(rectSlit.x+rectSlit.width+margen, rectSlit.y,
											  wwMap, hhMap);
			
			
//			warp.getDelayMap().draw(rectMap);
			if(!swmodoGL) 			warp.getDelayMap().getTextureReference().draw(rectMap);
			else					scanner.getDelayMapTex().draw(rectMap);

			ofNoFill();
			ofSetHexColor(0xcccccc);
			ofRect(rectMap);
		}
		ofPopStyle();
	
	
	ofPopStyle();
	
	
}

//--------------------------------------------------------------
void miSlitScan::keyPressed(int key){

	switch(key) {
		case 'i': 
			stats_savedImages();
			break;
		case 'v':
			swSeqActiva = !swSeqActiva;
			break;
		case 'n': 
			currentSampleMapIndex--;
			if(currentSampleMapIndex<0) currentSampleMapIndex+=sampleMaps.size();
//			currentSampleMapIndex%=sampleMaps.size();
			warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);			
			scanner.setDelayMap(sampleMaps[currentSampleMapIndex]->getTextureReference());
			break;
		case 'm': 
			currentSampleMapIndex++;
			currentSampleMapIndex%=sampleMaps.size();
			warp.setDelayMap(*sampleMaps[currentSampleMapIndex]);
			scanner.setDelayMap(sampleMaps[currentSampleMapIndex]->getTextureReference());
			break;
			
		case 'b':
			warp.toggleBlending();
			ofLogVerbose("Blending?: "+ofToString( warp.isBlending()) );
			break;
			
		case 'q':
			fr+=0.1;
			sequenceCam.setFrameRate(fr);
			ofLogVerbose("fr del sequencer: "+ofToString( fr ) );
			break;
		case 'a':
			fr-=0.1;
			if(fr<=0.0) fr = 0.1;
			sequenceCam.setFrameRate(fr);
			ofLogVerbose("fr del sequencer: "+ofToString( fr ) );
			break;
					
			
		case 'g':
			swmodoGL=!swmodoGL;
			break;
			

		case 'r':
			// lanzar/parar timer
			//			if(timer.getTimeLeftInMillis()>0) {
			
			if(timer.isTimerFinished()) ofLogNotice("Timer is finished");
			else ofLogNotice("Timer is NOT finished");
			break;
			
		case 't':
			// lanzar/parar timer
//			if(timer.getTimeLeftInMillis()>0) {
			
			if(!timer.isTimerFinished()) {
				// Si ya estaba activo: pararlo y quitar listener
				ofLogNotice("Timer en marcha. Lo paro y quito el listener");
				timer.stopTimer();
				timer.reset();
				ofRemoveListener(timer.TIMER_REACHED, this, &miSlitScan::tiempo);
				
			}
			else {
				// Lanzar timer y poner listener
				ofLogNotice("Lanzo Timer de 5000ms y pongo el listener");
				timer.setup(5000, false);
				
				// lanzar nueva coleccion de imagenes
				// escoger un id de camara al azar y lanzarlo
				cargarImagesCamara(idsCams[ floor(ofRandom(idsCams.size())) ]);		
				
				
				// capturar evento TIMER_REACHED
				ofAddListener(timer.TIMER_REACHED, this, &miSlitScan::tiempo);			
				
			}
			break;
			
			
			
	}
	
	
	
}

void miSlitScan::tiempo(ofEventArgs &e){
	ofLogNotice("Fin Timer. Listener");
//	ofRemoveListener(timer.TIMER_REACHED, this, &miSlitScan::tiempo);
	
	cargarImagesCamara(idsCams[ floor(ofRandom(idsCams.size())) ]);		
	
	timer.startTimer();
	
}



//--------------------------------------------------------------
void miSlitScan::keyReleased(int key){

}

//--------------------------------------------------------------
void miSlitScan::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void miSlitScan::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void miSlitScan::mousePressed(int x, int y, int button){
	// Comprobar si toca algun rectangulo
//	ofRectangle panel = ofRectangle(10,ultY,300,ofGetHeight()-ultY-10);
	for(int i=0; i<btnIdCamaras.size(); i++) {
		if( btnIdCamaras[i].rect.inside(ofPoint(x-panelCamaras.x,y-panelCamaras.y-25)) ) {
			ofLogVerbose("boton Camara #" + ofToString(btnIdCamaras[i].id));
			cargarImagesCamara(btnIdCamaras[i].id);
			return;
		}
	}
	for(int i=0; i<btnIdAlarmas.size(); i++) {
		if( btnIdAlarmas[i].rect.inside(ofPoint(x-panelAlarmas.x,y-panelAlarmas.y-25)) ) {
			ofLogVerbose("boton Alarma #" + ofToString(btnIdAlarmas[i].id));
			cargarImagesAlarma(btnIdAlarmas[i].id);
			return;
		}
	}
	

}

//--------------------------------------------------------------
void miSlitScan::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void miSlitScan::windowResized(int w, int h){

}

//--------------------------------------------------------------
void miSlitScan::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void miSlitScan::dragEvent(ofDragInfo dragInfo){ 

}