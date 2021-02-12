#pragma once
#include "ofMain.h"

class GameObject {

public:
    
    void setup();
    void update();
    void draw();

private:
    ofMesh myMesh;
    ofShader diffuseShader;
    ofImage texture;
    ofImage specular;

};
