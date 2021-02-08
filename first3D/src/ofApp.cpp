#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    using namespace glm;
    ofSetBackgroundColor(ofColor::bisque);
    ofDisableArbTex();
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    
    
    myMesh.load("Hero.ply");
    diffuseShader.load("mesh.vert", "specular.frag");
    texture.load("heroTex.png");
    specular.load("heroSpec.png");

    // move cam a bit out of screen
    cam.position = glm::vec3(0, 0.35f, 0.75f);
    
    // setup light
    light.direction = normalize(vec3(-1, -1, 0)); // pointin down
    light.color = vec3(1, 1, 1);
    light.intensity = 1.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    float speed = 0.4 * ofGetLastFrameTime();
    //charPos += vec3(inputDir.x * speed, inputDir.y * speed, 0);

    cam.position += camInputDir * speed;

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
    mat3 normalMatrix = (transpose(inverse(mat3(model))));

    mat4 mvp = proj * view * model;
    
    diffuseShader.begin();
    diffuseShader.setUniformTexture("mainTex", texture, 0);
    diffuseShader.setUniformTexture("specMap", specular, 1);
    diffuseShader.setUniformMatrix4f("mvp", mvp);
    diffuseShader.setUniformMatrix4f("model", model);
    diffuseShader.setUniformMatrix3f("normalMatrix", normalMatrix);
    diffuseShader.setUniform3f("cameraPos", cam.position);
    diffuseShader.setUniform3f("meshCol", glm::vec3(1, 1, 1)); 
    diffuseShader.setUniform3f("lightDir", getLightDirection(light));
    diffuseShader.setUniform3f("lightCol", getLightColor(light));
    diffuseShader.setUniform3f("ambientCol", vec3(1, 1, 1) * 0.1);
    myMesh.draw();
    diffuseShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    camInputDir = glm::vec3(0.0, 0.0, 0.0);
    if (key == 'a') {
        camInputDir.x = -1.0;
    }
    else if (key == 'd') {
        camInputDir.x = 1.0;
    }
    else if (key == 'q') {
        camInputDir.y = 1.0;
    }
    else if (key == 'e') {
        camInputDir.y = -1.0;
    }
    else if (key == 'w') {
        camInputDir.z = -1.0;
    }
    else if (key == 's') {
        camInputDir.z = 1.0;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'a' || key == 'd' || key == 'w' || key == 's' || key =='q' || key == 'e') {
        camInputDir = glm::vec3(0.0, 0.0, 0.0);
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

glm::vec3 ofApp::getLightDirection(DirectionalLight& l) {
    return glm::normalize(l.direction * -1.0f);
}

glm::vec3 ofApp::getLightColor(DirectionalLight& l) {
    return l.color * l.intensity;
}