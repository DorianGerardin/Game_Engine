#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 4) in vec3 vertexNormal;
//layout (location = 2) in vec2 aTexCoord;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

uniform float uHeight;
uniform float vHeight;

//uniform mat4 transfo_mat;
uniform mat4 modelMatrix;
uniform mat4 selfModelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D texSampler;
uniform sampler2D hmapSampler;
uniform bool useHeightMap;

out vec2 UV;
out vec3 normal;
out vec3 FragPos;

void main(){

        vec4 position = vec4(vertices_position_modelspace,1);
        if (useHeightMap) 
        {
            position.z = 5*texture(hmapSampler, vertexUV).r;
        } 

        gl_Position = projectionMatrix * viewMatrix * modelMatrix/*  * selfModelMatrix */ * position;
        // TODO : Output position of the vertex, in clip space : MVP * position
        UV = vertexUV;
        normal = vertexNormal;
        // FragPos =  vec3(modelMatrix * vec4(vertices_position_modelspace, 1.0));
        FragPos =  vertices_position_modelspace;
}