#include "../headers/Mesh.hpp"

string Mesh::getFileExt(const string &s)
{

    size_t i = s.rfind('.', s.length());
    if (i != string::npos)
    {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}

// Import Maillage .off et .obj
Mesh::Mesh(string filename, GLint modelID)
{
    string extension = getFileExt(filename);
    if (!extension.compare("off"))
    {
        loadOFF(filename, this->indexed_vertices, this->indices);
        this->calculate_normals();
    }
    if (!extension.compare("obj"))
        loadOBJ(filename, this->indexed_vertices, this->indices, this->uv, this->vericesNormals);

    this->modelID = modelID;
    this->initializeMaterial();
}

// Générattion Maillage d'une primitive (plan, cube, sphère)
Mesh::Mesh(int type, float size, GLint modelID)
{
    this->size = size;
    this->objectType = type;
    this->modelID = modelID;
    this->generateMesh();
}

Mesh::~Mesh()
{
    delete this;
}

void Mesh::initializeMaterial()
{
    this->material.ambient = vec3(0.6, 0.5, 0.3); // make everything yellowish by default
    this->material.diffuse = vec3(1.);
    this->material.specular = vec3(1.);
    this->material.shininess = 0.5;
}

void Mesh::generateMesh()
{
    if (this->objectType == SPHERE)
        this->generateSphere();
    else if (this->objectType == PLANE)
        this->generatePlane();
    else if (this->objectType == CUBE)
        this->generateCube();
    else
    {
        printf("Ce maillage n'est pas supporté \n");
        exit(1);
    }
    this->calculate_normals();
}

// void Mesh::generatePlane()
// {

//     int size = this->size;
//     this->indexed_vertices.clear();
//     this->indices.clear();
//     this->uv.clear();

//     int minZ = -1;
//     int maxZ = 1;

//     this->size = (int)floor(this->size);
//     int length = 1;

//     for (int i = 0; i < size; ++i)
//     {
//         for (int j = 0; j < size; ++j)
//         {
//             vec3 vertex = vec3(((float)i / (float)(this->size - 1) * length) - length / 2, ((float)j / (float)(this->size - 1) * length) - length / 2, 0);
//             this->indexed_vertices.push_back(vertex);
//             this->uv.push_back((float)j / (float)(size - 1));
//             this->uv.push_back((float)i / (float)(size - 1));
//         }
//     }

//     for (int i = 0; i < size - 1; ++i)
//     {
//         for (int j = 0; j < size - 1; ++j)
//         {
//             indices.push_back(i * size + j);
//             indices.push_back(i * size + (j + 1));
//             indices.push_back((i + 1) * size + j);

//             indices.push_back(i * size + (j + 1));
//             indices.push_back((i + 1) * size + (j + 1));
//             indices.push_back((i + 1) * size + j);
//         }
//     }
//     this->calculate_normals();
// }
void Mesh::generatePlane()
{

    this->indexed_vertices.clear();
    this->indices.clear();
    this->uv.clear();
    float length = this->size;

    this->indexed_vertices.push_back(vec3(0.0f, 0.0f, 0.0f));
    this->indexed_vertices.push_back(vec3(length, 0.0f, 0.0f));
    this->indexed_vertices.push_back(vec3(0.0f, length, 0.0f));
    this->indexed_vertices.push_back(vec3(length, length, 0.0f));

    // --------------------

    this->uv.push_back(0.0f);
    this->uv.push_back(0.0f);

    this->uv.push_back(1.0f);
    this->uv.push_back(0.0f);

    this->uv.push_back(0.0f);
    this->uv.push_back(1.0f);

    this->uv.push_back(1.0f);
    this->uv.push_back(1.0f);

    // --------------------

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(3);

    this->indices.push_back(0);
    this->indices.push_back(3);
    this->indices.push_back(2);

    this->calculate_normals();
}

void Mesh::generateSphere()
{

    float x, y, z, u, v;
    float theta, phi;

    // int size = this->size;
    int size = 50;

    this->indexed_vertices.clear();
    this->indices.clear();
    this->uv.clear();

    for (unsigned int i = 0; i < size; i++)
    {

        u = (float)i / (float)(size - 1);

        theta = 2 * M_PI * u;

        for (unsigned int j = 0; j < size; j++)
        {

            v = (float)j / (float)(size - 1);

            phi = -M_PI / 2.0f + v * M_PI;

            x = this->size * cos(theta) * cos(phi);
            y = this->size * sin(theta) * cos(phi);
            z = this->size * sin(phi);

            this->indexed_vertices.push_back(vec3(x, y, z));
            this->uv.push_back(u);
            this->uv.push_back(v);

            if (i < size - 1 && j < size - 1)
            {
                this->indices.push_back(i + j * size);
                this->indices.push_back((i + 1) + j * size);
                this->indices.push_back((i + 1) + (j + 1) * size);

                this->indices.push_back(i + j * size);
                this->indices.push_back((i + 1) + (j + 1) * size);
                this->indices.push_back(i + (j + 1) * size);
            }
        }
    }
    this->calculate_normals();
}

void Mesh::generateCube()
{
    this->indexed_vertices.clear();
    this->indices.clear();
    this->uv.clear();
    // float length = this->size;
    float length = this->size / 2.0f;

    // cf. http://ilkinulas.github.io/development/unity/2016/05/06/uv-mapping.html

    this->indexed_vertices.push_back(vec3(-length, length, -length));
    this->indexed_vertices.push_back(vec3(-length, -length, -length));
    this->indexed_vertices.push_back(vec3(length, length, -length));
    this->indexed_vertices.push_back(vec3(length, -length, -length));

    this->indexed_vertices.push_back(vec3(-length, -length, length));
    this->indexed_vertices.push_back(vec3(length, -length, length));
    this->indexed_vertices.push_back(vec3(-length, length, length));
    this->indexed_vertices.push_back(vec3(length, length, length));

    this->indexed_vertices.push_back(vec3(-length, length, -length));
    this->indexed_vertices.push_back(vec3(length, length, -length));

    this->indexed_vertices.push_back(vec3(-length, length, -length));
    this->indexed_vertices.push_back(vec3(-length, length, length));

    this->indexed_vertices.push_back(vec3(length, length, -length));
    this->indexed_vertices.push_back(vec3(length, length, length));

    // --------------------
    // 4:3 ratio
    // 0, 0.66f
    this->uv.push_back(0.0f);
    this->uv.push_back(0.66f);
    // 0.25f, 0.66f
    this->uv.push_back(0.25f);
    this->uv.push_back(0.66f);
    // 0, 0.33f
    this->uv.push_back(0.0f);
    this->uv.push_back(0.33f);
    // 0.25f, 0.33f
    this->uv.push_back(0.25f);
    this->uv.push_back(0.33f);

    // 0.5f, 0.66f
    this->uv.push_back(0.5f);
    this->uv.push_back(0.66f);
    // 0.5f, 0.33f
    this->uv.push_back(0.5f);
    this->uv.push_back(0.33f);
    // 0.75f, 0.66f
    this->uv.push_back(0.75f);
    this->uv.push_back(0.66f);
    // 0.75f, 0.33f
    this->uv.push_back(0.75f);
    this->uv.push_back(0.33f);

    // 1, 0.66f
    this->uv.push_back(1.0f);
    this->uv.push_back(0.66f);
    // 1, 0.33f
    this->uv.push_back(1.0f);
    this->uv.push_back(0.33f);

    // 0.25f, 1
    this->uv.push_back(0.25f);
    this->uv.push_back(1.0f);
    // 0.5f, 1
    this->uv.push_back(0.5f);
    this->uv.push_back(1.0f);

    // 0.25f, 0
    this->uv.push_back(0.25f);
    this->uv.push_back(0.0f);
    // 0.5f, 0
    this->uv.push_back(0.5f);
    this->uv.push_back(0.0f);

    // --------------------

    // front
    // 0, 2, 1,
    // 1, 2, 3,
    this->indices.push_back(0);
    this->indices.push_back(2);
    this->indices.push_back(1);

    this->indices.push_back(1);
    this->indices.push_back(2);
    this->indices.push_back(3);

    // back
    // 4, 5, 6,
    // 5, 7, 6,
    this->indices.push_back(4);
    this->indices.push_back(5);
    this->indices.push_back(6);

    this->indices.push_back(5);
    this->indices.push_back(7);
    this->indices.push_back(6);

    // top
    // 6, 7, 8,
    // 7, 9, 8,
    this->indices.push_back(6);
    this->indices.push_back(7);
    this->indices.push_back(8);

    this->indices.push_back(7);
    this->indices.push_back(9);
    this->indices.push_back(8);

    // bottom
    // 1, 3, 4,
    // 3, 5, 4,
    this->indices.push_back(1);
    this->indices.push_back(3);
    this->indices.push_back(4);

    this->indices.push_back(3);
    this->indices.push_back(5);
    this->indices.push_back(4);

    // left
    // 1, 11, 10,
    // 1, 4, 11,
    this->indices.push_back(1);
    this->indices.push_back(11);
    this->indices.push_back(10);

    this->indices.push_back(1);
    this->indices.push_back(4);
    this->indices.push_back(11);

    // right
    // 3, 12, 5,
    // 5, 12, 13
    this->indices.push_back(3);
    this->indices.push_back(12);
    this->indices.push_back(5);

    this->indices.push_back(5);
    this->indices.push_back(12);
    this->indices.push_back(13);

    // cout << this->indexed_vertices.size() << endl;
    // cout << this->uv.size() << endl;
    // cout << this->indices.size() << endl;
    this->calculate_normals();
}

void Mesh::calculate_normals()
{
    // vertice normals
    //  cout << "made it to calculate_normals" << endl;
    //  this->normals.resize(this->indexed_vertices.size());
    //  for (unsigned int i = 0; i < this->indexed_vertices.size (); i++)
    //      this->normals[indices[i]] = vec3 (1.0, 1.0, 0.0);
    //  for (unsigned int i = 0; i < this->indices.size (); i+=3) {
    //      vec3 e01 = this->indexed_vertices[this->indices[i + 1]] -  this->indexed_vertices[this->indices[i + 0]];
    //      vec3 e02 = this->indexed_vertices[this->indices[i + 2]] -  this->indexed_vertices[this->indices[i + 0]];
    //      vec3 n = cross(e01, e02);
    //      n = normalize(n);
    //      for (unsigned int j = 0; j < 3; j++)
    //          this->normals[this->indices[i + j]] += n;
    //  }
    //  for (unsigned int i = 0; i < this->indexed_vertices.size (); i++){
    //      this->normals[this->indices[i]] = normalize(this->normals[this->indices[i]]);
    //      // cout << this->indexed_vertices[this->indices[i]].x
    //      //     << this->indexed_vertices[this->indices[i]].y
    //      //     << this->indexed_vertices[this->indices[i]].z << endl;
    //  }

    this->faceNormals.clear();
    // cout << (int)this->indices.size()/3 << endl;
    this->faceNormals.resize((int)this->indices.size() / 3);
    // TODO: implémenter le calcul des normales par face
    // Attention commencer la fonction par triangle_normals.clear();
    // Iterer sur les triangles
    vec3 e_10, e_20, n;
    for (int i = 0; i < (int)this->indices.size(); i += 3)
    {
        // cout << i << endl;
        e_10 = this->indexed_vertices[this->indices[i + 1]] - this->indexed_vertices[this->indices[i]];
        e_20 = this->indexed_vertices[this->indices[i + 2]] - this->indexed_vertices[this->indices[i]];
        e_10 = normalize(e_10);
        e_20 = normalize(e_20);
        this->faceNormals[i / 3] = cross(e_10, e_20);
    }
}

void Mesh::setTexture(MaterialType materialType, GLint shader)
{
    this->textureID = glGetUniformLocation(shader, "texSampler");
    switch (materialType)
    {
    case WOOD:
        this->texture = loadBMP_custom("textures/wood1.bmp");
        break;
    case EMERALD:
        break;
    case BLACK_RUBBER:
        break;
    }
}
