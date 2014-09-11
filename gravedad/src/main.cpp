#include "ofMain.h"
#include "fGravedad.h"

int main() {
    ofSetupOpenGL(1280, 800, OF_FULLSCREEN);
	ofRunApp(new fGravedad());
}
