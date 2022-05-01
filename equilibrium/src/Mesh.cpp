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

Mesh::Mesh(string filename, GLint modelID)
{
    string extension = getFileExt(filename);
    if (!extension.compare("off")) {
        loadOFF(filename, this->indexed_vertices, this->indices);
        this->calculate_normals();
    }
    if (!extension.compare("obj"))
        loadOBJ(filename, this->indexed_vertices, this->indices, this->uv, this->normals);

    this->modelID = modelID;
    this->initializeMaterial();
}

Mesh::Mesh(string tag, float size, GLint modelID)
{
    this->size = size;
    this->generateMesh(tag);
    this->modelID = modelID;
    this->initializeMaterial();
}

Mesh::~Mesh()
{
    delete this;
}

void Mesh::initializeMaterial(){
    this->material.ambient = vec3(0.6, 0.5, 0.3) ; // make everything yellowish by default
    this->material.diffuse = vec3(1.) ; 
    this->material.specular = vec3(1.);
    this->material.shininess = 0.5;
}

void Mesh::generateMesh(string meshType)
{
    if (meshType.compare("sphere") == 0)
        this->generateSphere();
    else if (meshType.compare("plane") == 0)
        this->generatePlane();
    else
    {
        printf("Ce maillage n'est pas supporté \n");
        exit(1);
    }
    this->calculate_normals();
}

void Mesh::generatePlane()
{

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
            vec3 vertex = vec3(((float)i / (float)(this->size - 1) * length) - length / 2, ((float)j / (float)(this->size - 1) * length) - length / 2, 0);
            this->indexed_vertices.push_back(vertex);
            this->uv.push_back((float)j / (float)(size - 1));
            this->uv.push_back((float)i / (float)(size - 1));
        }
    }

    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - 1; ++j)
        {
            indices.push_back(i * size + j);
            indices.push_back(i * size + (j + 1));
            indices.push_back((i + 1) * size + j);

            indices.push_back(i * size + (j + 1));
            indices.push_back((i + 1) * size + (j + 1));
            indices.push_back((i + 1) * size + j);
        }
    }
    this->calculate_normals();
}

void Mesh::generateSphere()
{

    float x, y, z, u, v;
    float theta, phi;

    int size = this->size;

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

            x = cos(theta) * cos(phi);
            y = sin(theta) * cos(phi);
            z = sin(phi);

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

void Mesh::calculate_normals()
{
    //vertice normals
    // cout << "made it to calculate_normals" << endl;
    // this->normals.resize(this->indexed_vertices.size());
    // for (unsigned int i = 0; i < this->indexed_vertices.size (); i++)
    //     this->normals[indices[i]] = vec3 (1.0, 1.0, 0.0);
    // for (unsigned int i = 0; i < this->indices.size (); i+=3) {
    //     vec3 e01 = this->indexed_vertices[this->indices[i + 1]] -  this->indexed_vertices[this->indices[i + 0]];
    //     vec3 e02 = this->indexed_vertices[this->indices[i + 2]] -  this->indexed_vertices[this->indices[i + 0]];
    //     vec3 n = cross(e01, e02);
    //     n = normalize(n);
    //     for (unsigned int j = 0; j < 3; j++)
    //         this->normals[this->indices[i + j]] += n;
    // }
    // for (unsigned int i = 0; i < this->indexed_vertices.size (); i++){
    //     this->normals[this->indices[i]] = normalize(this->normals[this->indices[i]]);
    //     // cout << this->indexed_vertices[this->indices[i]].x
    //     //     << this->indexed_vertices[this->indices[i]].y
    //     //     << this->indexed_vertices[this->indices[i]].z << endl;
    // }

    this->normals.clear();
    // cout << (int)this->indices.size()/3 << endl;
    this->normals.resize((int)this->indices.size()/3);
    //TODO: implémenter le calcul des normales par face
    //Attention commencer la fonction par triangle_normals.clear();
    //Iterer sur les triangles
    vec3 e_10,e_20,n;
    for (int i = 0; i < (int)this->indices.size(); i+=3){
        // cout << i << endl;
        e_10 = this->indexed_vertices[this->indices[i + 1]] - this->indexed_vertices[this->indices[i]];
        e_20 = this->indexed_vertices[this->indices[i + 2]] - this->indexed_vertices[this->indices[i]];
        e_10 = normalize(e_10);
        e_20 = normalize(e_20);
        this->normals[i/3] = cross(e_10, e_20);  
    }

}
