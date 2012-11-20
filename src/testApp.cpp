#include "testApp.h"
//--------------------------------------------------------------
void testApp::setup(){
	// Set up the OSC sender
	sender.setup(HOST, PORT);
    // Set up OSC listener so we can get the vis timing right
    oscRec.setup(23456);

    // Select the track and difficulty here:
    cout << "Please select a track\n";
    cout << "1:  Onefourfiveone - D\n";
    cout << "2:  Michael Jackson - Billie Jean\n";
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

    // This is everything from 1451 and Billie Jean
    maximums[0] = 273; maximums[1] = 145; maximums[2] = 210;
    maximums[3] = 125; maximums[4] = 272; maximums[5] = 89;

    minimums[0] = -69; minimums[1] = -232; minimums[2] = -228;
    minimums[3] = -38; minimums[4] = -131; minimums[5] = -71;

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

    // Get the ranges
    for (int i = 0; i < 6; i++) {
        ranges[i] = abs(maximums[i] - minimums[i]);
    }

    // Set up our new, 3D world!
    // 0, 0, 0 is now center, not top-left
    windowSizeX = 1024;
    windowSizeY = 768;
    windowSizeZ = 768;
    ofBackground(0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    // Move the camera so we can see everything
    cam.setDistance(windowSizeZ * 1.5);
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
    // Listen for new osc message
    while(oscRec.hasWaitingMessages()) {
        ofxOscMessage m;
        oscRec.getNextMessage(&m);
        timbreIndex = timbreIndex + 6 % timbreData.size();   
    }
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
    ofSphere(controlPoint, sphereRadius);

    // We'll get these from the kinect once we've got it up
    //    int currentRHX = 0;
    //    int currentRHY = 0;
    //    int currentRHZ = 0;

    //    int currentLHX = 0;
    //    int currentLHY = 0;
    //    int currentLHZ = 0;

    // GOD I NEED TO MAKE THESE FUNCTIONS
        // Draw red for right hand!
    if (mode != 'E') {
        int nextScaledTimbreRHX = (int) ((timbreData[timbreIndex] - ((maximums[0] + minimums[0]) / 2)) * ((float)windowSizeX / (float)ranges[0]) );
        int nextScaledTimbreRHY = (int) ((timbreData[timbreIndex + 1] - ((maximums[1] + minimums[1]) / 2)) * ((float)windowSizeY / (float)ranges[1]));
        int nextScaledTimbreRHZ = (int) ((timbreData[timbreIndex + 2] - ((maximums[2] + minimums[2]) / 2)) * ((float)windowSizeZ / (float)ranges[2]));

        p1.set(nextScaledTimbreRHX, nextScaledTimbreRHY, nextScaledTimbreRHZ);
        spheresRH.push_back(p1);

        // Draw blue for left hand!
        int nextScaledTimbreLHX = (int)((timbreData[timbreIndex + 3] - ((maximums[3] + minimums[3]) / 2)) * ((float)windowSizeX / (float)ranges[3]));
        int nextScaledTimbreLHY = (int)((timbreData[timbreIndex + 4] - ((maximums[4] + minimums[4]) / 2)) * ((float)windowSizeY / (float)ranges[4]));
        int nextScaledTimbreLHZ = (int)((timbreData[timbreIndex + 5] - ((maximums[5] + minimums[5]) / 2)) * ((float)windowSizeZ / (float)ranges[5]));
        
        p2.set(nextScaledTimbreLHX, nextScaledTimbreLHY, nextScaledTimbreLHZ);
        spheresLH.push_back(p2);
    }

    ofSetColor(255, 0, 0);
    for (int i=0; i< spheresRH.size(); i++) {
        ofSphere(spheresRH[i], sphereRadius);
    }

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
    // Translate to center for my mouse-based control point
    int transX = x - windowSizeX / 2;
    int transY = -1 * (y - windowSizeY / 2);
    controlPoint.set(transX, transY, controlPoint.z);

	// Need to scale things to the timbres here
    // This will have to change once I get the kinect / leap in
    ofxOscMessage sendLocationData;
	int scaledX = (int)(transX * ((float)ranges[0] / (float)windowSizeX) + (maximums[0] + minimums[0]) / 2);
    int scaledY = (int)(transY * ((float)ranges[1] / (float)windowSizeY) + (maximums[1] + minimums[1]) / 2);
    int scaledZ = (int)(controlPoint.z * ((float)ranges[2] / (float)windowSizeZ) + (maximums[2] + minimums[2]) / 2);
    sendLocationData.setAddress("/mouse/position");

	sendLocationData.addIntArg(scaledX);
    sendLocationData.addIntArg(scaledY);
    sendLocationData.addIntArg(scaledZ);
    //ofLog(OF_LOG_NOTICE, ofToString(scaledX));
    //ofLog(OF_LOG_NOTICE, ofToString(scaledY));
    //ofLog(OF_LOG_NOTICE, ofToString(scaledZ));

	sender.sendMessage(sendLocationData);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // This moves Z around, for now
    if (button == 3) {
        controlPoint.set(controlPoint.x, controlPoint.y, controlPoint.z - 50);
    }
    if (button == 4) {
        controlPoint.set(controlPoint.x, controlPoint.y, controlPoint.z + 50);
    }
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

