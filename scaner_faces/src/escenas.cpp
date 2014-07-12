/*
 *  escenas.cpp
 *  scaner_faces
 *
 *  Created by guillermo casado on 23/06/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#include "scanner_faces.h"

using namespace ofxCv;
using namespace cv;

void scanner_faces::setupEscenas() {
	
	contCaras = 0;
	CONT_CARAS_LIM = 40;
	
	setupMensajes();
	
}

void scanner_faces::setupMensajes() {
	mensajes.push_back("OOXOO");
	mensajes.push_back("SITUA TU ROSTRO EN EL MARCO");
	mensajes.push_back("ESPERA");
	mensajes.push_back("PROCESADO");
	mensajes.push_back("OOXOO");
	idMensajeAct = 0;
	
	fontMensajes.loadFont("fonts/atari_full.ttf", 45);
	
}

// - 

void scanner_faces::setEscena(int idScena) {
	ofLogNotice("SetExcena INI");
	if(idScena==SCN_WAITING) {
		nombreEscena = "WAITING";
		scnAct = SCN_WAITING;
		contCaras = 0;
		
		bFaceDetected = false;
		
		// eliminar cosas y timers, eventos
		
		
		// lanzar animaciones: timers y demás 
		
	} 
	else if(idScena==SCN_PRESCAN) {
		nombreEscena = "PRESCAN";
		scnAct = SCN_PRESCAN;
		
		idMensajeAct = 0;
		
		caraOut = true;
		
		
		// eliminar cosas y timers, eventos
		
		
		// lanzar animaciones: timers y demás 
		
	} 
	else if(idScena==SCN_SCAN) {
		nombreEscena = "SCAN";
		scnAct = SCN_SCAN;
		
		// eliminar cosas y timers, eventos
		
		// lanzar animaciones: timers y demás 
		
	} 
	else if(idScena==SCN_FIN) {
		nombreEscena = "END";
		scnAct = SCN_FIN;
		
		// eliminar cosas y timers, eventos
		
		
		// lanzar animaciones: timers y demás 
		
	}
	ofLogNotice("SetExcena FIN");
	
}


void scanner_faces::updateEscena() {
	// Update una cosa u otra según el modo
	if(scnAct==SCN_WAITING) {
		update_ScnWait();
		if(bFaceDetected) {
			contCaras++;
			if(contCaras>=CONT_CARAS_LIM) {
				setEscena(SCN_PRESCAN);
			}
		}
	}
	else if(scnAct==SCN_PRESCAN) {
		
	}
	else if(scnAct==SCN_SCAN) {
		
	}
	else if(scnAct==SCN_FIN) {
		
	}
	
	
}




