#include "../headers/LightObject.hpp"


LightObject::LightObject(GLuint shader){
    this->shader = shader;
    this->position = vec3(-1., 0., 0.);
    this->material.ambient = vec3(1.) * glm::vec3(0.5f); // decrease the influence
    this->material.diffuse = vec3(1.) * glm::vec3(0.2f); // decrease the influence
    this->material.specular = vec3(1.);
};

void LightObject::draw()
{
    glUseProgram(this->shader);
    GLint lightPositionID = glGetUniformLocation(this->shader, "light.position");
    glUniform3fv(lightPositionID, 1,  glm::value_ptr(this->position));
    GLint lightMaterialAmbientID = glGetUniformLocation(this->shader, "light.material.ambient");
    glUniform3fv(lightMaterialAmbientID,1,  glm::value_ptr(this->material.ambient));
    GLint lightMaterialDiffuseID = glGetUniformLocation(this->shader, "light.material.diffuse");
    glUniform3fv(lightMaterialDiffuseID,  1,  glm::value_ptr(this->material.diffuse));
    GLint lightMaterialSpecularID = glGetUniformLocation(this->shader, "light.material.specular");
    glUniform3fv(lightMaterialSpecularID,1,  glm::value_ptr(this->material.specular));



}