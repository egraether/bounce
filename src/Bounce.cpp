#include "Bounce.h"
#include "constants.h"

Bounce::Bounce() : 
    tracker(&infobox, &console),
    mode(MENU),
    hit(false),
    gamePaused(false),
    activeGame(0),
    shootingCans("Shooting Cans", &infobox, "cans.txt"),
    robotDefense("Robot Defense", &infobox, "robots.txt"),
    balloonHunt("BalloonHunt", &infobox, "balloons.txt"),
    risingNinja("RisingNinja", &infobox, "ninja.txt") {
}

void Bounce::setup() {
    ofBackground(220, 220, 255);
    //ofSetVerticalSync(true);
    ofSetFrameRate(60.0f);
    
    srand(time(NULL));
    
    // audio
    ofSoundStreamSetup(0, 1, this);
    fullScreen = false;
    
    calibrateButton.set("calibrate", WIDTH - 10 - WIDTH / 5, HEIGHT - 10 - HEIGHT / 5, WIDTH / 5, HEIGHT / 5);
    shootingCansButton.set("shootingCans", 10, 100, WIDTH / 5, HEIGHT / 5);
    robotDefenseButton.set("robotDefense", 20 + WIDTH / 5, 100, WIDTH / 5, HEIGHT / 5);
    balloonHuntButton.set("balloonHunt", 30 + 2 * WIDTH / 5, 100, WIDTH / 5, HEIGHT / 5);
    risingNinjaButton.set("risingNinja", 40 + 3 * WIDTH / 5, 100, WIDTH / 5, HEIGHT / 5);
}

void Bounce::update() {}

void Bounce::draw() {
    
    switch (mode) {
        case MENU:
            calibrateButton.draw();
            shootingCansButton.draw();
            robotDefenseButton.draw();
            balloonHuntButton.draw();
            risingNinjaButton.draw();
            
            if (calibrateButton.checkHit(hit, hitPoint))
                changeMode(CALIBRATE);
            else if (shootingCansButton.checkHit(hit, hitPoint))
                changeMode(SHOOTING_CANS);
            else if (robotDefenseButton.checkHit(hit, hitPoint))
                changeMode(ROBOT_DEFENSE);
            else if (balloonHuntButton.checkHit(hit, hitPoint))
                changeMode(BALLOON_HUNT);
            else if (risingNinjaButton.checkHit(hit, hitPoint))
                changeMode(RISING_NINJA);
            break;
        case CALIBRATE:
            if (!tracker.draw(hit, hitPoint))
                changeMode(MENU);
            break;
        default:
            if (!gamePaused) {
                if (!activeGame->draw(hit, hitPoint))
                    changeMode(MENU);
            }
            else {
                tracker.drawPauseScreen();
                ofEnableAlphaBlending();
                ofSetColor(220, 220, 220, 125);
                ofRect(0, 0, WIDTH, HEIGHT);
                ofDisableAlphaBlending();
            }
            break;
    }
    
    tracker.drawPics();
    infobox.draw();
    
    ofFill();
    ofSetColor(0, 255, 0);
    ofCircle(hitPoint.x, hitPoint.y, HITPOINT_SIZE);
    
    hit = false;
    
    console.draw();
    ofSetColor(0x000000);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), WIDTH - 150, 40);
    
    if (!gamePaused)
        hit = tracker.getHitPoint(hitPoint);
}

void Bounce::audioReceived (float* input, int bufferSize, int nChannels) {
    float energy = .0f;
	for (int i = 0, ii = bufferSize * nChannels; i < ii; i++) {
        energy += input[i] * input[i];
	}
    
    tracker.audioInput(energy);
}

void Bounce::keyPressed(int key) {
    if (activeGame && activeGame->keyPressed(key))
        return;
    
    switch (key) {
        case 'm':
            changeMode(MENU);
            break;
        case 'f':
            fullScreen = !fullScreen;
            ofSetFullscreen(fullScreen);
            break;
        case 'k':
            console.show = !console.show;
            break;
        case ' ':
            if (activeGame) {
                if (gamePaused) {
                    gamePaused = false;
                    activeGame->proceed();
                }
                else {
                    gamePaused = activeGame->pause();
                }
            }
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
    infobox.clear();
    
    switch (mode) {
        case MENU:
            activeGame = 0;
            break;
        case CALIBRATE:
            tracker.reset();
            break;
        case SHOOTING_CANS:
            activeGame = &shootingCans;
            break;
        case ROBOT_DEFENSE:
            activeGame = &robotDefense;
            break;
        case BALLOON_HUNT:
            activeGame = &balloonHunt;
            break;
        case RISING_NINJA:
            activeGame = &risingNinja;
            break;
        default:
            break;
    }
    
    if (activeGame)
        activeGame->reset();
}