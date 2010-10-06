#ifndef _LASTSCORE_
#define _LASTSCORE_

#include "ofMain.h"

#include <set>
#include <deque>

class LastScore {
private:
    multiset<int, greater<int> > scoresSorted;
    deque<int> scoresHistory; 
    
    multiset<int, greater<int> >::iterator it, at;
    
public:
    void insertScore(int score) {
        at = scoresSorted.insert(score);
        scoresHistory.push_back(score);
        
        if (scoresHistory.size() > 10) {
            int score = scoresHistory.front();
            scoresHistory.pop_front();
            scoresSorted.erase(score);
        }
    }
    
    void draw(ofTrueTypeFont &font, int x, int y) {
        ofPushMatrix();
        ofTranslate(x, y, 0);
        ofFill();
        
        ofEnableAlphaBlending();
        ofSetColor(220, 220, 255, 200);
        ofRect(0, 0, 120, (font.getLineHeight() + 10) * 10);
        ofDisableAlphaBlending();
        
        int row = 20;
        ofSetColor(0x384585);
        
        for (it = scoresSorted.begin(); it != scoresSorted.end(); it++) {
            if (it == at) {
                ofSetColor(180, 180, 255);
                ofRect(0, row - 15, 120, 20);
                ofSetColor(0x384585);
            }
            
            font.drawString(ofToString(*it), 60 - font.stringWidth(ofToString(*it)) / 2, row);
            row += font.getLineHeight() + 10;
        }
        ofPopMatrix();
    }
};

#endif