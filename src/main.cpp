#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "Bounce.h"
#include "constants.h"

int main() {
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, WIDTH, HEIGHT, OF_WINDOW);
	ofRunApp(new Bounce());
}
