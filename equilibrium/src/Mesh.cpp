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
    }
    if (!extension.compare("obj"))
        loadOBJ(filename, this->indexed_vertices, this->indices, this->uv, this->vericesNormals);

    this->modelID = modelID;
    // this->initializeMaterial();
    this->computeNormals();
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

void Mesh::generateMesh()
{
    if (this->objectType == SPHERE)
        this->generateSphere();
    else if (this->objectType == PLANE)
        this->generatePlane();
    else if (this->objectType == INFINITE_PLANE)
        this->generatePlane();
    else if (this->objectType == CUBE)
        this->generateCube();
    else
    {
        printf("Ce maillage n'est pas supporté \n");
        exit(1);
    }
    this->computeNormals();
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
//     this->computeNormals();
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

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(3);

    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
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
}

void Mesh::generateCube()
{
    this->indexed_vertices.clear();
    this->indices.clear();
    this->uv.clear();
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

    this->computeNormals();
}

void Mesh::computeFaceNormals()
{

    int facesNb = (int)indices.size() / 3;
    faceNormals.clear();
    faceNormals.resize(facesNb, vec3(0.));
    vec3 e_10, e_20, n;
    for (int i = 0; i < facesNb; i++)
    {
        e_10 = indexed_vertices[indices[i * 3 + 1]] - indexed_vertices[indices[i * 3 + 0]];
        e_20 = indexed_vertices[indices[i * 3 + 2]] - indexed_vertices[indices[i * 3 + 0]];
        e_10 = normalize(e_10);
        e_20 = normalize(e_20);
        faceNormals[i] = cross(e_20, e_10);

        faceNormals[i] = normalize(faceNormals[i]);
    }
}

// Compute vertices normals as the average of its incident faces normals
void Mesh::computeVerticesNormals()
{

    vericesNormals.clear();
    int normales_size = indexed_vertices.size(),
        faces_size = (int)faceNormals.size();
    vericesNormals.resize(normales_size, vec3(0.));

    for (int face = 0; face < faces_size; face++)
    {
        for (int sommet = 0; sommet < 3; sommet++)
        {
            vericesNormals[indices[face * 3 + sommet]] += faceNormals[face];
        }
    }
    for (int sommet = 0; sommet < normales_size; sommet++)
    {
        vericesNormals[sommet] = normalize(vericesNormals[sommet]);
    }
}

void Mesh::computeNormals()
{
    computeFaceNormals();
    computeVerticesNormals();
}
