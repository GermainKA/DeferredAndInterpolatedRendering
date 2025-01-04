#include "Scene.h"

Scene::Scene(){
    image =  new Image(read_image("data/terrain/Clipboard01.png"));
    instancesPos = new std::vector<vec3> ;
    m_Scene_Width = image->width();
    m_Scene_Height = image->height();
        m_Cube = new Cube();
        Color color;
        vec3 pos;
        float z;
        for (int x = 0; x <  image->width() ; x+=2) {
            for (int y = 0; y < image->height() ; y+=2) {
                 color = image->operator()(x, y);
                 z = (color.r + color.b + color.b) / 3;

                 pos = { (float)x,(float)y,(float)z * 20 };
                 instancesPos->push_back(pos);
            }
        }
        //glGenVertexArrays(1, &m_VAO);
        glBindVertexArray( m_Cube->getVertexArrayId());

        // Configurer les attributs de position et de normale du cube
        glBindBuffer(GL_ARRAY_BUFFER, m_Cube->getVertexBufferId());
        

        // Configurer l'attribut d'instance
        glGenBuffers(1, &m_instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instancesPos->size() * sizeof(vec3), instancesPos->data(), GL_STATIC_DRAW);

        GLuint location = glGetAttribLocation(m_Cube->getShaderId(), "instancePosition");
        if (location == -1) {
            std::cerr << "Attribut 'instancePosition' non trouvé dans le shader" << std::endl;
        }
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
        glVertexAttribDivisor(location, 1);
        
        glBindVertexArray(0);

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre

        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

}

Scene::~Scene(){
    delete image;
    delete m_Cube;
    delete instancesPos;
}

void Scene::onDraw(Transform mat4Projection,Transform mat4View ){


    Transform model = Identity();
    mat4View = mat4View * RotationX(-90) * Translation(0.0f, 0.0f, -10.0f) * Translation((float)-image->width() / 2, 0.0f, 0.0f);
    mat4Projection = Perspective(45, float(window_width()) / float(window_height()), 0.1, 1000);
    glUseProgram(m_Cube->getShaderId());

    //Attach Matrix
    glUniformMatrix4fv(m_Cube->getViewMatrixLocation(), 1, GL_TRUE, mat4View.data());
    glUniformMatrix4fv(m_Cube->getProjectionMatrixLocation(), 1, GL_TRUE, mat4Projection.data());
    //Attach Attrib
    //glUniform4f(m_Cube->getColorLocation(), 0.5, 0.5, 0.5, 1);
            
    glBindVertexArray( m_Cube->getVertexArrayId());    
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_Cube->getVertexCount(), instancesPos->size());
    glUseProgram(0);
}