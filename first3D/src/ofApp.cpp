#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    using namespace glm;
    ofSetBackgroundColor(ofColor(255 * 0.1569f, 255 * 0.4549f, 255 * 0.698f));
    ofDisableArbTex();
    ofEnableDepthTest();
    ofEnableAlphaBlending();

    actorMesh.load("models/hylian-shield2.ply");
    //waterMesh.load("models/quad.ply");
    buildQuad(waterMesh, 1, 1);

    actorShader.load("shaders/mesh.vert", "shaders/multiLight.frag");
    actorTexture.load("textures/shield-diffuse.png");
    actorSpecMap.load("textures/shield-spec.png");
    actorNormalMap.load("textures/shield-normal2.png");

    waterShader.load("shaders/water.vert", "shaders/waterMultiLight.frag");
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

    // -------- Setup lights ----------

    for (int i = 0; i < std::size(dirLights); ++i) {
        DirectionalLight light;
        light.direction = normalize(vec3(0.5, -1, 1));
        light.color = vec3(1, 1, 1) * 0.5f ;
        light.intensity = 1.0f;

        dirLights[i] = light;
    }

    for (int i = 0; i < std::size(pointLights); ++i) {
        PointLight light; 
        light.color = vec3(1, 1, 0);
        light.radius = 1.0f;
        light.position = vec3((1 * i - 0.5) * 2, 0.5, 0.25);
        light.intensity = 3.0;

        pointLights[i] = light;
    }

    for (int i = 0; i < std::size(spotLights); ++i) {
        SpotLight light;
        light.color = vec3((float)i / std::size(spotLights) + 0.5f, 1 * i, 1) ;
        light.position = cam.position + vec3((1 * i - 0.5) , 0, 1);
        light.intensity = 1.0;
        light.direction = vec3(0, 0, -1);
        light.cutoff = glm::cos(glm::radians(15.0));
        light.hardness = 0.7;

        spotLights[i] = light;
    }

}

//--------------------------------------------------------------
void ofApp::update() {
    float speed = 0.4 * ofGetLastFrameTime();

    cam.position += camInputDir * speed;
    cam.rotation += mouseDeltaX * ofGetLastFrameTime() * 10;
}

//--------------------------------------------------------------
void ofApp::draw() {
    using namespace glm;

    vec2 window = ofGetWindowSize();
    float aspect = window.x / window.y;

    mat4 view = inverse(translate(cam.position) * rotate(radians(cam.rotation), vec3(0, 1, 0)));
    mat4 proj = perspective(cam.fov, aspect, 0.01f, 100.0f);

    drawSkybox(proj, view);
    drawShield(proj, view);
    drawWater(proj, view);
}

void ofApp::drawShield(glm::mat4& proj, glm::mat4& view) {
    using namespace glm;

    static float rotAngle = 0.0f;
    //rotAngle += ofGetLastFrameTime() * 25;
    vec3 up = vec3(0, 1, 0);

    mat4 model = translate(vec3(0, 0.5, -0.5)) * rotate(radians(rotAngle), up) * scale(vec3(1, 1, 1)) * 1;
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
    actorShader.setUniform3f("ambientCol", vec3(1, 1, 1) * 0.0);

    putLightsInShader(actorShader);

    actorMesh.draw();
    actorShader.end();

}

void ofApp::drawWater(glm::mat4& proj, glm::mat4& view) {
    using namespace glm;

    static float time = 0.0f;
    time += ofGetLastFrameTime();

    vec3 right = vec3(1, 0, 0);
    mat4 rotation = rotate(radians(-90.0f), right);
    mat4 model = rotation * scale(vec3(10.0f, 10.0f, 1.0f));
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
    waterShader.setUniform3f("ambientCol", vec3(1, 1, 1) * 0.0f);
    waterShader.setUniform3f("cameraPos", cam.position);
    waterShader.setUniform1f("alpha", 1.0f);

    putLightsInShader(waterShader);

    waterMesh.draw();
    waterShader.end();

}

void ofApp::putLightsInShader(ofShader& shader) {
    for (int i = 0; i < std::size(dirLights); ++i) {
        DirectionalLight light = dirLights[i];
        shader.setUniform3f("directionalLights[" + ofToString(i) + "].direction", getLightDirection(light));
        shader.setUniform3f("directionalLights[" + ofToString(i) + "].color", getLightColor(light));
    }
    for (int i = 0; i < std::size(pointLights); ++i) {
        PointLight light = pointLights[i];
        shader.setUniform3f("pointLights[" + ofToString(i) + "].position", light.position);
        shader.setUniform3f("pointLights[" + ofToString(i) + "].color", light.color * light.intensity);
        shader.setUniform1f("pointLights[" + ofToString(i) + "].radius", light.radius);
    }
    for (int i = 0; i < std::size(spotLights); ++i) {
        SpotLight light = spotLights[i];
        shader.setUniform3f("spotLights[" + ofToString(i) + "].position", light.position);
        shader.setUniform3f("spotLights[" + ofToString(i) + "].direction", light.direction);
        shader.setUniform3f("spotLights[" + ofToString(i) + "].color", light.color * light.intensity);
        shader.setUniform1f("spotLights[" + ofToString(i) + "].cutoff", light.cutoff);
        shader.setUniform1f("spotLights[" + ofToString(i) + "].hardness", light.hardness);
    }
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

void ofApp::drawSkybox(glm::mat4& proj, glm::mat4& view) {
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
void ofApp::keyPressed(int key) {
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
void ofApp::keyReleased(int key) {
    if (key == 'a' || key == 'd' || key == 'w' || key == 's' || key == 'q' || key == 'e') {
        camInputDir = glm::vec3(0.0, 0.0, 0.0);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
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
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
    mouseDeltaX = 0;
    mouseDeltaY = 0;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

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
            -w / 2,    h / 2,   0,   // 0 = left top
            -w / 2,   -h / 2,   0,   // 1 = left bottom
             w / 2,   -h / 2,   0,   // 2 = right bottom
             w / 2,    h / 2,   0    // 3 = right top
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