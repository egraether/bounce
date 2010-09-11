#define USE_CAMERA_TRACKING

#include "ofMain.h"
#include "Bounce.h"
#include "ofAppGlutWindow.h"
#include "constants.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, WIDTH, HEIGHT, OF_WINDOW);
	ofRunApp(new Bounce());
}
