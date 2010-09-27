#ifndef _HIGHSCORE_
#define _HIGHSCORE_

#include <cstring>
#include <fstream>
#include <map>

using namespace std;

class HighScore {
private:
    struct classcomp {
        bool operator() (const int l, const int& r) const {
            return l > r;
        }
    };
    
    multimap<int, string, classcomp> scores;
    multimap<int, string, classcomp>::iterator it;
    
public:
    bool checkScore(int score) {
        return score > scores.rbegin()->first;
    }
    
    void insertScore(string name, int score) {
        if (checkScore(score)) {
            scores.insert(pair<int, string>(score, name));
            scores.erase(--scores.end());
        }
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
    }
    
    void writeFile(const char* fileName) {
        ofstream file(fileName);
        
        for (it = scores.begin(); it != scores.end(); it++)
            file << it->second << ' ' << it->first << '\n';
        
        file.close();
    }
    
    void printScores() {
        for (it = scores.begin(); it != scores.end(); it++)
            cout << it->first << ' ' << it->second << '\n';
    }
};

#endif