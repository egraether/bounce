#ifndef _GAME_
#define _GAME_

#include "ofMain.h"
#include "Vector.h"
#include "PushButton.h"
#include "Texture.h"
#include "Infobox.h"
#include "HighScore.h"
#include "constants.h"
#include "Sign.h"

#include <vector>

class Game {
protected:
    string titel;
    bool gameOver;
    enum {INIT, PLAY, PANEL} mode;
    
    Infobox* infobox;
    int points;
    int ballHits;
    
    string scoresFileName;
    HighScore highScore;
    
    string name;
    bool insertName;
    
    float startTime, pauseTime;
    int screenTime;
    
    vector<Sign> signs;
    Texture* background;
    
public:
    static ofTrueTypeFont* titelFont;
    static ofTrueTypeFont* gameFont;
    static ofTrueTypeFont* scoreFont;
    static ofTrueTypeFont* bigFont;
    static Texture* panel;
    
    Game(string _titel, Infobox* _infobox, string _scoresFileName) : 
        titel(_titel), infobox(_infobox), scoresFileName(_scoresFileName), insertName(false) {
            
        highScore.readFile(scoresFileName.c_str());
    }
    
    virtual void reset() = 0;
    virtual bool draw(bool hit, Vector &hitPoint) = 0;
    
    void resetGame() {
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
    
    void startGame() {
        mode = PLAY;
        infobox->clear();
        startTime = ofGetElapsedTimef();
    }
    
    void stopGame() {
        mode = PANEL;
        gameOver = true;
        infobox->set("bounce to continue.");
        
        if (highScore.checkScore(points)) {
            highScore.insertScore(name, points);
            infobox->set("you got a new highScore, insert your name at the keyboard and press enter.");
            insertName = true;
        }
    }
    
    void drawPanel() {
        ofPushMatrix();
        ofTranslate((WIDTH - 500) / 2, (HEIGHT - 500) / 2, 0);
        
        panel->draw(0, 0, 500, 500);
        
        ofSetColor(0x384585);
        titelFont->drawString(titel, 50, 70);
        scoreFont->drawString("your score: " + ofToString(points), 50, 110);
        highScore.draw(scoreFont, 150, 150);
        
        ofPopMatrix();
    }
    
    void drawSigns() {
        for (int i = 0; i < signs.size(); i++) {
            if (!signs[i].draw(gameFont)) {
                signs.erase(signs.begin() + i);
                i--;
            }
        }
        
        ofSetColor(0x384585);
        gameFont->drawString("points: " + ofToString(points), 20, 40);
        string t = "time: " + ofToString(screenTime);
        gameFont->drawString(t, WIDTH - 20 - gameFont->stringWidth(t), 40);
    }
    
    bool keyPressed(int key) {
        if (insertName) {
            char character = char(key);
            
            switch (key) {
                case OF_KEY_BACKSPACE:
                    name = name.substr(0, name.length() - 1);
                    highScore.updateNewName(name);
                    break;
                case OF_KEY_RETURN:
                    //highScore.insertScore(name, points);
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
    
    bool pause() {
        if (mode == PLAY) {
            pauseTime = ofGetElapsedTimef();
            return true;
        }
        return false;
    }
    
    void proceed() {
        startTime += ofGetElapsedTimef() - pauseTime;
    }
    
    static void initializeStatics() {
        panel = new Texture();
        panel->load("wordbubble2.png", true, GL_CLAMP, GL_CLAMP);
        titelFont = new ofTrueTypeFont();
        titelFont->loadFont("keypuncn.ttf", 40);
        gameFont = new ofTrueTypeFont();
        gameFont->loadFont("microgme.ttf", 26);
        scoreFont = new ofTrueTypeFont();
        scoreFont->loadFont("microgme.ttf", 18);
    }
};

#endif

