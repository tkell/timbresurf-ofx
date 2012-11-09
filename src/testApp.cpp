#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(64, 64, 64);

	// Set up the OSC sender
	sender.setup(HOST, PORT);
    // Set up OSC listener so we can get the vis timing right
    oscRec.setup(23456);

    // Select the track and difficulty here:
    cout << "Please select a track\n";
    cout << "1:  Onefourfiveone - D\n";
    cin >> track_number;

    cout << "Please select a difficulty\n";
    cout << "1:  Bars\n";
    cout << "2:  Beats\n";
    cout << "3:  Tatums\n";
    cout << "4:  Segments\n";
    cin >> difficulty_rating;

    cout << "Loading...\n";
    // 1451 - D
    if (track_number == 1) {
            // This is for bars and beats and tatums  Will do for now
            maximums[0] = 261; maximums[1] = 142; maximums[3] = 162;
            maximums[4] = 107; maximums[5] = 104; maximums[6] = 65;

            minimums[0] = -61; minimums[1] = -157; minimums[3] = -98;
            minimums[4] = -37; minimums[5] = -108; minimums[6] = -64;

        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("1451.bars.timbre"); // reading into the buffer
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("1451.beats.timbre"); // reading into the buffer
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("1451.tatums.timbre"); // reading into the buffer
        }
    }

    // Get the ranges
    for (int i = 0; i < 6; i++) {
        ranges[i] = abs(maximums[i] - minimums[i]);
    }

    windowSizeX = 640;
    windowSizeY = 480;
    timbreIndex = 0;

    // Load the timbre data
    string line;
    line = buffer.getNextLine();
    while (line != "") {
        timbreData.push_back(ofToInt(line));
        line = buffer.getNextLine();
    }

    cout << "Loaded. Please boot the ChucK app\n";
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
    // GOD I NEED TO MAKE THESE FUNCTIONS
    int nextScaledTimbreX = (int)((timbreData[timbreIndex] - minimums[0]) * ((float)windowSizeX / (float)ranges[0]) );
    int nextScaledTimbreY = (int)((timbreData[timbreIndex + 1] - minimums[1]) * ((float)windowSizeY / (float)ranges[1]));
    ofSetColor(255,0,0);
    ofCircle(nextScaledTimbreX,nextScaledTimbreY,25);  

    if (timbreIndex -6 >= 0) {
        int oldScaledTimbreX = (int)((timbreData[timbreIndex-6] - minimums[0]) * ((float)windowSizeX / (float)ranges[0]));
        int oldScaledTimbreY = (int)((timbreData[timbreIndex-6 + 1] - minimums[1]) * ((float)windowSizeY / (float)ranges[1]));
        ofSetColor(0,0,255);
        ofCircle(oldScaledTimbreX,oldScaledTimbreY,12);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 32) {
        timbreIndex = timbreIndex + 6;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

// --------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    // Need to scale things to the timbres here
    // this is super hacky because I'll have to do kinect scaling properly later

	ofxOscMessage sendLocationData;
	int scaledX = (int)(x * ((float)ranges[0] / (float)windowSizeX) + minimums[0]);
    int scaledY = (int)(y * ((float)ranges[1] / (float)windowSizeY) + minimums[1]);
    sendLocationData.setAddress("/mouse/position");
	sendLocationData.addIntArg(scaledX);
    sendLocationData.addIntArg(scaledY);
    // ofLog(OF_LOG_NOTICE, ofToString(scaledX));
	sender.sendMessage(sendLocationData);
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

