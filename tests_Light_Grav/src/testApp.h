#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ofxUI.h"

#include "ParticleX.h"

#define W_WIDTH	 768
#define W_HEIGHT 768

class testApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofTrueTypeFont font;
		ofShader shader;
		bool doShader;
	
		ofVec2f		zentro;
	
		ofMesh espacio;
		ofVbo	espacioVBO;
		void setupMesh();
	
	
	ofMesh	superf, superfOrig;
	int		superfW, superfH;
	int		nH, nW;
	void	setupMeshSuperf();
	void	updateMeshSuperf();
	
	float kk;
	
	// helper functions
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c);
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c);
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d);
	ofVec3f getVertexFromImg(ofImage& img, int x, int y);
	
	void setNormals( ofMesh &mesh );
	
	
	ofEasyCam	cam;
	ofCamera	camera;
	
    ofLight		light;
	ofMaterial	mat1;
	ofColor		mat1Color;
	
	bool	swWireFrame;
	bool	swCool;
	bool	swLight;
	bool	swOrtho;
	
	
	bool	bUseTexture;
	ofImage	imgTex;

	ofFbo	imgMix;
	ofFbo	imgDyn;
	ofFbo	imgWeb;
	void	updateFBO(vector<ofVec2f> ptsInter);
	void	updateFBO();
	
	float rSol;
	ofSpherePrimitive sol;
	
	float nivelNoise;
	
	
	void setupGUI();
	ofxUISuperCanvas *gui1;	
	void guiEvent(ofxUIEventArgs &e);
	
    void exit();
	
	
	// camara-z
	float zCam;
	
	
	
	
	// Particulas
	vector <ParticleX> particulas;
	void	addParticleOut();
	void	updateParticlesX();	
	int		ratePartic;
	
};

