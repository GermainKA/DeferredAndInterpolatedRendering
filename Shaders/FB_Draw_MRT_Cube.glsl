#version 330 core

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 aPosition;
layout(location= 1) in vec3 aNormal;
layout(location= 2) in vec2 aTexCoord;
//layout(location= 3) in vec3 instancePosition;

uniform mat4 viewMat ;
uniform mat4 projectionMat;

out vec4 frgPosition;
out vec3 frgNormal;
out vec2 frgTexCoord;



void main() {
    mat4 model = mat4(1.0);
    //model[3] = vec4(instancePosition, 1.0);
    frgPosition = viewMat * model * vec4(aPosition,1);
    gl_Position= projectionMat * frgPosition;
    frgNormal = mat3(transpose(inverse(viewMat * model))) * aNormal; 
    frgTexCoord = aTexCoord;
};

#endif

#ifdef FRAGMENT_SHADER

layout(location = 0) out vec4 out_Position; // Position
layout(location = 1) out vec4 out_Normal;   // Normal
layout(location = 2) out vec4 out_Diffuse;   // Diffuse
layout(location = 3) out vec4 out_Specular; // Specular

in vec4 frgPosition;     // Position du fragment dans l'espace monde
in vec3 frgNormal;      // Normale interpolée
in vec2 frgTexCoord;   // Coordonnées de texture

//uniform sampler2D diffuseMap;   // Texture diffuse
//uniform sampler2D specularMap;  // Texture spéculaire

void main() {
    // Position dans l'espace monde
    out_Position = vec4(frgPosition.xyz,1.0) ;

    // Normale normalisée dans l'espace monde
    out_Normal = vec4(normalize(frgNormal), 0.0);

    // Couleur diffuse
    out_Diffuse = vec4(1.0,0.0,1.0,1.0);
    //out_Diffuse = texture(diffuseMap, texCoords);

    // Couleur spéculaire
    out_Specular = vec4(1.0,1.0,0.0,1.0);
    //out_Specular = texture(specularMap, texCoords);
    
}

#endif
