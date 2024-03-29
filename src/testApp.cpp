#include "testApp.h"
//--------------------------------------------------------------
void testApp::setup(){
	// Set up the OSC sender
	sender.setup(HOST, PORT);
    // Set up OSC listener so we can get the vis timing right
    oscRec.setup(23456);

    // Set up the kinect lisenter
    kinectRec.setup(7110);

    // Select the track and difficulty here:
    cout << "Please select a track\n";
    cout << "1:  Onefourfiveone - D\n";
    cout << "2:  Michael Jackson - Billie Jean\n";
    cout << "3:  Mozart - Enie Kleine Nachtmusik - I\n";
    cout << "4:  The Beatles - A Hard Day's Night\n";
    cout << "5:  Tangerine Dream - Rising Runner Missed By Endless Sender\n";
    cin >> track_number;

    cout << "Please select a difficulty\n";
    cout << "1:  Bars\n";
    cout << "2:  Beats\n";
    cout << "3:  Tatums\n";
    cout << "4:  Segments\n";
    cin >> difficulty_rating;

    cout << "Enter V to visualize, E to explore, P to play\n";
    cin >> mode;

    cout << "Loading...\n";

    // This is all four songs.
    maximums[0] = 273; maximums[1] = 145; maximums[2] = 300;
    maximums[3] = 125; maximums[4] = 272; maximums[5] = 89;

    minimums[0] = -129; minimums[1] = -232; minimums[2] = -245;
    minimums[3] = -50; minimums[4] = -154; minimums[5] = -71;

    // 1451 - D
    if (track_number == 1) {
        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("1451.bars.timbre"); // reading into the buffer
            sphereRadius = 10;
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("1451.beats.timbre"); // reading into the buffer
            sphereRadius = 5;
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("1451.tatums.timbre"); // reading into the buffer
            sphereRadius = 2;
        }
        else if (difficulty_rating == 4) {
            buffer = ofBufferFromFile("1451.segments.timbre"); // reading into the buffer
            sphereRadius = 1;
        }
    }

     // Michael Jackson - Billie Jean
    if (track_number == 2) {
        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("billiejean.bars.timbre"); // reading into the buffer
            sphereRadius = 10;
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("billiejean.beats.timbre"); // reading into the buffer
            sphereRadius = 5;
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("billiejean.tatums.timbre"); // reading into the buffer
            sphereRadius = 2;
        }
        else if (difficulty_rating == 4) {
            buffer = ofBufferFromFile("billiejean.segments.timbre"); // reading into the buffer
            sphereRadius = 1;
        }
    }

    // Mozart - Eine Kleine
    if (track_number == 3) {
        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("einekleine.bars.timbre"); // reading into the buffer
            sphereRadius = 10;
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("einekleine.beats.timbre"); // reading into the buffer
            sphereRadius = 5;
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("einekleine.tatums.timbre"); // reading into the buffer
            sphereRadius = 2;
        }
        else if (difficulty_rating == 4) {
            buffer = ofBufferFromFile("einekleine.segments.timbre"); // reading into the buffer
            sphereRadius = 1;
        }
    }

    // Beatles - Hard Day's Night
    if (track_number == 4) {
        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("harddaysnight.bars.timbre"); // reading into the buffer
            sphereRadius = 10;
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("harddaysnight.beats.timbre"); // reading into the buffer
            sphereRadius = 5;
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("harddaysnight.tatums.timbre"); // reading into the buffer
            sphereRadius = 2;
        }
        else if (difficulty_rating == 4) {
            buffer = ofBufferFromFile("harddaysnight.segments.timbre"); // reading into the buffer
            sphereRadius = 1;
        }
    }

    // Tangerine Dream - Rising Runner Missed By Endless Sender
    if (track_number == 5) {
        if (difficulty_rating == 1) {
            buffer = ofBufferFromFile("tangerine.bars.timbre"); // reading into the buffer
            sphereRadius = 10;
        }
        else if (difficulty_rating == 2) {
            buffer = ofBufferFromFile("tangerine.beats.timbre"); // reading into the buffer
            sphereRadius = 5;
        }
        else if (difficulty_rating == 3) {
            buffer = ofBufferFromFile("tangerine.tatums.timbre"); // reading into the buffer
            sphereRadius = 2;
        }
        else if (difficulty_rating == 4) {
            buffer = ofBufferFromFile("tangerine.segments.timbre"); // reading into the buffer
            sphereRadius = 1;
        }
    }


    // Get the ranges
    for (int i = 0; i < 6; i++) {
        ranges[i] = abs(maximums[i] - minimums[i]);
    }
    transX = 0;
    transY = 0;
    transZ = 0;
    minimumIndex = 0;

    // Set up our new, 3D world!
    // 0, 0, 0 is now center, not top-left
    windowSizeX = 1024;
    windowSizeY = 768;
    windowSizeZ = 768;
    ofBackground(0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    // Move the camera so we can see everything
    cam.setDistance(windowSizeZ * 1);
    cam.move(windowSizeX / 16, windowSizeY / 16, 0);
    cam.pan(5);
    ofSetSmoothLighting(true);
    light.setPosition(0, 0, windowSizeZ);
    
    // Load the timbre data
    timbreIndex = 0;
    string line;
    line = buffer.getNextLine();
    while (line != "") {
        timbreData.push_back(ofToInt(line));
        line = buffer.getNextLine();
    }

    // If we're exploring, load ALL THE THINGS
    if (mode == 'E') {
        for (int i = 0; i < timbreData.size(); i+=6) {
            int nextScaledTimbreRHX = (int) ((timbreData[i] - ((maximums[0] + minimums[0]) / 2)) * ((float)windowSizeX / (float)ranges[0]) );
            int nextScaledTimbreRHY = (int) ((timbreData[i + 1] - ((maximums[1] + minimums[1]) / 2)) * ((float)windowSizeY / (float)ranges[1]));
            int nextScaledTimbreRHZ = (int) ((timbreData[i + 2] - ((maximums[2] + minimums[2]) / 2)) * ((float)windowSizeZ / (float)ranges[2]));
            p1.set(nextScaledTimbreRHX, nextScaledTimbreRHY, nextScaledTimbreRHZ);
            spheresRH.push_back(p1);

            int nextScaledTimbreLHX = (int)((timbreData[i + 3] - ((maximums[3] + minimums[3]) / 2)) * ((float)windowSizeX / (float)ranges[3]));
            int nextScaledTimbreLHY = (int)((timbreData[i + 4] - ((maximums[4] + minimums[4]) / 2)) * ((float)windowSizeY / (float)ranges[4]));
            int nextScaledTimbreLHZ = (int)((timbreData[i + 5] - ((maximums[5] + minimums[5]) / 2)) * ((float)windowSizeZ / (float)ranges[5]));
            p2.set(nextScaledTimbreLHX, nextScaledTimbreLHY, nextScaledTimbreLHZ);
            spheresLH.push_back(p2);
        }
    }

    cout << "Loaded. Please boot the ChucK app\n";
}

//--------------------------------------------------------------
void testApp::update(){
    // Update the visuals
    while(oscRec.hasWaitingMessages()) {
        ofxOscMessage m;
        oscRec.getNextMessage(&m);
        if (m.getAddress() == "/visuals/tick") {
            timbreIndex = timbreIndex + 6 % timbreData.size();  
        }
        if (m.getAddress() == "minimum/index") {
            minimumIndex = m.getArgAsInt32(0);    
        }
    }
    // Get location data
    while(kinectRec.hasWaitingMessages()) {
        ofxOscMessage m;
        kinectRec.getNextMessage(&m);

        // what the fuck, kinect.
        // The bad crazy:  my hands appear to be REVERSED.  
        // so:  
        if (m.getAddress() == "/joint") {
            if (m.getArgAsString(0) == "r_hand") {
                currentLHY = m.getArgAsFloat(3);
            }
            else if (m.getArgAsString(0) == "l_hand") {
                currentRHX = m.getArgAsFloat(2);
                currentRHY = m.getArgAsFloat(3);
                currentRHZ = m.getArgAsFloat(4);
            }
           else if (m.getArgAsString(0) == "torso") {
                currentTorsoX = m.getArgAsFloat(2);
                currentTorsoY = m.getArgAsFloat(3);
                currentTorsoZ = m.getArgAsFloat(4);
            }
        }
    }

    // Scale
    float newX = (currentRHX - currentTorsoX) * windowSizeX * 3;
    float newY = ((1 - currentTorsoY) - (1 - currentRHY)) * windowSizeY * -3;
    float newZ = (currentRHZ - (currentTorsoZ * 0.825)) * windowSizeZ * 3;

    float newVolume = (1 - currentLHY) * 1.5;
    if (newVolume > 1.0) {
        newVolume = 1.0;
    }
    ofLog(OF_LOG_NOTICE, ofToString(newZ));

    // Smooth
    transX = (int) ((transX * 0.5 + newX ) / 1.5);
    transY = (int) ((transY * 0.5 + newY) / 1.5);
    transZ = (int) ((transZ * 0.5 + newZ) / 1.5);

    // Log & update
    //ofLog(OF_LOG_NOTICE, ofToString(transX));
    //ofLog(OF_LOG_NOTICE, ofToString(transY));
    ofLog(OF_LOG_NOTICE, ofToString(transZ));
    controlPoint.set(transX, transY, transZ);

    // Send
    ofxOscMessage sendLocationData;
	int scaledX = (int)(transX * ((float)ranges[0] / (float)windowSizeX) + (maximums[0] + minimums[0]) / 2);
    int scaledY = (int)(transY * ((float)ranges[1] / (float)windowSizeY) + (maximums[1] + minimums[1]) / 2);
    int scaledZ = (int)(transX * ((float)ranges[2] / (float)windowSizeZ) + (maximums[2] + minimums[2]) / 2);
    sendLocationData.setAddress("/position");

	sendLocationData.addIntArg(scaledX);
    sendLocationData.addIntArg(scaledY);
    sendLocationData.addIntArg(scaledZ);
    sendLocationData.addFloatArg(newVolume);
	sender.sendMessage(sendLocationData);


}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableLighting();
    light.enable();

    // Clear the draw vectors, unless we're in attract or explore modes
    if (mode != 'V' && mode != 'E') {
        spheresRH.clear();
        spheresLH.clear();
    }

    cam.begin();
    ofPushMatrix();
    // Draw X, Y, Z axes here
    ofSetColor(0, 255, 0);
    p1.set(-windowSizeX * 8, 0,0); 
    p2.set(windowSizeX * 8, 0, 0); 
    ofLine(p1, p2);

    p1.set(0, -windowSizeY * 8, 0); 
    p2.set(0, windowSizeY * 8, 0); 
    ofLine(p1, p2);

    p1.set(0, 0, -windowSizeZ * 8); 
    p2.set(0, 0, windowSizeZ * 8); 
    ofLine(p1, p2);

    ofSetColor(225, 225, 225);
    ofSphere(controlPoint, 20);


    // GOD I NEED TO MAKE THESE FUNCTIONS
        // Draw red for right hand!
    if (mode != 'E') {
        int nextScaledTimbreRHX = (int) ((timbreData[timbreIndex] - ((maximums[0] + minimums[0]) / 2)) * ((float)windowSizeX / (float)ranges[0]) );
        int nextScaledTimbreRHY = (int) ((timbreData[timbreIndex + 1] - ((maximums[1] + minimums[1]) / 2)) * ((float)windowSizeY / (float)ranges[1]));
        int nextScaledTimbreRHZ = (int) ((timbreData[timbreIndex + 2] - ((maximums[2] + minimums[2]) / 2)) * ((float)windowSizeZ / (float)ranges[2]));

        p1.set(nextScaledTimbreRHX, nextScaledTimbreRHY, nextScaledTimbreRHZ);
        spheresRH.push_back(p1);

        // CURRENTLY NOT DOING BLUE Draw blue for left hand!
//        int nextScaledTimbreLHX = (int)((timbreData[timbreIndex + 3] - ((maximums[3] + minimums[3]) / 2)) * ((float)windowSizeX / (float)ranges[3]));
//        int nextScaledTimbreLHY = (int)((timbreData[timbreIndex + 4] - ((maximums[4] + minimums[4]) / 2)) * ((float)windowSizeY / (float)ranges[4]));
//        int nextScaledTimbreLHZ = (int)((timbreData[timbreIndex + 5] - ((maximums[5] + minimums[5]) / 2)) * ((float)windowSizeZ / (float)ranges[5]));
//        
//        p2.set(nextScaledTimbreLHX, nextScaledTimbreLHY, nextScaledTimbreLHZ);
//        spheresLH.push_back(p2);
    }

    for (int i=0; i < spheresRH.size(); i++) {
        if (i == minimumIndex) {
            ofSetColor(255, 0, 0);
            ofSphere(spheresRH[i], sphereRadius * 4);
            ofSetColor(225, 225, 225);
            ofLine(controlPoint, spheresRH[i]);
        }
        else {
            ofSetColor(127, 0, 0);
            ofSphere(spheresRH[i], sphereRadius);
        }
    }
// CURRENTLY NOT DOING BLUE 
//    ofSetColor(0, 0, 127);
//    for (int i=0; i< spheresLH.size(); i++) {
//        ofSphere(spheresLH[i], sphereRadius);
//    }

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

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

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

