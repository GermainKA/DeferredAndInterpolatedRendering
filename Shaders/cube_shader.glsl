#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec3 normal;
layout(location= 2) in vec3 instancePosition;


uniform mat4 view ;
uniform mat4 projection;

out vec3 view_normal;
out vec2 view_texcoord;
flat out int instanceTextureLayer;


void main( )
{
    mat4 model = mat4(1.0);
    model[3] = vec4(instancePosition, 1.0);
    gl_Position= projection * view * model * vec4(position, 1);

    view_normal = mat3(transpose(inverse(view * model))) * normal; 
    view_texcoord = texcoord;
    instanceTextureLayer = textureLayer;

}

#endif


#ifdef FRAGMENT_SHADER

uniform vec4 color;
in vec3 view_normal;
in vec2 view_texcoord;
flat in int instanceTextureLayer;

out vec4 fragment_color;


void main( )
{

    vec3 norm = normalize(view_normal);
    //vec4 color= texture(textureArray,vec3(view_texcoord,instanceTextureLayer));
    //fragment_color= color;
    fragment_color= vec4(color * abs(norm.z));

}

#endif
