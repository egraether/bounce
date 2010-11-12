/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#include "Game.h"

Texture* Game::panel = 0;
ofTrueTypeFont Game::titelFont;
ofTrueTypeFont Game::gameFont;
ofTrueTypeFont Game::scoreFont;
ofTrueTypeFont Game::bigFont;

Game::Game(string _titel, Infobox* _infobox, string _scoresFileName) : 
    titel(_titel), 
    infobox(_infobox), 
    scoresFileName(_scoresFileName), 
    insertName(false), 
    textColor(0x384585) {
    
    highScore.readFile(scoresFileName.c_str());
}

void Game::resetGame() {
    mode = INIT;
    
    points = 0;
    ballHits = 0;
    insertName = false;
    
    name = "";
    gameOver = false;
    
    signs.clear();
    infobox->set("bounce to start.");
    
    screenTime = 0;
}

void Game::startGame() {
    mode = PLAY;
    infobox->clear();
    startTime = ofGetElapsedTimef();
}

void Game::stopGame() {
    mode = PANEL;
    gameOver = true;
    infobox->set("bounce to continue.");
    
    if (highScore.checkScore(points)) {
        highScore.insertScore(name, points);
        infobox->set("you have a new highScore, insert your name at the keyboard and press enter.");
        insertName = true;
    }
    
    lastScore.insertScore(points);
}

void Game::drawPanel() {
    ofPushMatrix();
    ofTranslate((ofGetWidth() - 500) / 2, (ofGetHeight() - 600) / 2, 0);
    
    panel->draw(0, 0, 500, 630);
    
    ofSetColor(0x384585);
    titelFont.drawString(titel, 50, 70);
    
    gameFont.drawString("your score:", 50, 130);
    bigFont.drawString(ofToString(points), (500 - bigFont.stringWidth(ofToString(points))) / 2, 200);
    
    ofSetColor(0x584585);
    scoreFont.drawString("Highcores:", 50, 250);
    scoreFont.drawString("Last Players:", 310, 250);
    highScore.draw(scoreFont, 50, 270);
    lastScore.draw(scoreFont, 310, 270);
    
    ofPopMatrix();
}

void Game::drawSigns() {
    for (int i = 0; i < signs.size(); i++) {
        if (!signs[i].draw(gameFont)) {
            signs.erase(signs.begin() + i);
            i--;
        }
    }
    
    ofSetColor(textColor);
    gameFont.drawString("points: " + ofToString(points), 20, 40);
    string t = "time: " + ofToString(screenTime);
    gameFont.drawString(t, ofGetWidth() - 20 - gameFont.stringWidth(t), 40);
}

bool Game::keyPressed(int key) {
    if (insertName) {
        char character = char(key);
        
        switch (key) {
            case OF_KEY_BACKSPACE:
                name = name.substr(0, name.length() - 1);
                highScore.updateNewName(name);
                break;
            case OF_KEY_RETURN:
                highScore.writeFile(scoresFileName.c_str());
                insertName = false;
                infobox->set("bounce to continue.");
                break;
            default:
                if (name.length() >= 10)
                    break;
                
                name += character;
                highScore.updateNewName(name);
                break;
        }
    }
    
    return insertName;
}

bool Game::pause() {
    if (mode == PLAY) {
        pauseTime = ofGetElapsedTimef();
        return true;
    }
    return false;
}

void Game::proceed() {
    startTime += ofGetElapsedTimef() - pauseTime;
}

void Game::initializeStatics() {
    panel = new Texture();
    panel->load("wordbubble.png", true, GL_CLAMP, GL_CLAMP);
    titelFont.loadFont("keypuncn.ttf", 40);
    gameFont.loadFont("microgme.ttf", 26);
    scoreFont.loadFont("microgme.ttf", 16);
    bigFont.loadFont("microgme.ttf", 46);
}

