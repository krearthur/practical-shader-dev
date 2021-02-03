#pragma once

#include "ofMain.h"

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

        glm::mat4 buildMatrix(glm::vec3 trans, float rot, glm::vec3 scale);

    private:
        void buildQuad(ofMesh& mesh, float w, float h, glm::vec3 pos);
        void updateSprite();

        ofMesh charMesh;
        ofMesh backgroundMesh;
        ofMesh cloudMesh;
        ofMesh sunMesh;
        
        ofShader alphaTestShader;
        ofShader cloudShader;
        ofShader spriteShader;

        ofImage alienImg;
        ofImage backgroundImg;
        ofImage cloudImg;
        ofImage sunImg;

        // character state / inputs
        bool walkRight;
        glm::vec3 charPos;
		
};
