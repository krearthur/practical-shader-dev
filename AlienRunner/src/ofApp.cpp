#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    ofEnableDepthTest();

    buildQuad(backgroundMesh, 1, 1, glm::vec3(0.0, 0.0, 0.5));
    buildQuad(cloudMesh, 0.35, 0.3, glm::vec3(-0.5, -0.10f, 0.4));
    buildQuad(charMesh, 0.15, 0.2, glm::vec3(0.0, -0.60f, 0.0));
    
    alienImg.load("alien_george.png");
    backgroundImg.load("trees-cliffs.png");
    cloudImg.load("cloud.png");

    shader.load("passthrough.vert", "alphaTest.frag");
    cloudShader.load("passthrough.vert", "cloud.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    using namespace glm;

    shader.begin();

    // draw character
    shader.setUniformTexture("tex", alienImg, 0);
    charMesh.draw();

    // draw background
    shader.setUniformTexture("tex", backgroundImg, 0);
    backgroundMesh.draw();

    shader.end();

    // draw cloud
    cloudShader.begin();
    shader.setUniformTexture("tex", cloudImg, 0);
    cloudMesh.draw();
    cloudShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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


void ofApp::buildQuad(ofMesh & mesh, float w, float h, glm::vec3 pos)
{
    // Define the four vertices for the quad
    float verts[] = {
        -w + pos.x, -h + pos.y, pos.z, // left top
        -w + pos.x, h + pos.y, pos.z, // left bottom
        w + pos.x, h + pos.y, pos.z, // right bottom
        w + pos.x, -h + pos.y, pos.z // right top
    };

    float uvs[] = { 0,0, 0,1, 1,1, 1,0 }; // same order as verts definition

    for (int i = 0; i < 4; ++i) {
        int idx = i * 3;
        int uvIdx = i * 2;

        mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]) );
        mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
    }

    ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
    mesh.addIndices(indices, 6);
}
