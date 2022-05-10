#include "../headers/LightObject.hpp"

LightObject::LightObject(GLuint shader)
{
    this->shader = shader;
    this->position = vec3(0.0f);
    this->ambient = vec3(1.) ; // * glm::vec3(0.5f)decrease the influence
    this->diffuse = vec3(1.) ; //* glm::vec3(0.2f) decrease the influence
    this->specular = vec3(1.);
};

void LightObject::draw(int i)
{
    glUseProgram(this->shader);
    string light = "lights" + to_string(i);
    GLint lightPositionID = glGetUniformLocation(this->shader, ("lights[" +  to_string(i) + "].position").c_str());
    
    glUniform3fv(lightPositionID, 1, glm::value_ptr(this->position));
    GLint lightMaterialAmbientID = glGetUniformLocation(this->shader, ("lights[" +  to_string(i) + "].material.ambient").c_str());
    glUniform3fv(lightMaterialAmbientID, 1, glm::value_ptr(this->ambient));
    GLint lightMaterialDiffuseID = glGetUniformLocation(this->shader,  ("lights[" +  to_string(i) + "].material.diffuse").c_str());
    glUniform3fv(lightMaterialDiffuseID, 1, glm::value_ptr(this->diffuse));
    GLint lightMaterialSpecularID = glGetUniformLocation(this->shader, ("lights[" +  to_string(i) + "].material.specular").c_str());
    glUniform3fv(lightMaterialSpecularID, 1, glm::value_ptr(this->specular));
}