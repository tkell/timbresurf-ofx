#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofBackground(40, 100, 40);

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);


    // Let's open the timbre file here.  We'll hard-code the path, for now
    // Eventually, I should shell-script the startup process
    max1 = 202; // first-pass variables, will make an array later
    min1 = 0;
    range1 = abs(max1 - min1);
    windowSizeX = 640;

    timbreIndex = 0;

    string line;
    ofBuffer buffer = ofBufferFromFile("bars.timbre"); // reading into the buffer
    line = buffer.getNextLine();
    while (line != "") {
        timbreData.push_back(ofToInt(line));
        line = buffer.getNextLine();
    }

    // Set up OSC listener so we can get the vis timing right
    oscRec.setup(23456);

}

//--------------------------------------------------------------
void testApp::update(){
    // Listen for new osc message
    while(oscRec.hasWaitingMessages()) {
        ofxOscMessage m;
        oscRec.getNextMessage(&m);
        timbreIndex = timbreIndex + 6 % timbreData.size();
    }

}

//--------------------------------------------------------------
void testApp::draw(){
    // Scale to the window size
    int nextScaledTimbre = (int)(timbreData[timbreIndex] * ((float)windowSizeX / (float)range1));
    ofSetColor(255,0,0);
    ofCircle(nextScaledTimbre,240,5);  

    if (timbreIndex -6 >= 0) {
        int oldScaledTimbre = (int)(timbreData[timbreIndex -6] * ((float)windowSizeX / (float)range1));
        ofSetColor(0,0,255);
        ofCircle(oldScaledTimbre,240,5);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}


// --------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    // Need to scale things to be 0 to 202 here
    // this is super hacky because I'll have to do kinect scaling properly later

	ofxOscMessage sendX;
	int scaledX = (int)(x * ((float)range1 / (float)windowSizeX));
    sendX.setAddress("/mouse/position/x");
	sendX.addIntArg(scaledX);
    // ofLog(OF_LOG_NOTICE, ofToString(scaledX));
	sender.sendMessage(sendX);
    
    ofxOscMessage sendY;
    sendY.setAddress("/mouse/position/y");
	sendY.addIntArg(y);
    // ofLog(OF_LOG_NOTICE, ofToString(y));
	sender.sendMessage(sendY);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

