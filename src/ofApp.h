#pragma once

#include "ofMain.h"

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
    void createMesh();
//    void updateMesh();
    
    ofFbo fbo;		//buffer for drawing screen
    ofFbo fbo2;		//second buffer for drawing screen
    ofVboMesh mesh; //
    
	ofEasyCam cam;
    
    ofVideoGrabber myVideo;
    ofImage tempImage;
    ofVideoPlayer 		myVidPlayer;

    ofSoundPlayer sound;	//Sound sample
    
    ofShader shader;	//Shader

    
	int camWidth, camHeight;
    int stepSize, xSteps, ySteps;
    
    int arrowUpDown;
//    int arrowDown;
    int arrowRightLeft;
//    int arrowUpLeft;
};
