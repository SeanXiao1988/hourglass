// TODO:
// rotation with y/x axis in orthographic projection
// using glm frustum to achieve that effect then do normal matrix tricks

#include "HG.h"
#include "lua.hpp"

using namespace HG;

typedef std::map<uint32_t, Recti> RectMap;
#define RECT_COUNT 100

void mainloop()
{
    bool running = true;
    double simulationTime = glfwGetTime();
    double realTime = 0.0;
    
    double timeScale = 0.5;
    
    GLuint rt = RENDER.rtargetCreate(800, 600);
    Quad q;
    quad_set_default(&q);
    q.tex = RENDER.rtargetGetTexture(rt);
    quad_set_texture_rect(&q, 0, 0, 800, 600, 800, 600);
    quad_set_coord(&q, 0, 0, 800, 600);
    
    while (running)
    {
        realTime = glfwGetTime();
        
        while (simulationTime/timeScale < realTime)
        {
            simulationTime += 0.016;
            SCENEMANAGER.update(0.016);
        }
        
        RENDER.calculateFPS();
        static float fpstimer = 0.0f;
        fpstimer += 0.016;
        if (fpstimer > 1.0f)
        {
            fpstimer = 0.0f;
            //printf("%.1f\n", RENDER.getFPS());
        }
        
        //*/ render normal stuff
        RENDER.beginScene(0);
        RENDER.clear();
        
        SCENEMANAGER.render();
        
        CONSOLE.render(0.016f);
        
        RENDER.endScene();
        //*/

        /* render screen ,GUI and console
        RENDER.beginScene();
        RENDER.clear();
        
        RENDER.renderQuad(&q);
        
        CONSOLE.render(0.016);
        
        RENDER.endScene();
        //*/
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

#if PLATFORM == PLATFORM_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, const char * argv[])
#endif
{
    APPLICATION.initialize();
    APPLICATION.setMainLoopFunction(mainloop);
    return APPLICATION.run();
}