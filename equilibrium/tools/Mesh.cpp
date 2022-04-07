#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
 
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;



class Mesh {

    public:
        vector<vec3> indexed_vertices;
        vector<unsigned short> indices;
        vector<float> uv;
        float size;
        GLuint texture, textureID;
        GLint modelID;

    public:

        Mesh(string tag, float size, GLint modelID) {
            this->size = size;
            this->generateMesh(tag);
            this->modelID = modelID;
        }

        Mesh(string filename, GLint modelID) {
            loadOFF(filename, this->indexed_vertices, this->indices);
            this->modelID = modelID;
        }

        ~Mesh() {
            delete this;
        }

    private:

        void generateMesh(string meshType) {
            if(meshType.compare("sphere") == 0) this->generateSphere();
            else if(meshType.compare("plane") == 0) this->generatePlane();
            else {
                printf("Ce maillage n'est pas supporté \n");
                exit(1);
            }
        }

        void generatePlane() {

            int size = this->size;
            this->indexed_vertices.clear();
            this->indices.clear();
            this->uv.clear();

            int minZ = -1;
            int maxZ = 1;

            this->size = (int)floor(this->size);
            int length = 10;

            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    vec3 vertex = vec3(( (float)i/(float)(this->size-1)*length)-length/2, ((float)j/(float)(this->size-1)*length)-length/2, 0);
                    this->indexed_vertices.push_back(vertex);
                    this->uv.push_back((float)j/(float)(size-1));
                    this->uv.push_back((float)i/(float)(size-1));
                }
            }

            for (int i = 0; i < size-1; ++i)
            {
                for (int j = 0; j < size-1; ++j)
                {
                    indices.push_back(i*size+j);
                    indices.push_back(i*size+(j+1));
                    indices.push_back((i+1)*size+j);

                    indices.push_back(i*size+(j+1));
                    indices.push_back((i+1)*size+(j+1));
                    indices.push_back((i+1)*size+j);
                }
            }
        }

        void generateSphere() {

            float x, y, z, u, v;
            float theta, phi;

            int size = this->size;

            this->indexed_vertices.clear();
            this->indices.clear();
            this->uv.clear();

            for(unsigned int i = 0; i < size; i++) {

                u = (float)i / (float)(size-1);

                theta = 2 * M_PI * u;

                for(unsigned int j = 0; j < size; j++) {
                    
                    v = (float)j / (float)(size-1);
                    
                    phi = -M_PI / 2.0f + v * M_PI;

                    x = cos(theta) * cos(phi);
                    y = sin(theta) * cos(phi);
                    z = sin(phi);

                    this->indexed_vertices.push_back(vec3(x,y,z));
                    this->uv.push_back(u);
                    this->uv.push_back(v);

                    if(i < size-1 && j < size-1) {
                        this->indices.push_back(i+j*size);
                        this->indices.push_back((i+1)+j*size);
                        this->indices.push_back((i+1)+(j+1)*size);

                        this->indices.push_back(i+j*size);
                        this->indices.push_back((i+1)+(j+1)*size);
                        this->indices.push_back(i+(j+1)*size);
                    }
                }
            }
        }

};






