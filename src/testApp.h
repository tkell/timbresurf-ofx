#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------
class testApp : public ofBaseApp {

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

        ofEasyCam cam; 
        ofLight light;

		ofTrueTypeFont font;
		ofxOscSender sender;
        ofxOscReceiver oscRec;
        ofxOscReceiver kinectRec;
        ofBuffer buffer;
    
        int track_number;
        int difficulty_rating;
        char mode;
        int sphereRadius;

        ofPoint p1; 
        ofPoint p2;
        ofPoint controlPoint; 
        vector<ofPoint> spheresRH;
        vector<ofPoint> spheresLH;

        float currentRHX;
        float currentRHY;
        float currentRHZ;
        float currentLHX;
        float currentLHY;
        float currentLHZ;
        float currentTorsoX;
        float currentTorsoY;
        float currentTorsoZ;
        int transX;
        int transY;
        int transZ;
    

        int maximums[6];
        int minimums[6];
        int ranges[6];

        int windowSizeX;
        int windowSizeY;
        int windowSizeZ;

        int timbreIndex;
        vector<int> timbreData;
};

