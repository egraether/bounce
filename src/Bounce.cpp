#include "Bounce.h"

void Bounce::setup() {
    ofBackground(255, 255, 255);
    ofSetVerticalSync(true);
}

void Bounce::update() {
}

void Bounce::draw() {
    tracker.draw();
}


void Bounce::keyPressed(int key) {
    tracker.keyPressed(key);
}

void Bounce::mouseMoved(int x, int y ) {}
void Bounce::mousePressed(int x, int y, int button) {}
void Bounce::mouseDragged(int x, int y, int button) {}
void Bounce::mouseReleased(int x, int y, int button) {}
void Bounce::windowResized(int w, int h) {}