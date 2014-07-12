#pragma once

#include "ofMain.h"
#include "ofxSlitScan.h"
#include "ofxImageSequence_Xtd.h"

#include "SlitScanGL.h"

#include "ofxTimer.h"

struct RectBtnID {
	ofRectangle rect;
	int			id;
	
	ofColor		colorRect;
	ofColor		colorFont;
	
	void draw(ofTrueTypeFont* f) {
		ofPushStyle();
			ofSetColor(colorRect);
			ofRect(rect);
			ofSetColor(colorFont);
			f->drawString(ofToString(id), rect.x+rect.width/2.0, rect.y+rect.height*2.0/3.0);
		ofPopStyle();
	}
};



class miSlitScan : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// INPUTS
		// lector de v’deo y lector de c‡mara 
		// y de secuencias de imagenes
		ofVideoGrabber	vidGrabber;
		ofVideoPlayer	vidPlayer;
	
		// Sequences
		ofxImageSequence_Xtd sequenceCam;
		ofxImageSequence_Xtd sequenceAlarm;
		ofxImageSequence_Xtd sequenceActiva;
		bool				 swSeqActiva;
	
		ofPixels			imgSeqPixels;	// *** para convertir de ofTexture a ofPixels y pasarlo al warp_SlitScan
		float fr;
	
		// lista de nombres de im‡genes de c‡mara
	//		cam_N_alarm_M_YYYYMMDDHHMMSS.jpg
	//		N: id de camara		M: id de alarma
		vector <string> savedImagesNames;
		void			leer_savedImagesNames();
		bool			swImagesLoaded;
	
		void			stats_savedImages();
		void			cargarImagesSeleccion();
		void			cargarImagesCamara(int idcam);
		void			cargarImagesAlarma(int idalarm);
	
		// Para las estadisticas y gestion de cargas de imagenes
		vector <int> idsAlarms;
		vector <int> statsAlarms;
		vector <int> idsCams;
		vector <int> statsCams;
	
		// - - - - - - - input selection - - - 
		bool useLiveVideo;
		void initLiveVideo();
		void initMovie();
		bool isFullScreen;
		bool isPaused;
	
		// - - - - - - - SLITSCAN - - - - - - - 
		ofxSlitScan		warp;
		int				capacity;
		void			setupSlitScan();
		void			setupMaps();
		void			changeCapacity();
		int				WIDTH_SS;
		int				HEIGHT_SS;

		// - - - - - - - SLITSCANGL - - - - - -
		ofxSlitScanGL	scanner;
		bool			swmodoGL;
	
		ofFbo		fboAux;
		
		// contenedor de mapas de SlitScan
		vector<string> sampleMapStrings;
		vector<ofImage*> sampleMaps;
		int currentSampleMapIndex;
	
		// GUI: imagenes de apoyo
		ofImage	colorImg;
		ofImage movieImg;
		ofImage	warpImage;	// ****
		ofImage previewImage;
		ofImage movieImage;
	
		// Formato imagen Final
		int wRender, hRender;
	
		// botones
		void				crearBotones(float wPanel, float margen, float wElem, float hElem, float sepElemX, float sepElemY);
		vector<RectBtnID>	btnIdCamaras;
		vector<RectBtnID>	btnIdAlarmas;
		ofPoint				posPanelCamaras;
		ofPoint				posPanelAlarmas;
		void				setupPanelesGUI();
	
		ofRectangle			panelTitulo;
		ofRectangle			panelCamaras;
		ofRectangle			panelAlarmas;
		ofRectangle			panelVisorSeq;
		ofRectangle			panelSeqCamaras;
		ofRectangle			panelSeqAlarmas;
		ofRectangle			panelSlitScan;
	
		// Fuente
		ofTrueTypeFont fontFutured;
		ofTrueTypeFont fontVerdana;
		ofTrueTypeFont fontVerdanaPeq;
	
	
	
		// Timer para cambiar autom‡ticamente de secuencia a reproducir
		ofxTimer	timer;
		void		tiempo(ofEventArgs &e);
		
};


