#include "Bounce.h"
#include "constants.h"

Bounce::Bounce() : 
    tracker(&infobox, &menuButton, &console),
    mode(MENU),
    hit(false),
    shootingCans(&infobox, &menuButton),
    robotDefense(&infobox, &menuButton),
    balloonHunt(&infobox, &menuButton) {
}

void Bounce::setup() {
    ofBackground(220, 220, 255);
    ofSetVerticalSync(true);
    
    srand(time(NULL));
    
    // audio
    ofSoundStreamSetup(0, 1, this);
    bangLevel = 2;
    lastBang = 0;
    bang = false;
    
    console.addInformation("lastBang", &lastBang);
    console.addRegulation("bangLevel", &bangLevel, 0, 15);
    
    calibrateButton.set("calibrate", WIDTH - 10 - WIDTH / 5, HEIGHT - 10 - WIDTH / 5,WIDTH / 5,WIDTH / 5);
    menuButton.set("Menu", WIDTH - 10 - WIDTH / 5, HEIGHT - 10 - WIDTH / 5,WIDTH / 5,WIDTH / 5);
    shootingCansButton.set("shootingCans", 10, 100, WIDTH / 5, WIDTH / 5);
    robotDefenseButton.set("robotDefense", 20 + WIDTH / 5, 100, WIDTH / 5, WIDTH / 5);
    balloonHuntButton.set("balloonHunt", 30 + 2 * WIDTH / 5, 100, WIDTH / 5, WIDTH / 5);
}

void Bounce::update() {
}

void Bounce::draw() {
    
    switch (mode) {
        case MENU:
            calibrateButton.draw();
            shootingCansButton.draw();
            robotDefenseButton.draw();
            balloonHuntButton.draw();
            
            if (calibrateButton.checkHit(hit, hitPoint)) {
                changeMode(CALIBRATE);
                tracker.reset();
            }
            else if (shootingCansButton.checkHit(hit, hitPoint)) {
                changeMode(SHOOTING_CANS);
                shootingCans.reset();
            }
            else if (robotDefenseButton.checkHit(hit, hitPoint)) {
                changeMode(ROBOT_DEFENSE);
                robotDefense.reset();
            }
            else if (balloonHuntButton.checkHit(hit, hitPoint)) {
                changeMode(BALLOON_HUNT);
                balloonHunt.reset();
            }
            break;
        case CALIBRATE:
            if (!tracker.draw(hit, hitPoint))
                changeMode(MENU);
            break;
        case SHOOTING_CANS:
            if (!shootingCans.draw(hit, hitPoint))
                changeMode(MENU);
            break;
        case ROBOT_DEFENSE:
            if (!robotDefense.draw(hit, hitPoint))
                changeMode(MENU);
            break;
        case BALLOON_HUNT:
            if (!balloonHunt.draw(hit, hitPoint))
                changeMode(MENU);
            break;
        default:
            break;
    }
    
    infobox.draw();
    
    tracker.drawPics();
    
    ofFill();
    ofSetColor(0, 255, 0);
    ofCircle(hitPoint.x, hitPoint.y, HITPOINT_SIZE);
    
    hit = false;
    
    console.draw();
    ofSetColor(0x000000);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), WIDTH - 150, 20);
    
    if (bang) {
        tracker.getPics();
        hit = tracker.getHitPoint(hitPoint);
        bang = false;
    }
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
    switch (key) {
        case 'm':
            changeMode(MENU);
            break;
        case 'f':
            ofSetFullscreen(true);
            break;
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

void Bounce::changeMode(Mode m) {
    mode = m;
    infobox.kill();
}