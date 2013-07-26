//
//  TestScene.cpp
//  KinectPerformanceV01
//
//  Created by Ben Mcchesney on 11/6/12.
//
//

#include "HuePointCloudScene.h"

//--------------------------------------------------------------
HuePointCloudScene::HuePointCloudScene() {
    
}

//--------------------------------------------------------------
HuePointCloudScene::~HuePointCloudScene() {
    
}

//--------------------------------------------------------------
void HuePointCloudScene::setup() {

    meshHueTimeMultiplier = 0.0f ;
    bToggleTrails = false ;
    extrudeDepth = 100.0f ;
    extrudeNoiseStrength = 0.0f ;
    redrawAlpha = 0.0f ;

    sceneScale = 0.0f ; 

     string path = ofToDataPath( "../../../../ofxOpenVJ/shaders/" ) ;
    shader.load( path + "basic.vert", path + "HuePointCloud.frag" ) ;
}

//--------------------------------------------------------------
void HuePointCloudScene::setupGui(float a_x, float a_y) {
    // creates new gui and adds the name to it //
    BaseScene::setupGui(a_x, a_y);
    
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    
    int width = 300 ;
    int height = 25 ;
    gui->addWidgetDown(new ofxUILabel("KINECT PARAMS", OFX_UI_FONT_MEDIUM ));
  
    gui->addSlider( "MESH TIME HUE MULTIPLIER"  ,  0.0f , 255.0f , meshHueTimeMultiplier , width , height ) ;
    gui->addSlider( "FBO FADE AMOUNT"  , 0.0f , 255.0f , fboFadeAmount , width , height) ;
    gui->addSlider( "EXTRUDE DEPTH"  , 0.0f , 10.0f , extrudeDepth , width , height) ;
    gui->addSlider( "EXTRUDE NOISE STRENGTH"  , 0.0f , 500.0f , extrudeNoiseStrength , width , height) ;
    gui->addSlider( "REDRAW ALPHA"  , 0.0f, 255.0f , redrawAlpha , width , height) ;
    gui->addSlider( "SCENE SCALE"  , -5.0f , 5.0f , sceneScale , width , height) ;

    ofAddListener( gui->newGUIEvent, this, &HuePointCloudScene::guiEvent );
}

//--------------------------------------------------------------
void HuePointCloudScene::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "MESH TIME HUE MULTIPLIER" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        meshHueTimeMultiplier = slider->getScaledValue() ;
	}
    
    if(name == "FBO FADE AMOUNT" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        fboFadeAmount = slider->getScaledValue() ;
	}
    
    if ( name == "TOGGLE TRAILS")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        bToggleTrails = toggle->getValue() ;
    }
    
    if (name == "EXTRUDE DEPTH" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        extrudeDepth = slider->getScaledValue() ;
	}
    
    if (name == "EXTRUDE NOISE STRENGTH" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        extrudeNoiseStrength = slider->getScaledValue() ;
	}
  
    if (name == "PC Z" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        pointCloudOffset.z = slider->getScaledValue() ;
	}
    
    if (name == "REDRAW ALPHA" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        redrawAlpha = slider->getScaledValue() ;
	}
    
    if (name == "SCENE SCALE" )
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        sceneScale = slider->getScaledValue() ;
	}

}

//--------------------------------------------------------------
void HuePointCloudScene::activate() {
    if(trailFbo.getWidth() != getWidth() || trailFbo.getHeight() != getHeight()) {
        trailFbo.allocate( getWidth() , getHeight(), GL_RGBA ) ;
        trailFbo.begin() ;
        ofClear( 1 , 1 , 1 , 0 ) ;
        trailFbo.end() ;
    }
}

//--------------------------------------------------------------
void HuePointCloudScene::deactivate() {
    
    BaseScene::deactivate();
    
    trailFbo.begin() ;
        ofClear ( 1 , 1 , 1 , 0 ) ; 
    trailFbo.end( ) ;
    
}

//--------------------------------------------------------------
void HuePointCloudScene::update() {
    kinectMan->update( );
}

//--------------------------------------------------------------
void HuePointCloudScene::draw() {
    
  //   if ( bToggleTrails )
    
    trailFbo.begin () ;
    
    if ( bToggleTrails )
    {
        ofEnableAlphaBlending() ; 
        ofSetColor( 0 , 0 , 0, fboFadeAmount ) ;
        ofRect( 0 , 0, getWidth() , getHeight() ) ;
            }
    else
    {
        ofClear( 1 , 1 , 1 , 0 ) ;
    }
    
    ofSetColor( 255 , 255 ,255 ) ;
    
    //cameraMan->setTarget( pointCloudOffset );
    cameraMan->begin();
    ofPushMatrix() ;
        
    ofScale( sceneScale , sceneScale , 1.0f ) ;
        drawPointCloud();
    ofPopMatrix() ;
    cameraMan->end();
    
  
    ofBlendMode( OF_BLENDMODE_ADD ) ;
    ofSetColor( 255 , 255 , 255 , redrawAlpha ) ;
    trailFbo.draw(0 , 0 ) ;
    ofEnableAlphaBlending() ;

   
    trailFbo.end() ;
    
    ofSetColor( 255 , 255 , 255 ) ;
    ofPushMatrix( ) ;
    ofTranslate( 0 , getHeight() ) ;
    ofScale( 1 , -1 , 1 ) ;
    
    
    ofBlendMode( OF_BLENDMODE_ALPHA ) ;
    trailFbo.draw( 0 , 0 ) ;

    
    ofPopMatrix( ) ;
 
}

void HuePointCloudScene::drawPointCloud( )
{
    
    int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_LINES );
	int step = 2;
    //Get out hue offset
    int timeHue = ofGetElapsedTimef() * meshHueTimeMultiplier ;
    timeHue %= 255 ;
    
    ofColor hueOffset = ofColor::fromHsb( timeHue , 600.0f , 600.0f ) ;
	
    float _time = ofGetElapsedTimef() ; 
    float theta = sin ( ofGetElapsedTimef() ) + ofNoise( ofGetElapsedTimef() ) ;
    
    float maxZ = kinectMan->pointCloudMaxZ ;
    float minZ = kinectMan->pointCloudMinZ ; 
    for(int y = 0; y < h; y += step)
    {
		for(int x = 0; x < w; x += step)
        {
            
            float   noiseStep = mid * ofNoise( _time + x ) * extrudeNoiseStrength ; //* ( bassAmplitude + 0.2 )  *
            
            float lineLength = ( low + 0.25 ) * 200.0f ;
            float dLineLength = lineLength * 2.0f ;
            
			if( kinectMan->kinect.getDistanceAt(x, y) > 0)
            {
                ofVec3f vertex = kinectMan->kinect.getWorldCoordinateAt(x, y) ;
                if ( vertex.z > minZ && vertex.z < maxZ )
                {
                    float zOffset = noiseStep ;
                    //mesh.addVertex( vertex ) ;
                    vertex.z += zOffset ;
                   
                    ofColor col = kinectMan->kinect.getColorAt( x , y ) + hueOffset ;
                    
                    ofVec3f _v = vertex ;
                    
                    vertex.x += sin ( theta ) * lineLength ;
                    vertex.y -= cos ( theta ) * lineLength ;
                    mesh.addVertex( vertex );
                    mesh.addColor( col );
                    
                    vertex.x -= sin ( theta ) * dLineLength ;
                    vertex.y += cos ( theta ) * dLineLength ;
                    mesh.addVertex( vertex );
                    mesh.addColor( col );
                                  
                    _v.x -= sin ( theta ) * lineLength ;
                    _v.y -= cos ( theta ) * lineLength ;
                    mesh.addVertex( _v );
                    mesh.addColor( col );
                    
                    _v.x += sin ( theta ) * dLineLength ;
                    _v.y += cos ( theta ) * dLineLength ;
                    mesh.addVertex( _v );
                    mesh.addColor( col );
                    
            
                }
                
                
                
				
			}
		}
	}
	glPointSize(3);
   
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofEnableBlendMode(OF_BLENDMODE_ADD ) ; 
    ofScale(1, -1, -1);
        ofTranslate( 0.0f , 0.0f , pointCloudOffset.z ) ;
        mesh.draw( );
    ofPopMatrix();
    //}
     

}
















