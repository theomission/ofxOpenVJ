//
//  TestScene.cpp
//  KinectPerformanceV01
//
//  Created by Ben Mcchesney on 11/6/12.
//
//

#include "CircleMaskScene.h"

//--------------------------------------------------------------
CircleMaskScene::CircleMaskScene() {
    
}

//--------------------------------------------------------------
CircleMaskScene::~CircleMaskScene() {
    
}

//--------------------------------------------------------------
void CircleMaskScene::setup() {
    
    string path = ofToDataPath( "../../../../ofxOpenVJ/shaders/" ) ;
    //
    shader.load( path+"basic.vert", path+"CircleNoise.frag" ) ;
    
    //tring path = ofToDataPath( "../../../../ofxOpenVJ/shaders/" ) ;
    
    simpleMask.setup( path+"composite_rgb", ofRectangle( 0 , 0 , ofGetScreenWidth() , ofGetScreenHeight() )) ;
   // shader.load( "shader/basicVertex.vert" , "shader/hexagon.frag" ) ;
    fbo.allocate( ofGetScreenWidth() , ofGetScreenHeight() ) ;
    fbo.begin() ;
    ofClear( 1 , 1 , 1 , 0 ) ;
    fbo.end() ;
    
    maskFbo.allocate( ofGetScreenWidth() , ofGetScreenHeight() ) ;
    maskFbo.begin() ;
    ofClear( 1 , 1 , 1 , 0 ) ;
    maskFbo.end() ;                     
}

//--------------------------------------------------------------
void CircleMaskScene::setupGui(float a_x, float a_y) {
    // creates new gui and adds the name to it //
    BaseScene::setupGui(a_x, a_y);
    
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    
    // gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    //gui->addWidgetDown(new ofxUIRangeSlider(length, dim, 0 , 10000 , pointCloudMinZ , pointCloudMaxZ, "POINT CLOUD Z RANGE")) ;
    int width = 300 ;
    int height = 18 ;
    
    gui->addSlider( "Mask Fade Amount" , 0.0 , 255.0 , maskFadeAmount , width , height ) ;

    
    ofAddListener( gui->newGUIEvent, this, &CircleMaskScene::guiEvent );
}

//--------------------------------------------------------------
void CircleMaskScene::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "Mask Fade Amount" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        maskFadeAmount = slider->getScaledValue() ;
	}
}

//--------------------------------------------------------------
void CircleMaskScene::activate() {
    if(fbo.getWidth() != getWidth() || fbo.getHeight() != getHeight()) {
        fbo.allocate( getWidth() , getHeight(), GL_RGBA ) ;
        fbo.begin() ;
        ofClear( 1 , 1 , 1 , 0 ) ;
        fbo.end() ;
    }
}

//--------------------------------------------------------------
void CircleMaskScene::deactivate() {
    // turn off the gui //
    BaseScene::deactivate();
}

//--------------------------------------------------------------
void CircleMaskScene::update() {
        kinectMan->update();
    if(kinectMan->isFrameNew()) {
        kinectMan->calculateCVOperations();
        
     //   if( ( ofGetElapsedTimeMillis() - lastAddTime  ) > millisDelay ) {
     //       addContour();
            //cout << "lines.size() = " << lines.size() << " particles.size() = " << particles.size() << endl;
    
    }
}

//--------------------------------------------------------------
void CircleMaskScene::draw() {
    
   
    ofSetColor(255, 255, 255);
    
    fbo.begin() ;
    shader.begin() ;
    shader.setUniform1f("time", ofGetElapsedTimef() );
    shader.setUniform2f("resolution", getWidth()  , getHeight() );
    shader.setUniform1f("low", low ) ;
    shader.setUniform1f("mid", mid ) ;
    shader.setUniform1f("high", high ) ;
    ofSetColor( 255 ) ;
    ofRect( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;
    shader.end() ;
    fbo.end() ;
    
    maskFbo.begin() ;
    ofSetColor( 0 , 0 , 0, maskFadeAmount ) ;
    ofRect( 0 , 0 , ofGetWidth() , ofGetHeight() ) ;
    ofSetColor( 255 , 255 , 255 ) ;
    ofEnableBlendMode(OF_BLENDMODE_ADD ) ;
    //ofClear( 1 , 1 , 1 , 0 ) ;
    kinectMan->grayImage.draw( 0  , 0 , getWidth() , getHeight() ) ;
    ofEnableAlphaBlending() ;
    maskFbo.end() ;
    ofSetColor( 255 ) ;
    
    simpleMask.drawMask( fbo.getTextureReference() , maskFbo.getTextureReference() , 0, 0, 1.0f ) ;
    
 
}

//--------------------------------------------------------------
void CircleMaskScene::drawDebug() {
    
}
















