
//! \file tuto9.cpp utilisation d'un shader 'utilisateur' pour afficher un objet Mesh

#include <cmath>

#include "mat.h"
#include "mesh.h"
#include "wavefront.h"

#include "orbiter.h"
#include "program.h"
#include "uniforms.h"
#include "draw.h"

#include "app_time.h"        // classe Application a deriver
#include "FB_Objet.h"
#include "Scene.h"



// utilitaire. creation d'une grille / repere.
Mesh make_grid( const int n= 10 )
{
    Mesh grid= Mesh(GL_LINES);
    
    // grille
    grid.color(White());
    for(int x= 0; x < n; x++)
    {
        float px= float(x) - float(n)/2 + .5f;
        grid.vertex(Point(px, 0, - float(n)/2 + .5f)); 
        grid.vertex(Point(px, 0, float(n)/2 - .5f));
    }

    for(int z= 0; z < n; z++)
    {
        float pz= float(z) - float(n)/2 + .5f;
        grid.vertex(Point(- float(n)/2 + .5f, 0, pz)); 
        grid.vertex(Point(float(n)/2 - .5f, 0, pz)); 
    }
    
    // axes XYZ
    grid.color(Red());
    grid.vertex(Point(0, 0.1, 0));
    grid.vertex(Point(1, 0.1, 0));
    
    grid.color(Green());
    grid.vertex(Point(0, 0.1, 0));
    grid.vertex(Point(0, 1.1, 0));
    
    grid.color(Blue());
    grid.vertex(Point(0, 0.1, 0));
    grid.vertex(Point(0, 0, 1));
    
    return grid;
}

// utilise AppTime pour les screenshots...
class TP : public AppTime
{
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    TP( ) : AppTime(1024, 640) {}
    
    int init( )
    {

        
        Point center(0,0,0);
        m_camera.lookat(center, 60);


        m_WindowSplitWidth = (int) window_width() / 2;
        m_WindowSplitHeigth = (int) window_height() / 2;
        
        //Init GBuffer
        m_Scene = new Scene();
        m_Cube = new Cube();
        m_GBuffer = new FB_Objet(m_WindowSplitWidth,m_WindowSplitHeigth,GL_TEXTURE_2D,GL_TEXTURE_2D,3,GL_NEAREST);

        FB_Objet::staticInit();
        
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest


        return 0;   // ras, pas d'erreur
        
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        delete m_GBuffer ;
        delete m_Scene ;
        delete m_Cube;
        
        FB_Objet::staticDestroy();
        return 0;
    }
    
    // dessiner une nouvelle image
    int render( )
    {
        // recupere l'etat de la souris
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        
        // deplace la camera
        if(mb & SDL_BUTTON(1))
            m_camera.rotation(mx, my);      // tourne autour de l'objet
        else if(mb & SDL_BUTTON(3))
            m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height()); // deplace le point de rotation
        else if(mb & SDL_BUTTON(2))
            m_camera.move(mx);           // approche / eloigne l'objet
        
        // recupere l'etat de la molette / touch
        SDL_MouseWheelEvent wheel= wheel_event();
        if(wheel.y != 0)
        {
            clear_wheel_event();
            m_camera.move(8.f * wheel.y);  // approche / eloigne l'objet
        }
        
        // recharge le shader program
        if(key_state('r'))
        {
        }
        

        // configurer le shader program
        // . recuperer les transformations
        Transform model= RotationX(global_time() / 20) * Scale(10);
        Transform view= m_camera.view();
        Transform projection= m_camera.projection(window_width(), window_height(), 45);

        //m_Scene = new Scene();
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

                    
        m_GBuffer->enable();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Scene->onDraw(projection,view);
        m_GBuffer->disable();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0,m_WindowSplitWidth,m_WindowSplitHeigth);
        m_GBuffer->onDraw(GL_COLOR_ATTACHMENT0);
        glViewport(m_WindowSplitWidth,0,m_WindowSplitWidth*2,m_WindowSplitHeigth);
        m_GBuffer->onDraw(GL_COLOR_ATTACHMENT1);
        glViewport(0,m_WindowSplitHeigth,m_WindowSplitWidth,m_WindowSplitHeigth*2);
        m_GBuffer->onDraw(GL_COLOR_ATTACHMENT2);
        glViewport(m_WindowSplitWidth,m_WindowSplitHeigth,m_WindowSplitWidth*2,m_WindowSplitHeigth*2);
        m_GBuffer->onDraw(GL_COLOR_ATTACHMENT3);

        

        
        return 1;
    }

protected:
    Mesh m_objet;
    Mesh m_grid;
    Orbiter m_camera;
    GLuint m_program;
    FB_Objet * m_GBuffer;
    Scene * m_Scene;
    Cube * m_Cube;
    int m_WindowSplitWidth;
    int m_WindowSplitHeigth;

};


int main( int argc, char **argv )
{
    TP tp;
    tp.run();
    
    return 0;
}

