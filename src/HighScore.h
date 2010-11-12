/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#ifndef _HIGHSCORE_
#define _HIGHSCORE_

#include "ofMain.h"

#include <cstring>
#include <fstream>
#include <map>

using namespace std;

class HighScore {
private:    
    multimap<int, string, greater<int> > scores;
    multimap<int, string, greater<int> >::iterator it, at;
    unsigned char alpha;
    bool blink;
    
public:
    bool checkScore(int score) {
        at = 0;
        blink = false;
        return score > scores.rbegin()->first;
    }
    
    void insertScore(string name, int score) {
        if (checkScore(score)) {
            at = scores.insert(pair<int, string>(score, name));
            scores.erase(--scores.end());
            blink = true;
        }
    }
    
    void updateNewName(string name) {
        at->second = name;
    }
    
    void readFile(const char* fileName) {
        ifstream file(fileName);
        
        if (file.fail()) {
            for (int i = 0; i < 10; i++) {
                scores.insert(pair<int, string>(10 * i, "~j~"));
            }
        }
        else {
            string scoreLine;
            for (int i = 0; i < 10; i++) {
                getline(file, scoreLine);
                
                int spacePosition = scoreLine.find_last_of(' ');
                string name = scoreLine.substr(0, spacePosition);
                int number = atoi(scoreLine.substr(spacePosition + 1).c_str());
                
                scores.insert(pair<int, string>(number, name));
            }
        }
        
        file.close();
        at = 0;
    }
    
    void writeFile(const char* fileName) {
        ofstream file(fileName);
        
        for (it = scores.begin(); it != scores.end(); it++)
            file << it->second << ' ' << it->first << '\n';
        
        file.close();
        blink = false;
        alpha = 255;
    }
    
    void printScores() {
        for (it = scores.begin(); it != scores.end(); it++)
            cout << it->first << ' ' << it->second << '\n';
    }
    
    void draw(ofTrueTypeFont &font, int x, int y) {
        ofPushMatrix();
        ofTranslate(x, y, 0);
        ofFill();
        
        ofEnableAlphaBlending();
        ofSetColor(220, 220, 255, 200);
        ofRect(0, 0, 200, (font.getLineHeight() + 10) * 10);
        ofDisableAlphaBlending();
        
        int row = 20;
        ofSetColor(0x384585);
        
        for (it = scores.begin(); it != scores.end(); it++) {
            if (it == at) {
                ofEnableAlphaBlending();
                if (blink)
                    alpha = char((sin(ofGetElapsedTimef() * 3) + 1) / 2 * 255);
                ofSetColor(180, 180, 255, alpha);
                ofRect(0, row - 15, 200, 20);
                ofDisableAlphaBlending();
                ofSetColor(0x384585);
            }
            
            font.drawString(ofToString(it->first), 40 - font.stringWidth(ofToString(it->first)) / 2, row);
            font.drawString(it->second, 140 - font.stringWidth(it->second) / 2, row);
            row += font.getLineHeight() + 10;
        }
        ofPopMatrix();
    }
};

#endif