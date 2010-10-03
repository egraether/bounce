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
    ofSetFrameRate(60.0f);
    
    srand(time(NULL));
    
    // audio
    ofSoundStreamSetup(0, 1, this);
    fullScreen = false;
    
    background.load("menu_bg.png", false, GL_CLAMP, GL_CLAMP);
    bounceLogo.load("logo_bounce.png", true, GL_CLAMP, GL_CLAMP);
    fhLogo.load("logo_fh.png", true, GL_CLAMP, GL_CLAMP);
    mmaMmtLogo.load("logo_mmammt.png", true, GL_CLAMP, GL_CLAMP);
    
    calibrateButton.set("calibrate_button.png", WIDTH - 350, HEIGHT - 150, 250, 100);
    creditsButton.set("credits_button.png", 100, HEIGHT - 150, 250, 100);
    
    shootingCansButton.set("cans_button.png", (WIDTH / 3 - 300) / 2, 250, 300, 300);
    robotDefenseButton.set("robot_button.png", (WIDTH / 3 - 300) / 2 + WIDTH / 3, 250, 300, 300);
    balloonHuntButton.set("balloon_button.png", (WIDTH / 3 - 300) / 2 + WIDTH / 3 * 2, 250, 300, 300);
    risingNinjaButton.set("wordbubble2.png", (WIDTH / 3 - 300) / 2 + WIDTH / 3, 500, 300, 300);
    
    Game::initializeStatics();
}

void Bounce::update() {}

void Bounce::draw() {
    switch (mode) {
        case MENU:
            background.draw(0, 0, WIDTH, HEIGHT);
            bounceLogo.draw(20, 20);
            fhLogo.draw(WIDTH - 400, 20);
            mmaMmtLogo.draw(WIDTH - 375, 150);
            
            calibrateButton.draw();
            creditsButton.draw();
            shootingCansButton.draw();
            robotDefenseButton.draw();
            balloonHuntButton.draw();
//            risingNinjaButton.draw();
            
            if (calibrateButton.checkHit(hit, hitPoint))
                changeMode(CALIBRATE);
            else if (shootingCansButton.checkHit(hit, hitPoint))
                changeMode(SHOOTING_CANS);
            else if (robotDefenseButton.checkHit(hit, hitPoint))
                changeMode(ROBOT_DEFENSE);
            else if (balloonHuntButton.checkHit(hit, hitPoint))
                changeMode(BALLOON_HUNT);
//            else if (risingNinjaButton.checkHit(hit, hitPoint))
//                changeMode(RISING_NINJA);
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