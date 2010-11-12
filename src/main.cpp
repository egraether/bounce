#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "Bounce.h"

int main() {
    int windowWidth = 1280;
    int windowHeight = 800;
    
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, windowWidth, windowHeight, OF_WINDOW);
	ofRunApp(new Bounce());
}
