#include "Game.h"

Texture* Game::panel = 0;
ofTrueTypeFont* Game::titelFont = 0;
ofTrueTypeFont* Game::gameFont = 0;
ofTrueTypeFont* Game::scoreFont = 0;
ofTrueTypeFont* Game::bigFont = 0;

//class Game {
//protected:
//    string titel;
//    bool gameOver;
//    enum {INIT, PLAY, PANEL} mode;
//    
//    Infobox* infobox;
//    int points;
//    int ballHits;
//    
//    string scoresFileName;
//    HighScore highScore;
//    
//    string name;
//    bool insertName;
//    
//    float startTime, pauseTime;
//    
//    vector<Sign> signs;
//    Texture* background;
//    
//public:
//    static ofTrueTypeFont* titelFont;
//    static Texture* panel;
//    
//    Game(string _titel, Infobox* _infobox, string _scoresFileName) : 
//        titel(_titel), infobox(_infobox), scoresFileName(_scoresFileName), insertName(false) {
//            
//        highScore.readFile(scoresFileName.c_str());
//    }
//    
//    virtual void reset() = 0;
//    virtual bool draw(bool hit, Vector &hitPoint) = 0;
//    
//    void resetGame() {
//        mode = INIT;
//        points = 0;
//        ballHits = 0;
//        insertName = false;
//        name = "";
//        gameOver = false;
//        signs.clear();
//        infobox->set("bounce to start.");
//    }
//    
//    void startGame() {
//        mode = PLAY;
//        infobox->clear();
//        startTime = ofGetElapsedTimef();
//    }
//    
//    void stopGame() {
//        mode = PANEL;
//        gameOver = true;
//        infobox->set("bounce to continue.");
//        
//        if (highScore.checkScore(points)) {
//            highScore.insertScore(name, points);
//            infobox->set("you got a new highScore, insert your name at the keyboard and press enter.");
//            insertName = true;
//        }
//    }
//    
//    void drawPanel() {
//        panel->draw(WIDTH * 3 / 8, HEIGHT / 4, WIDTH / 4, HEIGHT / 2);
//        
//        ofDrawBitmapString(titel, WIDTH * 3 / 8 + 20, HEIGHT / 4 + 20);
//        highScore.draw(WIDTH * 3 / 8 + 20, HEIGHT / 4 + 50);
//        ofDrawBitmapString("your score: " + ofToString(points), WIDTH * 3 / 8 + 20, HEIGHT / 4 + 300);
//    }
//    
//    void drawSigns() {
//        for (int i = 0; i < signs.size(); i++) {
//            if (!signs[i].draw())
//                signs.erase(signs.begin() + i);
//        }
//    }
//    
//    bool keyPressed(int key) {
//        if (insertName) {
//            char character = char(key);
//            
//            switch (key) {
//                case OF_KEY_BACKSPACE:
//                    name = name.substr(0, name.length() - 1);
//                    highScore.updateNewName(name);
//                    break;
//                case OF_KEY_RETURN:
//                    //highScore.insertScore(name, points);
//                    highScore.writeFile(scoresFileName.c_str());
//                    insertName = false;
//                    infobox->set("bounce to continue.");
//                    break;
//                default:
//                    if (name.length() >= 10)
//                        break;
//                    
//                    name += character;
//                    highScore.updateNewName(name);
//                    break;
//            }
//        }
//        
//        return insertName;
//    }
//    
//    bool pause() {
//        if (mode == PLAY) {
//            pauseTime = ofGetElapsedTimef();
//            return true;
//        }
//        return false;
//    }
//    
//    void proceed() {
//        startTime += ofGetElapsedTimef() - pauseTime;
//    }
//};
