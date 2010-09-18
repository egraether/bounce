#ifndef _INFOBOX_
#define _INFOBOX_

#include "PushButton.h"
#include "Vector.h"

#include <vector>
#include <cstring>

using namespace std;

class Infobox {
public:
    enum Type {CHECK, CHOICE};
    enum State {ALIVE, ONE_HIT, TWO_HIT, DEAD};
    
private:
    vector<string> message;
    int width, height;
    Type type;
    State state;
    PushButton one, two;
    
public:
    Infobox();
    //~Infobox();
    
    void set(string m, Type t);
    void draw();
    State checkState(bool hit, Vector &hitPoint);
    void kill();
};

#endif

