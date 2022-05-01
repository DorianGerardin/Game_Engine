#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    bool activated;
    vec3 position;
    Material material;
};

uniform Material material;
Material finalMaterial;
uniform Light light;
Light light2;

uniform vec3 cameraPos;

// Ouput data
//in vec3 ourColor;
in vec2 UV;
in float zPos;
in vec3 FragPos;
in vec3 normal;

//out vec4 Color;
out vec3 color;

uniform sampler2D texSampler;
uniform sampler2D hmapSampler;
uniform bool hasTexture;
uniform bool isOutsideLoop;

void main(){
	// add texture and materials
	if(hasTexture) {
		color = texture( texSampler, UV ).rgb;
	} else {
		color = material.ambient;
	}


	finalMaterial.ambient = color;
	finalMaterial.diffuse = material.diffuse;
	finalMaterial.specular = material.specular;
	finalMaterial.shininess = material.shininess;

	// ambient
    vec3 ambient = light.material.ambient * finalMaterial.ambient;

	// diffuse 
    vec3 norm = normalize(normal);
    norm = vec3(1.);
    norm = normalize(norm);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.material.diffuse * (diff * finalMaterial.diffuse);

	// specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), finalMaterial.shininess);
    vec3 specular = light.material.specular * (spec * finalMaterial.specular);  
        
    color = (ambient + diffuse + specular) * color;
}