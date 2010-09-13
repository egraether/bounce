#ifndef _CONSOLE_
#define _CONSOLE_

#include <vector>
#include "ofMain.h"

class Console {
private:
    struct Argument {
        string name;
        int* value;
        int min, max;
        Argument(string n, int* v, int m, int x) : 
            name(n), value(v), min(m), max(x) {
        }
    };
    
    vector<Argument> values;
    vector<Argument>::iterator it;
    
public:
    bool show;
    
    Console();
    void draw();
    void add(string name, int* value, int min, int max);
    void next(bool toNext);
    void change(bool increase);
    //~Console() {}
};

#endif

