#define USE_CAMERA_TRACKING

#include "ofMain.h"
#include "Bounce.h"
#include "ofAppGlutWindow.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, OF_FULLSCREEN);
	ofRunApp(new Bounce());
}
