#pragma once

#include "ofMain.h"

struct CameraData {
    glm::vec3 position;
    float rotation;
    float fov;
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
        // Custom methods
        void calcTangents(ofMesh& mesh);
        void ofApp::drawShield(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view);
        void ofApp::drawWater(DirectionalLight& dirLight, glm::mat4& proj, glm::mat4& view);
        glm::vec3 getLightDirection(DirectionalLight& l);
        glm::vec3 getLightColor(DirectionalLight& l);
        void ofApp::buildQuad(ofMesh& mesh, float w, float h);

        // Actor properties
        ofMesh actorMesh;
        ofShader actorShader;
        ofImage actorTexture;
        ofImage actorSpecMap;
        ofImage actorNormalMap;

        // Water properties
        ofMesh waterMesh;
        ofShader waterShader;
        ofImage waterNormalMap;

        // Camera properties
        CameraData cam;
        glm::vec3 camInputDir;

};
