#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(16, 16, 16);
    ofSetSmoothLighting(true);
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

    cout << "Enter V to visualize, any other key to play\n";
    cin >> attract_mode;

    cout << "Loading...\n";
    // 1451 - D
    if (track_number == 1) {
            // This is for bars and beats and tatums  Will do for now
            maximums[0] = 261; maximums[1] = 142; maximums[2] = 162;
            maximums[3] = 107; maximums[4] = 104; maximums[5] = 65;

            minimums[0] = -61; minimums[1] = -157; minimums[2] = -98;
            minimums[3] = -37; minimums[4] = -108; minimums[5] = -64;

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


    // Set up our new, 3D world!
    windowSizeX = 1024;
    windowSizeY = 768;
    windowSizeZ = 768;
    sphereRadius = 5;
    
    // Load the timbre data
    timbreIndex = 0;
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
    // move these guys out?
    ofPoint p1; 
    ofPoint p2; 

    ofEnableLighting();
    light.enable();

    // Clear the draw vectors, unless we're in attract mode
    if (attract_mode != 'V') {
        spheresRH.clear();
        spheresLH.clear();
    }

    // 0, 0, 0 is now center, not top-left
    // And we rotate, just a little
    cam.begin();

    ofRotateX(ofRadToDeg(.25));
	ofRotateY(ofRadToDeg(-.25));

    ofPushMatrix();
    // Draw X, Y, Z axes here
    ofSetColor(0, 255, 0);
    p1.set(-windowSizeX * 8 ,0,0); 
    p2.set(windowSizeX * 8, 0, 0); 
    ofLine(p1, p2);

    p1.set(0, -windowSizeY * 8, 0); 
    p2.set(0, windowSizeY * 8, 0); 
    ofLine(p1, p2);

    p1.set(0, 0, -windowSizeZ * 8); 
    p2.set(0, 0, windowSizeZ * 8); 
    ofLine(p1, p2);

    // We'll get these from the kinect once we've got it up
    //    int currentRHX = 0;
    //    int currentRHY = 0;
    //    int currentRHZ = 0;

    //    int currentLHX = 0;
    //    int currentLHY = 0;
    //    int currentLHZ = 0;

    // GOD I NEED TO MAKE THESE FUNCTIONS
    // Draw red for right hand!
    int nextScaledTimbreRHX = (int) ((timbreData[timbreIndex] - ((maximums[0] + minimums[0]) / 2)) * ((float)windowSizeX / (float)ranges[0]) );
    int nextScaledTimbreRHY = (int) ((timbreData[timbreIndex + 1] - ((maximums[1] + minimums[1]) / 2)) * ((float)windowSizeY / (float)ranges[1]));
    int nextScaledTimbreRHZ = (int) ((timbreData[timbreIndex + 2] - ((maximums[2] + minimums[2]) / 2)) * ((float)windowSizeZ / (float)ranges[2]));

    p1.set(nextScaledTimbreRHX, nextScaledTimbreRHY, nextScaledTimbreRHZ);
    spheresRH.push_back(p1);

    ofSetColor(255, 0, 0);
    for (int i=0; i< spheresRH.size(); i++) {
        ofSphere(spheresRH[i], sphereRadius);
    }

    // Draw blue for left hand!
    int nextScaledTimbreLHX = (int)((timbreData[timbreIndex-6 + 3] - ((maximums[3] + minimums[3]) / 2)) * ((float)windowSizeX / (float)ranges[3]));
    int nextScaledTimbreLHY = (int)((timbreData[timbreIndex-6 + 4] - ((maximums[4] + minimums[4]) / 2)) * ((float)windowSizeY / (float)ranges[4]));
    int nextScaledTimbreLHZ = (int)((timbreData[timbreIndex-6 + 5] - ((maximums[5] + minimums[5]) / 2)) * ((float)windowSizeZ / (float)ranges[5]));
    
    p2.set(nextScaledTimbreLHX, nextScaledTimbreLHY, nextScaledTimbreLHZ);
    spheresLH.push_back(p2);

    ofSetColor(0, 0, 255);
    for (int i=0; i< spheresLH.size(); i++) {
        ofSphere(spheresLH[i], sphereRadius);
    }

    ofPopMatrix();
	cam.end();

    ofDisableLighting();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    // Press space to debug things forward in time
    if (key == 32) {
        timbreIndex = timbreIndex + 6 % timbreData.size();   
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

