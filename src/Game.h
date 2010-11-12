#ifndef _GAME_
#define _GAME_

#include "ofMain.h"
#include "Vector.h"
#include "PushButton.h"
#include "Texture.h"
#include "Infobox.h"
#include "HighScore.h"
#include "LastScore.h"
#include "constants.h"
#include "Sign.h"

#include <vector>
#include <set>

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
    LastScore lastScore;
    
    string name;
    bool insertName;
    
    float startTime, pauseTime;
    int screenTime;
    
    vector<Sign> signs;
    Texture background;
    
    int textColor;
    
public:
    static ofTrueTypeFont titelFont;
    static ofTrueTypeFont gameFont;
    static ofTrueTypeFont scoreFont;
    static ofTrueTypeFont bigFont;
    static Texture* panel;
    
    Game(string _titel, Infobox* _infobox, string _scoresFileName);
    
    virtual void reset() = 0;
    virtual bool draw(bool hit, Vector &hitPoint) = 0;
    
    void resetGame();
    void startGame();
    void stopGame();
    
    void drawPanel();
    void drawSigns();
    bool keyPressed(int key);
    
    bool pause();
    void proceed();
    
    static void initializeStatics();
};

#endif

