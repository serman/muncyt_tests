#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "Marco.h"

class scanner_faces : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	
	ofxCv::ObjectFinder finder;
	ofxCv::ObjectFinder finderEyes;
	void setupObjFinders();
	
	Marco marco;
	
	vector<ofColor> colores;
	
	vector<string> modos_haar;
	int id_modo_haar_act;

	
	// ESCENA
	void setupScn();
	// modos escena
	enum {	SCN_WAITING, // sistema en reposo
			SCN_PRESCAN,	// detecta a alguien. Propone acci—n. 
			SCN_SCAN,	// Animacion de escaneado.
			SCN_FIN	// Proceso escaneado terminado OK y Se ve el resultado en pantalla
	};
	int scnAct;	
	string nombreEscena;
	
	void setupEscenas();
	void setEscena(int idScena);
	void updateEscena();
	
	// caras detectadas
	int 	contCaras;
	int		CONT_CARAS_LIM;
	bool	bFaceDetected;
	
	// mensajes prescan
	vector<string> mensajes;
	void setupMensajes();
	int idMensajeAct;
	
	ofTrueTypeFont fontMensajes;
	
	// Control cambio de escena
	bool bScnOK;	
	
	// Limites tiempo
	int tWaiting;
	int tPreScan;
	int tScan;
	int tFin;
	
	int tLastCambio;

	
	
	// Animacion Chorras Escena Wait
	vector<ofPath>	shapes_Wait;
	void 		update_ScnWait();
	void 		draw_ScnWait();
	
	
	// Shader escaneo
	
	
	
	// grabar imagenes
	void saveScanImg(ofImage &imgScan, int nPers, int nImg);
	// nImg es un numero de sequencia de imagen del rostro:
	// 0: centro
	// 1: UL, 2: UR, 3:DR, 4:DL 
	int nZonaAct;
	int nPersonaAct;
	ofImage faceImage;
	ofRectangle faceRect, faceRectAmpl;

	bool caraOut;
	
	ofSoundPlayer	snd_click;
	
	
	void setLight(bool swlight);
	
};
