/*
 *  MIT License
 *  Copyright (c) 2010 Eberhard Gräther (http://egraether.multimediatechnology.at)
 *
 *  Salzburg University of Applied Sciences
 */

#include "Console.h"
#include "ofMain.h"

Console::Console() : 
    show(false) {
}

void Console::draw() {
    if (show) {
        ofSetColor(255, 255, 255);
        ofRect(0, ofGetHeight() - 50, 250, 50);
        ofRect(0, 0, 250, infos.size() * 20 + 25);
        ofRect(ofGetWidth() - 250, 0, 250, 30);
        
        ofSetColor(0, 0, 0);
        ofDrawBitmapString(it->name + " = " + ofToString(*it->value), 15, ofGetHeight() - 20);
        
        for (int i = 0; i < infos.size(); i++) {
            ofDrawBitmapString(infos[i].name + " " + ofToString(*infos[i].value), 15, 25 + i * 20);
        }
        
        ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), ofGetWidth() - 200, 20);
    }
} 

void Console::addInformation(string name, int* value) {
    infos.push_back(Information(name, value));
}

void Console::addRegulation(string name, int* value, int minValue, int maxValue) {
    values.push_back(Regulation(name, value, minValue, maxValue));
    it = values.begin();
    addInformation(name, value);
}

void Console::next(bool toNext) {
    if (show) {
        if (toNext) {
            if (it == --values.end())
                it = values.begin();
            else 
                it++;
        }
        else {
            if (it == values.begin())
                it = --values.end();
            else 
                it--;
        }
    }
}

void Console::change(bool increase) {
    if (show) {
        if (increase)
            *it->value = *it->value + 1 > it->maxValue ? it->maxValue : *it->value + 1;
        else
            *it->value = *it->value - 1 < it->minValue ? it->minValue : *it->value - 1;
    }
}