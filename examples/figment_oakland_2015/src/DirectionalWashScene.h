//
//  SimpleScene.h
//  simpleExample
//
//  Created by Ben McChesney on 5/15/14.
//
//

#pragma once

#include "BaseScene.h"



class DirectionalWashScene : public BaseScene
{
public :
	DirectionalWashScene() {}
	DirectionalWashScene( int a_index, string a_name ):BaseScene( a_index, a_name ) {}

    void setup();
    void setupGui(float a_x=0, float a_y=0);
//    void guiEvent(ofxUIEventArgs &e);
    
    void update();
    void draw();
	void drawDebug(); 
    
    void activate();
    void deactivate();
    
    bool transitionIn( float delay , float transitionTime );
    bool transitionOut( float delay , float transitionTime ) ;

	bool bNorth; 
	bool bEast; 
	bool bSouth; 
	bool bWest; 
	bool bRandom; 
    



    
};