#include "Bounce.h"

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
    Game::initializeStatics();
    
    ofBackground(220, 220, 255);
    ofSetFrameRate(60.0f);
    
    srand(time(NULL));
    
    // audio
    ofSoundStreamSetup(0, 1, this);
    
    // pictures
    background.load("menu_bg.png", false, GL_CLAMP, GL_CLAMP);
    bounceLogo.load("logo_bounce.png", true, GL_CLAMP, GL_CLAMP);
    fhLogo.load("logo_fh.png", true, GL_CLAMP, GL_CLAMP);
    mmtLogo.load("logo_mmt.jpg", false, GL_CLAMP, GL_CLAMP);
    mmaLogo.load("logo_mma.jpg", false, GL_CLAMP, GL_CLAMP);
    
    // buttons
    calibrateButton.set("calibrate_button.png", ofGetWidth() - 300, ofGetHeight() - 150, 250, 100);
    creditsButton.set("credits_button.png", 150, ofGetHeight() - 180, 250, 100);
    
    shootingCansButton.set("cans_button.png", (ofGetWidth() / 3 - 300) / 2 + 50, 250, 300, 300);
    robotDefenseButton.set("robot_button.png", (ofGetWidth()/ 3 - 300) / 2 + ofGetWidth() / 3, 250, 300, 300);
    balloonHuntButton.set("balloon_button.png", (ofGetWidth() / 3 - 300) / 2 + ofGetWidth() / 3 * 2 - 50, 250, 300, 300);
    risingNinjaButton.set("wordbubble.png", (ofGetWidth() / 3 - 300) / 2 + ofGetWidth() / 3, 500, 300, 300);
    
    // fonts
    font2.loadFont("microgme.ttf", 20);
    font.loadFont("microgme.ttf", 13);
}

void Bounce::update() {}

void Bounce::draw() {
    switch (mode) {
        case MENU:
            background.draw(0, 0, ofGetWidth(), ofGetHeight());
            bounceLogo.draw(20, 20);
            fhLogo.draw(ofGetWidth() - 300, 20, 300, 100);
            mmaLogo.draw(ofGetWidth() - 225, 120, 200, 40);
            mmtLogo.draw(ofGetWidth() - 450, 120, 200, 40);
            
            calibrateButton.draw();
            //creditsButton.draw();
            shootingCansButton.draw();
            robotDefenseButton.draw();
            balloonHuntButton.draw();
//            risingNinjaButton.draw();
            
            ofSetColor(0x384585);
            Game::scoreFont.drawString("developed by:", 50, ofGetHeight() - 185);
            font2.drawString("Eberhard Graether", 70, ofGetHeight()- 150);
            
            Game::scoreFont.drawString("graphic design:", 500, ofGetHeight()- 185);
            font2.drawString("Dominik Wiesauer", 520, ofGetHeight() - 150);
            
            Game::scoreFont.drawString("special thanks:", 50, ofGetHeight() - 110);
            font.drawString( 
                "Felix Hummel        Martin Ortner       Malte Langkabel     David Strausz\nChristian Winkler     Marius Schebella     Manuel Gottstein    Franz Lanzendorfer\nThomas Buchoester  Andreas Stallinger", 
                70, ofGetHeight() - 80
            );
            
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
                ofRect(0, 0, ofGetWidth(), ofGetHeight());
                ofDisableAlphaBlending();
                
                ofSetColor(0x384585);
                string s = "Game";
                Game::bigFont.drawString(s, (ofGetWidth() - Game::bigFont.stringWidth(s)) / 2, ofGetHeight() / 2 - 30);
                s = "Paused";
                Game::bigFont.drawString(s, (ofGetWidth() - Game::bigFont.stringWidth(s)) / 2, ofGetHeight() / 2 + 30);
                s = "press space-bar to continue.";
                Game::gameFont.drawString(s, (ofGetWidth() - Game::gameFont.stringWidth(s)) / 2, ofGetHeight() / 2 + 100);
            }
            break;
    }
    
    tracker.drawPics();
    infobox.draw();
    
    ofFill();
    ofSetColor(0, 255, 0);
    ofCircle(hitPoint.x, hitPoint.y, 3);
    
    hit = false;
    
    console.draw();
    
    if (!gamePaused)
        hit = tracker.getHitPoint(hitPoint);
}

void Bounce::audioReceived (float* input, int bufferSize, int nChannels) {
    
    // summing up the squares of each bufferbin to get full energy of audio input
    float energy = .0f;
	for (int i = 0, ii = bufferSize * nChannels; i < ii; i++) {
        energy += input[i] * input[i];
	}
    
    tracker.audioInput(energy);
}


void Bounce::keyPressed(int key) {
    
    // true when typing a new name at the highscores
    if (activeGame && activeGame->keyPressed(key))
        return;
    
    switch (key) {
        // end running game and return to menu
        case 'm':
            changeMode(MENU);
            break;
            
        // switch fullscreen
        case 'f':
            ofToggleFullscreen();
            break;
            
        // pause the game
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
            
        // show the console
        case 'k':
            console.show = !console.show;
            break;
        
        // console interface
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

// every mouseclick is a bounce
void Bounce::mousePressed(int x, int y, int button) {
    if (button == 0) {
        hitPoint.set(x, y);
        hit = true;
    }
}

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

void Bounce::mouseMoved(int x, int y ) {}
void Bounce::mouseDragged(int x, int y, int button) {}
void Bounce::mouseReleased(int x, int y, int button) {}
void Bounce::windowResized(int w, int h) {}