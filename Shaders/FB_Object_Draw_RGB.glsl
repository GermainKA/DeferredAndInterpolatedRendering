#version 330 

#ifdef VERTEX_SHADER

in vec2 glVertex;
in vec2 glTexCoord;
out vec2 frgTexCoord;
void main() {
    gl_Position = vec4(glVertex,0.0,1.0);

    frgTexCoord = glTexCoord ; 

};

#endif

#ifdef FRAGMENT_SHADER

precision mediump float;
uniform sampler2D Buffer;
in vec2 frgTexCoord;
out vec4 glFragColor;
void main() {
    glFragColor = texture(Buffer,frgTexCoord) ;
};
#endif