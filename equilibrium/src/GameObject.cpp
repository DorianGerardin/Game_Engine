#include "../headers/GameObject.hpp"

GameObject::GameObject(string id, string filename, GLint modelID, GLuint shader)
{
    this->id = id;
    transform = new Transform();
    this->mesh = new Mesh(filename, modelID);
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;
}

GameObject::GameObject(string id, int meshType, float size, GLint modelID, GLuint shader)
{
    this->id = id;
    this->transform = new Transform();
    this->mesh = new Mesh(meshType, size, modelID);
    this->parent = nullptr;
    this->shader = shader;
    this->useHeightMap = GL_FALSE;
    this->hasTexture = false;
}

GameObject::GameObject()
{
    this->transform = new Transform();
    this->useHeightMap = GL_FALSE;
    this->parent = nullptr;
    this->hasTexture = false;
}

GameObject::~GameObject()
{
    delete this;
}

bool GameObject::SameHalfPlane(vec3 p1, vec3 p2, vec3 a, vec3 b)
{
    vec3 cp1 = cross(b - a, p1 - a);
    vec3 cp2 = cross(b - a, p2 - a);
    if (dot(cp1, cp2) >= 0)
        return true;
    return false;
}

bool GameObject::pointInTriangle(vec3 p, vec3 a, vec3 b, vec3 c)
{
    if (SameHalfPlane(p, a, b, c) && SameHalfPlane(p, b, a, c) && SameHalfPlane(p, c, a, b))
        return true;
    return false;
}

int GameObject::triangleIndexOnPosition(Mesh *mesh)
{
    vector<unsigned short> triangles = mesh->indices;
    vector<vec3> indexed_vertices = mesh->indexed_vertices;

    vec3 pos = this->transform->getLocalTranslation();
    vec3 p = vec3(pos.x, pos.y, 0.0f);
    vec3 p1, p2, p3;

    for (size_t i = 0; i < triangles.size(); i += 3)
    {
        p1 = vec3(indexed_vertices[triangles[i]].x, indexed_vertices[triangles[i]].y, 0.0f);
        p2 = vec3(indexed_vertices[triangles[i + 1]].x, indexed_vertices[triangles[i + 1]].y, 0.0f);
        p3 = vec3(indexed_vertices[triangles[i + 2]].x, indexed_vertices[triangles[i + 2]].y, 0.0f);
        if (pointInTriangle(pos, p1, p2, p3))
        {
            return i / 3;
        }
    }
    return -1;
}

// https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Edge_approach
float GameObject::heightInTriangle(Mesh *mesh)
{
    vector<unsigned short> triangles = mesh->indices;
    vector<vec3> indexed_vertices = mesh->indexed_vertices;
    vec3 p = this->transform->getLocalTranslation();
    int i = triangleIndexOnPosition(mesh);
    if (i == -1)
        return 0.0f;
    vec3 p1 = indexed_vertices[triangles[3 * i]];
    vec3 p2 = indexed_vertices[triangles[3 * i + 1]];
    vec3 p3 = indexed_vertices[triangles[3 * i + 2]];

    // cout << p1 << "\t" << p2 << "\t" << p3 << endl;
    float lambda1 = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
    float lambda2 = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
    float lambda3 = 1.0f - lambda1 - lambda2;
    return (lambda1 * p1.z + lambda2 * p2.z + lambda3 * p3.z);
}

void GameObject::applyTexture(GLuint texture, GLuint textureID)
{

    this->hasTexture = true;
    this->mesh->texture = texture;
    this->mesh->textureID = textureID;
}

void GameObject::draw()
{
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;

    glUseProgram(this->shader);

    GLint useHeightMapID = glGetUniformLocation(this->shader, "useHeightMap");
    if (this->useHeightMap)
        glUniform1i(useHeightMapID, GL_TRUE);
    else
        glUniform1i(useHeightMapID, GL_FALSE);

    glUniformMatrix4fv(this->mesh->modelID, 1, GL_FALSE, &this->transform->getModelMatrix()[0][0]);

    // BUFFERS
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->mesh->indexed_vertices.size() * sizeof(vec3), &this->mesh->indexed_vertices[0], GL_STATIC_DRAW);

    // normals
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->mesh->normals.size() * sizeof(vec3), &this->mesh->normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(unsigned short), &this->mesh->indices[0], GL_STATIC_DRAW);

    // TEXTURES
    GLint hasTextureID = glGetUniformLocation(this->shader, "hasTexture");
    if (hasTexture)
    {

        glUniform1i(hasTextureID, GL_TRUE);

        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, this->mesh->uv.size() * sizeof(float), &this->mesh->uv[0], GL_STATIC_DRAW);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->mesh->texture);
        glUniform1i(this->mesh->textureID, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }
    else
        glUniform1i(hasTextureID, GL_FALSE);

    // MATERIAL
    GLint materialAmbientID = glGetUniformLocation(this->shader, "material.ambient");
    glUniform3fv(materialAmbientID, 1, glm::value_ptr(this->mesh->material.ambient));
    GLint materialDiffuseID = glGetUniformLocation(this->shader, "material.diffuse");
    glUniform3fv(materialDiffuseID, 1, glm::value_ptr(this->mesh->material.diffuse));
    GLint materialSpecularID = glGetUniformLocation(this->shader, "material.specular");
    glUniform3fv(materialSpecularID, 1, glm::value_ptr(this->mesh->material.specular));
    GLint materialShininessID = glGetUniformLocation(this->shader, "material.shininess");
    glUniform1f(materialShininessID, this->mesh->material.shininess);

    // DRAW
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        4,        // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !

    glDrawElements(
        GL_TRIANGLES,               // mode
        this->mesh->indices.size(), // count
        GL_UNSIGNED_SHORT,          // type
        (void *)0                   // element array buffer offset
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    /*
                glDeleteBuffers(1, &vertexbuffer);
                glDeleteBuffers(1, &elementbuffer);*/
}
