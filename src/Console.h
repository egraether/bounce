#ifndef _CONSOLE_
#define _CONSOLE_

#include <vector>
#include "ofMain.h"

class Console {
private:
    struct Information {
        string name;
        int* value;
        Information(string n, int* v) : 
            name(n), value(v) {
        }
    };
    
    struct Regulation {
        string name;
        int* value;
        int min, max;
        Regulation(string n, int* v, int m, int x) : 
            name(n), value(v), min(m), max(x) {
        }
    };
    
    vector<Information> infos;
    vector<Regulation> values;
    vector<Regulation>::iterator it;
    
public:
    bool show;
    
    Console();
    void draw();
    void addInformation(string name, int* value);
    void addRegulation(string name, int* value, int min, int max);
    void next(bool toNext);
    void change(bool increase);
    //~Console() {}
};

#endif

