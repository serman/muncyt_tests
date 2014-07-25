#include "ofMain.h"
#include "testApp.h"

int main() {
    ofSetupOpenGL(1280, 800, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
