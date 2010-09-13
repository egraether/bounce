#include "Bounce.h"
#include "constants.h"

Bounce::Bounce() : tracker(&console) {}

void Bounce::setup() {
    ofBackground(255, 255, 255);
    ofSetVerticalSync(true);
    
    hit = false;
    
    // audio
    ofSoundStreamSetup(0, 1, this);
    bangLevel = 9;
    lastBang = 0;
    bang = false;
    
    console.addInformation("lastBang", &lastBang);
    console.addRegulation("bangLevel", &bangLevel, 0, 15);
}

void Bounce::update() {
}

void Bounce::draw() {
    if (bang) {
        hit = tracker.getHitPoint(hitPoint);
        bang = false;
    }
    
    tracker.draw();
    
    ofFill();
    ofSetColor(0, 255, 0);
    ofCircle(hitPoint.x, hitPoint.y, HITPOINT_SIZE);
    
    hit = false;
    
    console.draw();
}

void Bounce::audioReceived (float* input, int bufferSize, int nChannels) {
	for (int i = 0; i < bufferSize; i++){
		if (input[i] * 10 > bangLevel) {
            lastBang = input[i] * 10;
            bang = true;
        }
	}	
}

void Bounce::keyPressed(int key) {
    switch (key){
        case 'k':
            console.show = !console.show;
            break;
        case OF_KEY_LEFT:
            console.next(false);
            break;
        case OF_KEY_RIGHT:
            console.next(true);
            break;
        case OF_KEY_UP:
            console.change(true);
            break;
        case OF_KEY_DOWN:
            console.change(false);
            break;
    }
    
    tracker.keyPressed(key);
}

void Bounce::mouseMoved(int x, int y ) {}
void Bounce::mousePressed(int x, int y, int button) {
    if (button == 0) {
        hitPoint.set(x, y);
        hit = true;
    }
}
void Bounce::mouseDragged(int x, int y, int button) {}
void Bounce::mouseReleased(int x, int y, int button) {}
void Bounce::windowResized(int w, int h) {}