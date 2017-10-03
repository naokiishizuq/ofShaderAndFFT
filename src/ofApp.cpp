#include "ofApp.h"

const int N = 256;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values

ofFloatImage spectrumImage;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    //    ofDisableArbTex();

    ofBackground(0);
    ofEnableDepthTest();
    
    cam.setDistance(400);
    
    camWidth = 640;
    camHeight = 480;
    
    //Load shader
    shader.load( "shaderVert.c", "shaderFrag.c" );
    
    createMesh();
    
//    ofDisableArbTex();
    myVidPlayer.load("images/asapFergPlainJane.mov");
    myVidPlayer.setVolume(0.0);
    myVidPlayer.play();

//    myVideo.initGrabber(640, 480);
//    ofEnableArbTex();
//    ofDisableArbTex();
    fbo.allocate(camWidth, camHeight);
    fbo2.allocate(camWidth, camHeight);
//    ofEnableArbTex();
    
    //Set up sound sample
    sound.loadSound("images/asapFergPlainJane.mp3" );
    sound.setLoop(true);
    sound.play();
    
    //Set spectrum values to 0
    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    bool bNewFrame = false;

//    myVideo.update();
    myVidPlayer.update();
    tempImage.setFromPixels(myVidPlayer.getPixelsRef());
    tempImage.resize(camWidth, camHeight);
    bNewFrame = myVidPlayer.isFrameNew();
    if(bNewFrame){
        
    } else {
        
    }
    //Update sound engine
    ofSoundUpdate();
    
    //Get current spectrum with N bands
    float *val = ofSoundGetSpectrum( N );
    //We should not release memory of val,
    //because it is managed by sound engine
    
    //Update our smoothed spectrum,
    //by slowly decreasing its values and getting maximum with val
    //So we will have slowly falling peaks in spectrum
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.95; //0.97;	//Slow decreasing
        spectrum[i] = max( spectrum[i], val[i] );
    }
    
    //Set spectrum to spectrumImage
    spectrumImage.setFromPixels( spectrum, N, 1, OF_IMAGE_GRAYSCALE );
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    float time = ofGetElapsedTimef();
    ofBackground(0, 0);
    
    ////////////////////////////////////////
    // draw on FBO and copy to the screen //
    ////////////////////////////////////////
    // setup
    fbo.begin();
    ofClear(0, 0, 0, 0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw Video Mesh
    glPointSize(3);
    ofPushMatrix();
//    cam.begin();
//    ofScale(-1, 1, 1);
//    ofTranslate(-camWidth/2+stepSize/2, -camHeight/2+stepSize/2);//For Camera
//    ofTranslate(ofGetWidth()/2-camWidth/2, ofGetHeight()/2-camHeight/2); // For Non-Camera
//    ofTranslate(camWidth/2, camHeight/2);
    tempImage.getTexture().bind();
    mesh.draw();
    tempImage.getTexture().unbind();
    ofPopMatrix();
    // end
    fbo.end();


    ////////////////////////////////////////
    // draw on FBO2 and copy to the screen //
    ////////////////////////////////////////
    // setup
    fbo2.begin();
    ofBackground(0, 0, 0);
    float ang = time * 30;
    ofClear(0, 0, 0, 0);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    // draw two circles
    ofSetColor( 255, 255, 255 );
//    ofCircle(0, 200, 100);
    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofSetColor(100);
    ofPushMatrix();
//    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofTranslate(camWidth/2, camHeight/2);
//    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRotate( ang );
    ofRect(mouseX-camWidth/2, mouseY-camHeight, camWidth/2+arrowUpDown, camHeight/2+arrowRightLeft);
    ofPopMatrix();
    // end
    fbo2.end();
    
    // reset blending mode
    ofEnableAlphaBlending();
    
    //Draw Spectrum Image
//    ofSetColor( 255, 255, 255 );
//    spectrumImage.draw( 0, 0, N*4, 50 );
    
//    cam.begin();
    ///**
     //Enable shader
     shader.begin();
     //sending Textures and Uniform to Shader
//     shader.setUniformTexture( "texture1", fbo.getTextureReference(), 1 ); //"1" means that it is texture 1
     shader.setUniformTexture( "texture1", fbo2.getTextureReference(), 2 ); //"1" means that it is texture 1
     shader.setUniformTexture( "texture2", spectrumImage.getTextureReference(), 3 ); //"2" means that it is texture 2
     shader.setUniform1i( "N", N );
     shader.setUniform1fv( "specArray", spectrum, N );
     shader.setUniform1f( "camWidth", camWidth );
     shader.setUniform1f( "camHeight", camHeight );
     //**/
    
    //Draw FBO!!!
//    cam.begin();
    ofSetColor(255, 50);
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    fbo2.draw(ofGetWidth()/2, 0);
//    fbo2.draw(0, 0);
    
    ofSetColor(255);
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    fbo.draw(mouseX, mouseY);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2-camWidth/2, ofGetHeight()/2-camHeight/2); // For Non-Camera
    fbo.draw(0, 0);
    ofPopMatrix();
    
    shader.end();
    
//    cam.end();
    cout<< mouseX << "/" << mouseY << endl;
    //Information Draw
    ofSetColor(255);
    string info;
    info = "Vertex num = " + ofToString(camWidth * camHeight, 0) + "\n";
    info += "FPS = " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(info, 30, 30);

}

//--------------------------------------------------------------
void ofApp::createMesh(){
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    stepSize =4;
    ySteps = camHeight / stepSize;
    xSteps = camWidth / stepSize;
    for(int y = 0; y < ySteps; y++) {
        for(int x = 0; x < xSteps; x++) {
            mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
            mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
        }
    }
    for(int y = 0; y + 1 < ySteps; y++) {
        for(int x = 0; x + 1 < xSteps; x++) {
            int nw = y * xSteps + x;
            int ne = nw + 1;
            int sw = nw + xSteps;
            int se = sw + 1;
            mesh.addIndex(nw);
            mesh.addIndex(ne);
            mesh.addIndex(se);
            mesh.addIndex(nw);
            mesh.addIndex(se);
            mesh.addIndex(sw);
        }
    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
    if(key == 357) arrowUpDown+=90; //arrowUp
    if(key == 359) arrowUpDown-=90; //arrowUp
    if(key == 356) arrowRightLeft+=90; //arrowUp
    if(key == 358) arrowRightLeft-=90; //arrowUp
    cout << key << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}