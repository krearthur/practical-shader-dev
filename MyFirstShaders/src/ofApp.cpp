#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Define the quad mesh vertices
    quad.addVertex(glm::vec3(-1, -1, 0));
    quad.addVertex(glm::vec3(-1, 1, 0));
    quad.addVertex(glm::vec3(1, 1, 0));
    quad.addVertex(glm::vec3(1, -1, 0));
    
    // Add some colors per vertex as vertex attributes
    quad.addColor(ofDefaultColorType(1, 0, 0, 1)); // red
    quad.addColor(ofDefaultColorType(0, 1, 0, 1)); // green
    quad.addColor(ofDefaultColorType(0, 0, 1, 1)); // blue
    quad.addColor(ofDefaultColorType(1, 1, 1, 1)); // white

    // Add uv coordinates to vertices
    quad.addTexCoord(glm::vec2(0, 0));
    quad.addTexCoord(glm::vec2(0, 1));
    quad.addTexCoord(glm::vec2(1, 1));
    quad.addTexCoord(glm::vec2(1, 0));

    // Define index buffer to define order of triangles/faces
    ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
    quad.addIndices(indices, 6);

    // Disable pixel coordinates, and load texture
    ofDisableArbTex();
    img1.load("parrot.jpg");
    img2.load("checkerboard_pattern.png");

    img1.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    img2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

    // Load vertex and fragment shader 
    shader.load("scrolling_uv.vert", "mix.frag");

    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    shader.begin();
    shader.setUniformTexture("parrotTex", img1, 0);
    shader.setUniformTexture("checkerBoardTex", img2, 1);

    //shader.setUniform1f("time", ofGetElapsedTimef());


    quad.draw();
    shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    glm::vec3 curPos = quad.getVertex(2);
    quad.setVertex(2, curPos + glm::vec3(-0.1f, -0.1f, 0));
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
