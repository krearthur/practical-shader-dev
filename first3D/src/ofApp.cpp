#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    ofEnableDepthTest();
    
    houseMesh.load("KameHouse.ply");
    uvShader.load("mesh.vert", "uv_vis.frag");

    // move cam a bit out of screen
    cam.position = glm::vec3(0, 0, 1);

}

//--------------------------------------------------------------
void ofApp::update(){
    float speed = 0.4 * ofGetLastFrameTime();
    //charPos += vec3(inputDir.x * speed, inputDir.y * speed, 0);

    cam.position += glm::vec3(camInputDir.x * speed, camInputDir.y * speed, 0);

}

//--------------------------------------------------------------
void ofApp::draw(){
    using namespace glm;

    cam.fov = radians(90.0f);
    vec2 window = ofGetWindowSize();
    float aspect = window.x / window.y;

    mat4 model = rotate(radians(45.0f), vec3(0, 1, 0)) * scale(vec3(0.5, 0.5f, 0.5f));
    mat4 view = inverse(translate(cam.position));
    mat4 proj = perspective(cam.fov, aspect, 0.01f, 10.0f);

    mat4 mvp = proj * view * model;
    
    uvShader.begin();
    uvShader.setUniformMatrix4f("mvp", mvp);
    houseMesh.draw();
    uvShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    camInputDir = glm::vec2(0.0, 0.0);
    if (key == 'a') {
        camInputDir.x = -1.0;
    }
    else if (key == 'd') {
        camInputDir.x = 1.0;
    }
    else if (key == 'w') {
        camInputDir.y = 1.0;
    }
    else if (key == 's') {
        camInputDir.y = -1.0;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'a' || key == 'd' || key == 'w' || key == 's') {
        camInputDir = glm::vec2(0.0, 0.0);
    }
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
