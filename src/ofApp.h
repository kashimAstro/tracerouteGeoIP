#pragma once

#include "ofMain.h"
#include "ofxStreetView.h"
#include "Map.h"
#include "OpenStreetMapProvider.h"
#include "GeoUtils.h"
#include "ofxUI.h"
#include "threadedObject.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
		
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void setCamera();

    ofEasyCam cam;
    ofxStreetView streetview;
    ofTexture texture;
    Map map;
    ofxUISuperCanvas *gui;
    ofxUITextInput *textInput;
    void guiEvent(ofxUIEventArgs &e);
    threadedObject thread;
};
