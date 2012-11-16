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
        ofBuffer buffer;
    
        int track_number;
        int difficulty_rating;
        char attract_mode;
        int sphereRadius;
        vector<ofPoint> spheresRH;
        vector<ofPoint> spheresLH;

        int maximums[6];
        int minimums[6];
        int ranges[6];

        int windowSizeX;
        int windowSizeY;
        int windowSizeZ;

        int timbreIndex;
        vector<int> timbreData;
};

