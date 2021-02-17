#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    using namespace glm;
    ofSetBackgroundColor(ofColor(255 * 0.1569f, 255 * 0.4549f, 255 * 0.698f));
    ofDisableArbTex();
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    

    actorMesh.load("models/hylian-shield2.ply");
    //waterMesh.load("models/quad.ply");
    buildQuad(waterMesh, 1, 1);
    
    actorShader.load("shaders/mesh.vert", "shaders/spotBlinnPhong.frag");
    actorTexture.load("textures/shield-diffuse.png");
    actorSpecMap.load("textures/shield-spec.png");
    actorNormalMap.load("textures/shield-normal2.png");

    waterShader.load("shaders/water.vert", "shaders/waterSpotBlinnPhong.frag");
    waterNormalMap.load("textures/water_normal.png");
    waterNormalMap.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

    // Calculate tangents for mesh
    calcTangents(actorMesh);
    calcTangents(waterMesh);

    // Setup Camera: move cam a bit out of screen
    cam.position = glm::vec3(0, 0.5f, .75f);
    cam.fov = radians(90.0f);

    // Setup skybox cubemap
    background.load("models/cube.ply");
    //bgShader.load("shaders/cubemap.vert", "shaders/cubemap.frag");
    skyboxShader.load("shaders/skybox.vert", "shaders/cubemap.frag");
    skybox.load(
        "textures/sky30/frontsh.jpg",
        "textures/sky30/backsh.jpg",
        "textures/sky30/leftsh.jpg",
        "textures/sky30/rightsh.jpg",
        "textures/sky30/toptsh.jpg",
        "textures/sky30/botsh.jpg"
    );

    
}

//--------------------------------------------------------------
void ofApp::update(){
    float speed = 0.4 * ofGetLastFrameTime();
    //charPos += vec3(inputDir.x * speed, inputDir.y * speed, 0);

    cam.position += camInputDir * speed;
    cam.rotation += mouseDeltaX * ofGetLastFrameTime() * 10;
}

//--------------------------------------------------------------
void ofApp::draw(){
    using namespace glm;

    // Setup shield model light
    DirectionalLight dirLight;
    dirLight.direction = normalize(vec3(0.5f, -1, -1));
    dirLight.color = vec3(1, 1, 1);
    dirLight.intensity = 1.0f;

    // Setup point light
    static float t = 0.0f;
    t += ofGetLastFrameTime();
    PointLight pointLight;
    pointLight.color = vec3(1, 1, 1);
    pointLight.radius = 1.0f;
    pointLight.position = vec3(sin(t), 0.5, 0.25);
    pointLight.intensity = 3.0;

    // Setup spot light
    SpotLight spotLight;
    spotLight.color = vec3(1, 1, 1);
    spotLight.position = cam.position;
    spotLight.intensity = 1.0;
    spotLight.direction = vec3(0, 0, -1); 
    spotLight.cutoff = glm::cos(glm::radians(20.0));

    // Setup water light (a bit like sun in the horizon)
    DirectionalLight waterLight;
    waterLight.direction = normalize(vec3(0.5, -1, 1));
    waterLight.color = vec3(1, 1, 1);
    waterLight.intensity = 1.0f;

    vec2 window = ofGetWindowSize();
    float aspect = window.x / window.y;

    mat4 view = inverse(translate(cam.position) * rotate(radians(cam.rotation), vec3(0, 1, 0)));
    mat4 proj = perspective(cam.fov, aspect, 0.01f, 100.0f);

    drawShield(spotLight, proj, view);
    drawWater(spotLight, proj, view);
    //drawCube(dirLight, proj, view);
    drawSkybox(spotLight, proj, view);

}

void ofApp::drawShield(SpotLight& light, glm::mat4& proj, glm::mat4& view) {
    using namespace glm;

    static float rotAngle = 0.0f;
    //rotAngle += 75 * ofGetLastFrameTime();
    vec3 up = vec3(0, 1, 0);

    mat4 model = rotate(radians(rotAngle), up) * scale(vec3(0.5, 0.5f, 0.5f)) * translate(vec3(0, 1, -0.5));
    mat3 normalMatrix = (transpose(inverse(mat3(model))));
    mat4 mvp = proj * view * model;

    actorShader.begin();
    // Uniforms for vertex shader
    actorShader.setUniformMatrix4f("mvp", mvp);
    actorShader.setUniformMatrix4f("model", model);
    actorShader.setUniformMatrix3f("normalMatrix", normalMatrix);
    // Uniforms for fragment shader 
    actorShader.setUniformTexture("mainTex", actorTexture, 0);
    actorShader.setUniformTexture("specMap", actorSpecMap, 1);
    actorShader.setUniformTexture("normalMap", actorNormalMap, 2);
    actorShader.setUniformTexture("envMap", skybox.getTexture(), 3);
    actorShader.setUniform3f("cameraPos", cam.position);
    actorShader.setUniform3f("meshCol", glm::vec3(1, 1, 1));
    //actorShader.setUniform3f("lightDir", getLightDirection(dirLight));
    actorShader.setUniform3f("lightConeDir", light.direction);
    actorShader.setUniform3f("lightCol", light.color * light.intensity);
    actorShader.setUniform3f("lightPos", light.position);
    actorShader.setUniform1f("lightCutoff", light.cutoff);
    actorShader.setUniform3f("ambientCol", vec3(1, 1, 1) * 0.0);

    actorMesh.draw();
    actorShader.end();

}

void ofApp::drawWater(SpotLight& light, glm::mat4& proj, glm::mat4& view) {
    using namespace glm;

    static float time = 0.0f;
    time += ofGetLastFrameTime();

    vec3 right = vec3(1, 0, 0);
    mat4 rotation = rotate(radians(-90.0f), right);
    mat4 model = rotation * scale(vec3(10.0f, 5.0f, 1.0f));
    mat4 mvp = proj * view * model;
    mat3 normalMatrix = mat3(transpose(inverse(model)));

    waterShader.begin();
    // Uniforms for vertex shader
    waterShader.setUniformMatrix4f("mvp", mvp);
    waterShader.setUniformMatrix4f("model", model);
    waterShader.setUniformMatrix3f("normalMatrix", normalMatrix);
    waterShader.setUniform1f("time", time);
    // Uniforms for fragment shader
    waterShader.setUniformTexture("normalMap", waterNormalMap, 0);
    waterShader.setUniformTexture("envMap", skybox.getTexture(), 1);
    //waterShader.setUniform3f("lightDir", getLightDirection(dirLight));
    //waterShader.setUniform3f("lightCol", getLightColor(dirLight));
    waterShader.setUniform3f("lightConeDir", light.direction);
    waterShader.setUniform3f("lightCol", light.color * light.intensity);
    waterShader.setUniform3f("lightPos", light.position);
    waterShader.setUniform1f("lightCutoff", light.cutoff);
    waterShader.setUniform3f("ambientCol", vec3(1, 1, 1) * 0.0f);
    waterShader.setUniform3f("cameraPos", cam.position);
    waterShader.setUniform1f("alpha", 1.0f);

    waterMesh.draw();
    waterShader.end();

}

//void ofApp::drawCube(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view) {
//    using namespace glm;
//
//    static float rotAngle = 0.0;
//    rotAngle += 0.1f;
//
//    mat4 r = rotate(radians(rotAngle), vec3(0, 1, 0));
//    mat4 s = scale(vec3(0.4, 0.4, 0.4));
//    mat4 model = translate(vec3(0.0, 0.75, 0.0f)) * r * s;
//    mat4 mvp = proj * view * model;
//
//    ofShader& shd = bgShader;
//
//    shd.begin();
//    shd.setUniformMatrix4f("mvp", mvp);
//    shd.setUniformTexture("envMap", skybox.getTexture(), 0);
//    shd.setUniform3f("cameraPos", cam.position);
//    background.draw();
//    shd.end();
//}

void ofApp::drawSkybox(SpotLight& dirLight, glm::mat4& proj, glm::mat4& view) {
    using namespace glm;

    // move the box center always to the cam position
    mat4 model = translate(cam.position);
    mat4 mvp = proj * view * model;

    ofShader& shd = skyboxShader;

    glDepthFunc(GL_LEQUAL);
    shd.begin();
    shd.setUniformMatrix4f("mvp", mvp);
    shd.setUniformTexture("envMap", skybox.getTexture(), 0);
    background.draw();
    shd.end();
    glDepthFunc(GL_LESS);
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
//    ofLog(OF_LOG_NOTICE, "mouse x:" + ofToString(x) + ", y:" + ofToString(y) );

    if (mousePrevX == -1 && mousePrevY == -1) {
        mousePrevX = x;
        mousePrevY = y;
        return;
    }
    return;
    mouseDeltaX = mousePrevX - x;
    mouseDeltaY = y - mousePrevY;

    mousePrevX = x;
    mousePrevY = y;
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
    mouseDeltaX = 0;
    mouseDeltaY = 0;
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

void ofApp::buildQuad(ofMesh& mesh, float w, float h)
{   
    mesh.clear();
    // Define the four vertices for the quad
    float verts[] = {
    //   X       Y      Z
        -w/2,    h/2,   0,   // 0 = left top
        -w/2,   -h/2,   0,   // 1 = left bottom
         w/2,   -h/2,   0,   // 2 = right bottom
         w/2,    h/2,   0    // 3 = right top
    };

    float uvs[] = { 0,0, 0,1, 1,1, 1,0 }; // same order as verts definition

    for (int i = 0; i < 4; ++i) {
        int idx = i * 3;
        int uvIdx = i * 2;

        mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
        mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
        mesh.addNormal(glm::vec3(0, 0, 1));

    }
    
    ofIndexType indices[6] = { 0, 1, 2, 2, 3, 0 };
//    mesh.addIndices(indices, 6);
    mesh.addTriangle(0, 1, 2);
    mesh.addTriangle(2, 3, 0);

}

void ofApp::calcTangents(ofMesh& mesh) {
    using namespace glm;

    std::vector<vec4> tangents;
    tangents.resize(mesh.getNumVertices());
    uint indexCount = mesh.getNumIndices();


    const vec3* vertices = mesh.getVerticesPointer();
    const vec2* uvs = mesh.getTexCoordsPointer();
    const uint* indices = mesh.getIndexPointer();

    for (uint i = 0; i < indexCount - 2; i += 3) {
        const vec3& v0 = vertices[indices[i]];
        const vec3& v1 = vertices[indices[i + 1]];
        const vec3& v2 = vertices[indices[i + 2]];
        const vec2& uv0 = uvs[indices[i]];
        const vec2& uv1 = uvs[indices[i + 1]];
        const vec2& uv2 = uvs[indices[i + 2]];

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;
        vec2 dUV1 = uv1 - uv0;
        vec2 dUV2 = uv2 - uv0;

        float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

        vec4 tan;
        tan.x = f * (dUV2.y * edge1.x - dUV1.y * edge2.x);
        tan.y = f * (dUV2.y * edge1.y - dUV1.y * edge2.y);
        tan.z = f * (dUV2.y * edge1.z - dUV1.y * edge2.z);
        tan.w = 0;
        tan = normalize(tan);

        tangents[indices[i]] += tan;
        tangents[indices[i + 1]] += tan;
        tangents[indices[i + 2]] += tan;
    }

    mesh.setColorForIndices(0, indexCount, ofColor::cyan);

    for (uint i = 0; i < tangents.size(); ++i) {
        vec3 t = normalize(tangents[i]);
        mesh.setColor(i, ofFloatColor(t.x, t.y, t.z, 0.0));
    }
}