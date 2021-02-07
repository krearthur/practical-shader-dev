#include "ofApp.h"
using namespace glm;

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();

    // positions in normalized screen coordinates (0, 0 => center of screen)
    // these positions are effectively the ORIGINS of the meshes
    // so we set them to zero and use vertex shaders for actual positioning!
    buildQuad(backgroundMesh, 1, 1, vec3(0.0, 0.0, -9.99f));
    buildQuad(sunMesh, 1.0, 1.0, vec3(0.0, 0.0, -0.4));
    buildQuad(cloudMesh, 0.35, 0.3, vec3(0.0, 0.0, 0.0));
    buildQuad(charMesh, 0.15, 0.2, vec3(0.0, -0.60, 0.0));
    
    alienImg.load("alien_spritesheet.png");
    alienImg.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    backgroundImg.load("trees-cliffs.png");
    cloudImg.load("cloud.png");
    sunImg.load("sun_rays.png");

    opaqueShader.load("passthrough.vert", "alphaTest.frag");
    cloudShader.load("passthrough.vert", "cloud.frag");
    spriteShader.load("spritesheet.vert", "alphaTest.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float speed = 0.4 * ofGetLastFrameTime();
    charPos += vec3(inputDir.x * speed, inputDir.y * speed, 0);

    cam.position += vec3(camInputDir.x * speed, camInputDir.y * speed, 0);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDisableBlendMode();
    ofEnableDepthTest();

    mat4 view = buildViewMatrix(cam);
    vec2 window = ofGetWindowSize();

    float aspectRatio = window.x / window.y;
    
    mat4 proj = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.0f, 10.0f);

    // draw character
    spriteShader.begin();
    updateSprite();
    mat4 charMatrix = buildMatrix(charPos, 0.0, vec3(1, 1, 1));
    spriteShader.setUniformMatrix4f("model", charMatrix);
    spriteShader.setUniformMatrix4f("view", view);
    spriteShader.setUniformMatrix4f("proj", proj);
    spriteShader.setUniformTexture("tex", alienImg, 0);
    charMesh.draw();
    spriteShader.end();

    // draw background
    opaqueShader.begin();
    opaqueShader.setUniformMatrix4f("model", mat4());
    opaqueShader.setUniformMatrix4f("view", view);
    opaqueShader.setUniformMatrix4f("proj", proj);
    opaqueShader.setUniformTexture("tex", backgroundImg, 0);
    backgroundMesh.draw();

    // disabling depth testing means we dont draw depth information to the depth buffer anymore
    ofDisableDepthTest();
    opaqueShader.end();

    // draw cloud
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

    // rotate cloud over time
    static float rotation = 0.0f;
    rotation += 1.0f * ofGetLastFrameTime();

    // Construct the transform for our un-rotated cloud
    mat4 translationA = glm::translate(vec3(-0.55, 0.0, 0.0));
    mat4 scaleA = glm::scale(vec3(1.5, 1, 1));
    mat4 transformA = translationA * scaleA;

    // Apply a rotation to that
    mat4 ourRotation = glm::rotate(rotation, vec3(0.0, 0.0, 1.0));
    mat4 newMatrix = translationA * ourRotation * glm::inverse(translationA);
    mat4 finalMatrixA = newMatrix * transformA;

    mat4 transformB = buildMatrix(vec3(0.4, 0.2, 0.0), 1.0f, vec3(1, 1, 1));
    
    cloudShader.begin();
    cloudShader.setUniformMatrix4f("view", view);
    cloudShader.setUniformMatrix4f("proj", proj);
    cloudShader.setUniformTexture("tex", cloudImg, 0);

    cloudShader.setUniformMatrix4f("model", finalMatrixA);
    cloudMesh.draw();

    cloudShader.setUniformMatrix4f("model", transformB);
    cloudMesh.draw();
    
    cloudShader.end();

    // draw the sun rays with additive blending
    opaqueShader.begin();
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
    opaqueShader.setUniformMatrix4f("model", mat4());
    opaqueShader.setUniformMatrix4f("view", view);
    opaqueShader.setUniformMatrix4f("proj", proj);
    opaqueShader.setUniformTexture("tex", sunImg, 0);
    sunMesh.draw();
    opaqueShader.end();

  
}

void ofApp::updateSprite() {
    static float frame = 0.0;
    frame = (frame > 3) ? 0.0 : frame += 0.2;
    spriteShader.setUniform2f("size", vec2(0.333f, 1));
    spriteShader.setUniform2f("offset", vec2((int)frame % 3, 0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    inputDir = vec2(0.0, 0.0);
    if (key == ofKey::OF_KEY_RIGHT) {
        inputDir.x = 1.0;
    }
    else if (key == ofKey::OF_KEY_LEFT) {
        inputDir.x = -1.0;
    }

    camInputDir = vec2(0.0, 0.0);
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
void ofApp::keyReleased(int key) {
    if (key == ofKey::OF_KEY_RIGHT || key == ofKey::OF_KEY_LEFT) {
        inputDir = vec2(0.0, 0.0);
    }

    if (key == 'a' || key == 'd' || key == 'w' || key == 's') {
        camInputDir = vec2(0.0, 0.0);
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

mat4 ofApp::buildViewMatrix(CameraData& cam) {
    return inverse(buildMatrix(cam.position, cam.rotation, vec3(1, 1, 1)));
}