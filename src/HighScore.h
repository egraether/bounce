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
    
public:
    bool checkScore(int score) {
        return score > scores.rbegin()->first;
    }
    
    void insertScore(string name, int score) {
        if (checkScore(score)) {
            at = scores.insert(pair<int, string>(score, name));
            scores.erase(--scores.end());
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
        at = 0;
    }
    
    void printScores() {
        for (it = scores.begin(); it != scores.end(); it++)
            cout << it->first << ' ' << it->second << '\n';
    }
    
    void draw(int x, int y) {
        int row = y + 15;
        ofSetColor(0, 0, 0);
        
        for (it = scores.begin(); it != scores.end(); it++) {
            if (it == at) {
                ofFill();
                ofSetColor(220, 220, 220);
                ofRect(x, row - 15, 200, 20);
                ofSetColor(0, 0, 0);
            }
            
            ofDrawBitmapString(ofToString(it->first), x + 10, row);
            ofDrawBitmapString(it->second, x + 50, row);
            row += 20;
        }
    }
};

#endif