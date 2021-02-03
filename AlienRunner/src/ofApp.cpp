#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();

    // positions in normalized screen coordinates (0, 0 => center of screen)
    // these positions are effectively the ORIGINS of the meshes
    // so we set them to zero and use vertex shaders for actual positioning!
    buildQuad(backgroundMesh, 1, 1, vec3(0.0, 0.0, 0.5));
    buildQuad(sunMesh, 1.0, 1.0, vec3(0.0, 0.0, 0.4));
    buildQuad(cloudMesh, 0.35, 0.3, vec3(0.0, 0.0, 0.0));
    buildQuad(charMesh, 0.15, 0.2, vec3(0.0, -0.60, 0.0));
    
    alienImg.load("alien_spritesheet.png");
    alienImg.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    backgroundImg.load("trees-cliffs.png");
    cloudImg.load("cloud.png");
    sunImg.load("sun_rays.png");

    alphaTestShader.load("passthrough.vert", "alphaTest.frag");
    cloudShader.load("cloud.vert", "cloud.frag");
    spriteShader.load("spritesheet.vert", "alphaTest.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (walkRight) {
        float speed = 0.4 * ofGetLastFrameTime();
        charPos += vec3(speed, 0, 0);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDisableBlendMode();
    ofEnableDepthTest();

    // draw character
    spriteShader.begin();

    updateSprite();

    spriteShader.setUniformTexture("tex", alienImg, 0);
    spriteShader.setUniform3f("position", charPos);
    charMesh.draw();
    spriteShader.end();

    // draw background
    alphaTestShader.begin();
    alphaTestShader.setUniformTexture("tex", backgroundImg, 0);
    backgroundMesh.draw();

    // disabling depth testing means we dont draw depth information to the depth buffer anymore
    ofDisableDepthTest();
    alphaTestShader.end();

    // draw cloud
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

    // rotate cloud over time
    static float rotation = 0.0f;
    rotation += 1.0f * ofGetLastFrameTime();
    mat4 transformA = buildMatrix(vec3(-0.55, 0.0, 0.0), rotation, vec3(1.5, 1, 1));
    mat4 transformB = buildMatrix(vec3(0.4, 0.2, 0.0), 1.0f, vec3(1, 1, 1));

    cloudShader.begin();
    cloudShader.setUniformTexture("tex", cloudImg, 0);

    cloudShader.setUniformMatrix4f("transform", transformA);
    cloudMesh.draw();

    cloudShader.setUniformMatrix4f("transform", transformB);
    cloudMesh.draw();
    
    cloudShader.end();

    // draw the sun rays with additive blending
    alphaTestShader.begin();
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
    alphaTestShader.setUniformTexture("tex", sunImg, 0);
    sunMesh.draw();
    alphaTestShader.end();

  
}

void ofApp::updateSprite() {
    static float frame = 0.0;
    frame = (frame > 3) ? 0.0 : frame += 0.2;
    spriteShader.setUniform2f("size", vec2(0.333f, 1));
    spriteShader.setUniform2f("offset", vec2((int)frame % 3, 0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ofKey::OF_KEY_RIGHT) {
        walkRight = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ofKey::OF_KEY_RIGHT) {
        walkRight = false;
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


void ofApp::buildQuad(ofMesh & mesh, float w, float h, vec3 pos)
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

        mesh.addVertex(vec3(verts[idx], verts[idx + 1], verts[idx + 2]) );
        mesh.addTexCoord(vec2(uvs[uvIdx], uvs[uvIdx + 1]));
    }

    ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
    mesh.addIndices(indices, 6);
}

mat4 ofApp::buildMatrix(vec3 trans, float rot, vec3 scale) {
    mat4 translation = glm::translate(trans);
    mat4 rotation = glm::rotate(rot, vec3(0.0, 0.0, 1.0));
    mat4 scaler = glm::scale(scale);

    // by default openFrameworks uses column-major matrices, so the order is post-multiply aka from right to left
    return translation * rotation * scaler;
}
