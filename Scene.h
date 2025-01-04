#pragma once
#include "mat.h"
#include "wavefront.h"
#include "Cube.h"
#include "vector"
#include "text.h"
#include "ostream"
#include "fstream"
#include "chrono"


class Scene {

    public :

    Scene();
    ~Scene();

    std::vector<vec3> * getinstancesPos(){return instancesPos;}
    int getSceneWidth() {return m_Scene_Width;}
    int getSceneHeight() {return m_Scene_Height;}





    void onDraw(Transform mat4Projection,Transform mat4View );

    protected:
    
    Transform m_Model;
    Cube * m_Cube;
    Image * image;
    std::vector<vec3> * instancesPos;
    GLuint _m_texture;
    GLuint m_instanceVBO;
    GLuint m_VAO;
    int m_Scene_Width;
    int m_Scene_Height;



};