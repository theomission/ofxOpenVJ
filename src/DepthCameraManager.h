//
//  DepthCameraManager.h
//  ofxOpenVJ
//
//  Created by Ben McChesney 7/29/2013
//	abstract class that allows for scenes to be cross-hardware
//

#pragma once
#include "ofMain.h"
#include "Constants.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"

class DepthCameraManager {
public:
    DepthCameraManager();
    
    virtual void open();
    virtual void close();
    virtual bool isConnected();
    
    virtual void update();
    virtual void draw();
    virtual void drawDebug();
    
    virtual bool isFrameNew();
    virtual float* getDistancePixels();
    virtual ofVec3f getWorldCoordAt( int x, int y );
    virtual void setWorldCoord( int x, int y, ofVec3f& inVec );
    
    virtual void setupMesh();
    virtual void calculateMesh( ofVec3f mesh_offset );
    virtual void calculatePoints( ofVec3f a_meshOffset );
    virtual void calculateTriangleMesh( ofVec3f mesh_offset, bool bCalcNormals=true );
    virtual void calculateTriangleStripMesh( ofVec3f mesh_offset, bool bCalcNormals=true );
    virtual ofVec3f getMeshCenter();
    
    virtual ofVec3f& getOffsetVector();
    
    virtual void calculateCVOperations();
    virtual vector<ofPolyline> getScaledContourPolyline( float x, float y, float w, float h );
    virtual vector<ofRectangle> getScaledContourBoundingBoxes( float x, float y, float w, float h );
    
    virtual void guiEvent(ofxUIEventArgs &e);
    virtual void setupGui(float a_x=0, float a_y=0);
    
    virtual void loadSettings();
    virtual void saveSettings();
    

    ofxUIScrollableCanvas* gui;
    

    // open cv work, for use with 2D elements //
    ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	ofxCvContourFinder contourFinder;
    
    bool* bPointsActive;
    
    int                 nearThreshold, farThreshold;
    int                 nearThreshCV, farThreshCV;
    
    ofMesh              mesh;
    int                 width, height;
    
    float               scale;
    int                 step;
    bool                bUseVideoColor;
    // how far can the pixels be before it is considered fucked? //
    float               maxTriDiff;
    
    ofVec3f             offsetVec;
    
    //Global settings for point clouds
    float               pointCloudMaxZ , pointCloudMinZ ;
    float               pointCloudZOffset ;
    bool                bFindHoles ;
    float               minBlobSize , maxBlobSize ;
    
    //In dark rooms the colors don't get picked up as well so we have to adjust
    //the raw RGB color
    unsigned char       minimumPixBrightness ;
    
    //ofxPostProcessing post ;
    
    void disableAllPostProcessing() ; 
    
protected:
    ofVec3f inverseAxes;
    // in degrees //
    ofVec3f axesRotation;
    bool bThreshWithOpenCV;
};