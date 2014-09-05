/*
 *  Centro.h
 *  Nuclear_Fuerte_1
 *
 *  Created by guillermo casado on 04/09/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 
 Centro sirve para:
 - detectar qué Emisores estan lanzando partículas 
 - obtener la direccion neta resultado del choque de los emisores
 
 - Interesante cuando la neta = 0 
 - Hay que tener en cuenta que la energia de cada rayo sea lo suficientemente alta (distancia al centro)
 
 
*/

class Centro {

	ofVec2f dirNeta;	
	
	Centro() {
		dirNeta = ofVec2f(0,0);
	}
	
	


	void emiteParticulas() {
		
	}
	
};