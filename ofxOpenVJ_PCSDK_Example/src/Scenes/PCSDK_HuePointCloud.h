//
//  TestScene.h
//  KinectPerformanceV01
//
//  Created by Ben Mcchesney on 11/6/12.
//
//

#pragma once 
#include "BaseScene.h"
#include "ofxTweenzor.h"

class PCSDK_HuePointCloud : public BaseScene {
public:
    PCSDK_HuePointCloud();
    PCSDK_HuePointCloud( int a_index, string a_name ):BaseScene( a_index, a_name ) {}
    ~PCSDK_HuePointCloud();
    
    void setup();
    void setupGui(float a_x=0, float a_y=0);
    void guiEvent(ofxUIEventArgs &e);
    
    float redrawAlpha ; 
    
    void update();
    void draw();
    void drawPointCloud( ) ;
    
    void activate();
    void deactivate();

	float extrudeDepth ;
    float extrudeNoiseStrength ;

    float meshHueTimeMultiplier ;
    float meshHueRangeMin , meshHueRangeMax ;
    
    float triangleSizeMax ; 
    float triangleSizeMin ; 
    ofShader shader ;
    
    float yColorMultipler ;
    float boxSize ;
    float pixelSampling ;
    float hueTimeMultiplier ;
    float rotationTimeMultiplier ;
    
    float hueOffsetRange ;
    float yColorOffset ; 

    
};
